# Plan: a SIM7600 AT-command library for the T-Deck

A C++ library that hides the AT command protocol behind a small, readable API, so the phone firmware never builds or parses AT strings itself. Priorities: **text messages and voice calls first**, with room left for mobile data (web) as a stretch goal.

Background: [board-test-findings.md](board-test-findings.md) (commands verified on the bench), [at-interface-options.md](at-interface-options.md) (command lists), [legacy/xiao_phone-notes.md](legacy/xiao_phone-notes.md) (what not to repeat), [next-steps.md](next-steps.md).

## Goals

- **Logical API:** `modem.sendSms(number, text)`, `modem.dial(number)`, not `AT+CMGS=...`. A reader should understand the phone firmware without knowing AT.
- **Non-blocking:** the T-Deck runs a display and a keyboard. Nothing in the library may stall the main loop for seconds (a bad SMS send can take 2+ minutes). Long operations return immediately and report through callbacks or events.
- **Asynchronous events handled properly:** `RING`, `+CLIP`, `+CMTI`, `NO CARRIER` and so on can arrive at any time, including in the middle of another command's response. The legacy sketch mangled these; the library must not.
- **Well commented and well documented:** every public function has a Doxygen comment (what it does, parameters, return, which AT commands it uses, failure modes). A short guide and runnable examples ship with it.
- **Testable without hardware:** the serial port is an interface, so a fake modem can replay recorded conversations in tests.
- **Small and dependency-light:** Arduino framework on ESP32-S3 (PlatformIO or Arduino IDE), no heap churn in the hot path where avoidable.

Non-goals for the first version: PDU-mode SMS (non-ASCII text, long messages), GNSS, multiplexing (`AT+CMUX`), 3G/2G fallback.

## Verified facts the design rests on

From the bench test of the SIM7600A-H on the old Waveshare HAT with a Ting line:

- UART 9600 baud, 8N1, no flow control (`AT+IPREX?` returns 9600, the permanent rate). Must be configurable. Note `AT+IPR` only changes the rate until the next reboot; `AT+IPREX` is the permanent one.
- Text mode SMS: `AT+CMGF=1`, `AT+CMGS="+1<number>"`, wait for `> `, text, Ctrl-Z, result `+CMGS: <n>` then `OK`. Failures arrive as `+CMS ERROR: <reason>` and can take minutes to arrive.
- Voice: `ATD+<E.164>;`, poll `AT+CLCC` (state 2 dialing, 3 alerting/ringing, 0 active), end with `AT+CHUP`. The module also prints `VOICE CALL: BEGIN`, `VOICE CALL: END: <hhmmss>` and `NO CARRIER`.
- A pending SMS send blocks other commands, including `ATD`.
- `AT+CNUM` can be empty, so do not depend on it.
- Calls and SMS work over VoLTE (already enabled on the module via `at+voltesetting`).
- Registration: `AT+CEREG?` 0,1 (home) or 0,5 (roaming).

Not yet verified (flag as TODO in code and tests): incoming `RING`/`+CLIP`/`+CMTI` handling, `AT+CNMI` behaviour, `AT+CMGR`/`AT+CMGL`/`AT+CMGD` on this firmware, DTMF, and anything about data.

## Layered design

```
 phone firmware (UI, contacts, notifications)
        |   events + simple calls
 +------------------+
 | Phone            |  high level: sms, call, network state     (public API)
 +------------------+
 | Sms | Call | Net |  feature modules: build commands, parse results
 +------------------+
 | CommandQueue     |  one command in flight, timeouts, retries
 +------------------+
 | Parser           |  line reader, response vs URC routing
 +------------------+
 | Transport        |  Stream-like interface (HardwareSerial, fake)
 +------------------+
```

- **Transport:** wraps an Arduino `Stream` (the T-Deck's `Serial1`). A fake implementation feeds scripted bytes in tests.
- **Parser:** accumulates bytes into lines (terminated by CR LF, handling the `> ` SMS prompt which has no newline). Classifies each line as a command response (`OK`, `ERROR`, `+CMS ERROR`, data lines) or an unsolicited result code (URC), and routes it. This is the layer that fixes the legacy sketch's `readString()` problem.
- **CommandQueue:** owns the rule "one AT command in flight". Each command has an expected terminator, a timeout (short for simple queries, up to 150 s for `AT+CMGS`), and a completion callback. Commands from the UI are queued, not dropped. Special commands with a prompt (`AT+CMGS`) are modelled as a two-phase command.
- **Feature modules:** `Sms`, `Call`, `Network` (and later `Data`) build commands, parse their specific replies and URCs, and keep their own state. They know AT; nothing above them does.
- **Phone:** the single object the firmware uses. It wires the modules together, runs the startup sequence and exposes events.

## Public API (draft)

```cpp
// Events the firmware subscribes to; all delivered from Phone::loop(), never from interrupts.
struct PhoneEvents {
  void (*onSmsReceived)(const Sms& msg);          // new text arrived
  void (*onSmsSent)(SmsId id, SmsResult result);  // outcome of sendSms()
  void (*onIncomingCall)(const char* number);     // RING + caller ID
  void (*onCallStateChanged)(CallState state);    // Dialing, Ringing, Active, Ended(reason)
  void (*onNetworkChanged)(NetworkState state);   // registered, signal, operator
};

class Phone {
 public:
  Phone(Stream& serial, PhoneEvents events);
  void begin(uint32_t baud = 9600);   // runs the startup sequence
  void loop();                        // call every iteration; does all I/O

  // --- Network ---
  NetworkState network() const;       // registration, signal (CSQ), operator (COPS)
  bool isReady() const;               // SIM ready and registered

  // --- SMS ---
  SmsId sendSms(const char* number, const char* text);   // queued; result via onSmsSent
  bool  readSms(int index, Sms& out);
  void  deleteSms(int index);

  // --- Voice ---
  bool dial(const char* number);      // ATD+...;
  bool answer();                      // ATA
  bool hangUp();                      // AT+CHUP
  void setVolume(uint8_t level);      // AT+CLVL
  void setMute(bool muted);           // AT+CMUT
  CallState callState() const;
};
```

Conventions:
- Numbers are accepted as digits with an optional leading `+`, normalised to E.164 (assume +1 for 10 digits as a configurable default region). Validation errors return immediately.
- Text is ASCII/GSM-7 in version 1, length limited to 160 characters, with a clear error otherwise.
- All results use small enums (`SmsResult::Ok`, `NetworkNotReady`, `Timeout`, `NetworkError`, ...) with `toString()` helpers, plus the raw modem error string for logging.

## Startup sequence (in `begin()` / background)

1. Open the serial port at the configured baud; send `AT` until `OK` (the module may still be booting, allow up to about 20 s).
2. `ATE0` (echo off) so replies are not mixed with echoed commands.
3. `AT+CMGF=1`, `AT+CSCS="GSM"`, `AT+CLIP=1`, `AT+CNMI=2,1` (notify new SMS by index), `AT+CMEE=2` (verbose errors), `AT+CSDVC=<route>`. Several of these are documented as not saved across reboot (`AT+CLIP` and `AT+CSDVC` are `NO_SAVE` in the manual), so re-send them every start rather than trusting what the module remembers. Do not read `AT+CLIP?` back to check state: on the bench unit it returned `ERROR`; track the setting in the library instead.
   - Also check VoLTE once: `at+voltesetting?` should return `+VOLTESETTING: 1`. If it is 0, report it to the application rather than changing it automatically (the Waveshare wiki's enable procedure also writes an NV item, `at+cnv=...`, and is a deliberate one-time setup step).
4. `AT+CPIN?` then poll `AT+CEREG?` until registered; read `AT+COPS?`, `AT+CSQ`.
5. Emit `onNetworkChanged`. Re-poll registration and signal on a slow timer.
6. If the module resets or stops answering, re-run the sequence. On the bench a reset prints `RDY`, `+CPIN: READY`, `SMS DONE`, `PB DONE` in that order; treat `SMS DONE` and `PB DONE` as "SMS and phonebook ready" before sending messages.

## SMS module

- **Send:** two-phase command. Send `AT+CMGS="<num>"`, wait for the `> ` prompt (short timeout), send the text then Ctrl-Z, wait for `+CMGS: <mr>` + `OK` (timeout 150 s). While a send is pending the queue holds other commands, since the module will not accept them. Retry policy: do not blindly retry (a retry can double-send); report the failure and let the UI offer resend.
- **Receive:** `AT+CNMI=2,1` is confirmed on the bench unit (`AT+CNMI?` returns `2,1,0,0,0`; the module supports modes 0-2 and `<mt>` 0-3). On `+CMTI: "SM",<index>` read the message with `AT+CMGR=<index>` (indexes start at **0**), parse the header and body, raise `onSmsReceived`, then delete it. On startup list stored messages with `AT+CMGL="ALL"` to catch any missed. Reading a message marks it `REC READ`; list filters are the strings `"REC UNREAD"`, `"REC READ"`, `"STO UNSENT"`, `"STO SENT"`, `"ALL"`.
- **Text-mode format (seen on the bench):** `+CMGL: <index>,"<stat>","<sender>","","yy/MM/dd,hh:mm:ss±zz"` followed by the body on the next line, where `zz` is the time-zone offset in quarter hours. The sender is **not always a phone number**: the carrier's welcome text arrived from the short code `3001`. Contact matching and display must accept short codes and alphanumeric senders, not only E.164 numbers.
- **Storage:** `AT+CPMS?` shows `"SM"` with 30 slots, and `AT+CPMS=?` lists `"ME"`, `"MT"`, `"SM"`, `"SR"`, so module memory (`ME`) is an option that may hold more (capacity not checked). Delete each message after it is saved to the T-Deck (`AT+CMGD=<index>`; the `<delflag>` forms can clear all read or all sent messages in one go, and `4` deletes everything including unread, so avoid it casually) and keep history on the T-Deck's SD card or flash. Behaviour when storage is full (new texts rejected or lost) is **not verified**, so test it and make the library delete promptly.
- **Character set:** the bench sends used `AT+CSCS="GSM"`; the module also supports `"IRA"` and `"UCS2"`. A few ASCII characters map differently in the GSM 7-bit alphabet, so test `@ $ _ [ ] { } \ ^ ~ |` before promising that every ASCII message round-trips; fall back to `"IRA"` for plain ASCII if needed.
- **If sends fail with `+CMS ERROR`:** the Waveshare wiki's FAQ fix is `AT+CSCS="IRA"` and `AT+CSMP=17,167,0,0`. It was not needed once the Ting line was fixed (see [board-test-findings.md](board-test-findings.md)), but keep it in the troubleshooting notes. `AT+CGSMS?` (SMS service, currently 1) is another knob to check if a line misbehaves; not tested.
- **Later:** long (concatenated) and non-ASCII messages need PDU mode (`AT+CMGF=0`); keep the Sms module's encode/decode behind one interface so PDU can replace text mode without touching the API.

## Call module

- State machine: `Idle`, `Dialing`, `Ringing` (remote), `Active`, `Incoming`, `Ended`. Driven by `AT+CLCC` polling (about once a second while a call exists) **and** by URCs (`RING`, `+CLIP`, `VOICE CALL: BEGIN`, `VOICE CALL: END`, `NO CARRIER`), so a call ending is noticed quickly. `AT+CLCC=1` (supported on this module) can also report call changes automatically as `+CLCC:` lines, which could replace polling.
- Map every `AT+CLCC` `<stat>` value, not just the ones seen on the bench: 0 active, 1 held, 2 dialing, 3 alerting, 4 incoming, 5 waiting, 6 disconnect. When there are no calls, `AT+CLCC` returns only `OK` with no `+CLCC:` line, which the parser must treat as "no call". `<dir>` 0 is outgoing, 1 incoming; `<type>` 145 means an international number.
- `dial()` is refused unless the network is registered and no call is in progress. `hangUp()` is always allowed, and **uses `AT+CHUP` only**: on this module `AT+CVHU?` returns 1, which means `ATH` is ignored (the manual says mode 1 ignores ATH and drop-DTR but still answers `OK`), so never rely on `ATH`. `AT+CHUP` may print more than one `VOICE CALL: END: <time>` line (one per call) and can take up to 120 s to respond per the manual.
- Incoming: `RING` then `+CLIP: "<number>",<type>,...`; raise `onIncomingCall` once per call (RING repeats every few seconds).
- End reasons carried in the event: local hangup, remote hangup (`NO CARRIER` or `VOICE CALL: END`), busy, failed.
- Audio: version 1 routes audio through the HAT's headset jack (module default, `AT+CSDVC=1`; 3 selects the speaker per the Waveshare wiki; this module accepts 0, 1 and 3). `AT+CSDVC` is **not saved**, so re-apply it at every startup. `AT+CLVL` takes 0 to 5 (default 4) and **is** saved to the module, so do not write it on every UI change; write once when the user settles on a level. `AT+CMUT` only works **during a connected call** and resets to 0 when all calls end, so `setMute()` must be gated on an active call and the UI must not assume mute persists. The library exposes volume and mute and leaves the audio path selectable, so a later PCM/I2S route can slot in without changing the call API.
- DTMF: `AT+VTS=<char>[,<duration>]` sends a tone, or a quoted comma-separated string of up to 29 characters (`AT+VTS="1,3,5"`), valid characters 0-9 * # A-D, **only during an active voice call** (manual; the module's `AT+VTS=?` agrees). Not yet tried on a real call.
- Optional audio tuning from the Waveshare wiki, to try only if audio is noisy or quiet: `AT^PWRCTL=0,1,3` at startup before calls (TDD noise), `AT+CECM=1` and `AT+CECH=0x500` during a call (echo and volume). Not tested here.

## Network module

- Registration state from `AT+CEREG?` (and `+CEREG:` URCs if enabled with `AT+CEREG=1`; the module accepts modes 0 to 2), signal from `AT+CSQ` converted to bars, operator from `AT+COPS?`, own number from `AT+CNUM` if present, technology and band from `AT+CPSI?` for diagnostics.
- Exposes a single `NetworkState` struct for the status bar.

## Stretch goal: mobile data (web)

Treated as a separate module added after SMS and voice are solid; the layering above does not change.

- **First, find out what the Ting plan includes.** Check the plan on the Ting account (data allowance, APN). The module's PDP context 1 currently uses the APN `hologram` from an earlier setup, which may be wrong for Ting; the correct APN has to come from Ting. Do not assume data works.
- If data is available: set the APN (`AT+CGDCONT`), bring up the data bearer, then use the module's built-in TCP/IP or HTTP(S) AT commands (see the SIM7500_SIM7600_SIM7800_TCPIP_App_Note in `reference/` and the HTTP and SSL chapters of the AT manual). Offering `Data::httpGet(url, callback)` and a raw TCP socket interface covers the realistic uses (simple web fetch, an API call, maybe a lightweight text-only browser).
- **The UART speed is the first limit:** at 9600 baud the link carries roughly 1 KB per second. Before any data work, raise the rate with `AT+IPREX=<speed>` (permanent; the module supports 115200 up to 3686400) and switch the T-Deck's `Serial1` to match; use `AT+IPR` only for a temporary change. Make baud switching part of the library (change the module, wait for `OK`, then reopen the port at the new rate). Also enlarge the ESP32 serial RX buffer.
- The AT manual has the HTTP(S) commands (`AT+HTTPINIT`, `AT+HTTPPARA`, `AT+HTTPACTION`, `AT+HTTPREAD`, `AT+HTTPTERM`) and the SSL/TCP commands (`AT+CCHSTART`, `AT+CCHOPEN` and so on); the TCP/IP app note covers `AT+NETOPEN` and `AT+CIPOPEN`. The command names I had written down earlier (`AT+CHTTPS...`) are not in this manual, so work from these.
- Data shares the single UART with AT, so a long transfer competes with SMS and call control. Either serialise through the same command queue and accept the latency, or look at `AT+CMUX` later for separate channels.
- Cost and plan caps: add a usage counter and a hard off switch so the phone cannot run up charges.

## Documentation plan

- **Doxygen comments** on every public type and function, written for a firmware author who does not know AT: purpose, parameters, blocking behaviour (none), the AT commands used, error cases.
- **Inline comments** explaining *why* for anything non-obvious (the 150 s send timeout, the `> ` prompt handling, why one command at a time).
- **`docs/at-library/`** (or the library README): a short user guide, an architecture page (the diagram above), a table mapping each API call to its AT commands, and a troubleshooting page seeded from [board-test-findings.md](board-test-findings.md).
- **Examples** (PlatformIO/Arduino): serial-monitor modem bridge, send an SMS, receive SMS, place and answer a call, status display. Each example compiles on its own.
- A **CHANGELOG** and a short statement of tested hardware (SIM7600A-H on the Waveshare HAT, T-Deck).

## Review against the documentation (2026-10-18)

The plan was checked against the SIMCom SIM7500/SIM7600 AT manual V3.00 (`reference/`), the Waveshare SIM7600X HAT wiki, and read-only queries to the real module (SIM7600A-H, revision `SIM7600A-H_V1.1`). Every command the plan uses exists in the manual and answered on the bench. The issues found, and what changed in this plan:

| # | Finding | Source | Change |
|---|---|---|---|
| 1 | `ATH` is ignored on this module (`AT+CVHU?` is 1). | AT manual CVHU; bench | `hangUp()` uses `AT+CHUP` only |
| 2 | `AT+CLCC` states run 0 to 6, and no calls gives only `OK`. The plan listed only 0, 2 and 3. | AT manual CLCC | Full state map and "no `+CLCC:` line means no call" rule; `AT+CLCC=1` auto-report noted |
| 3 | `AT+CMUT` works only in a connected call and resets when calls end. | AT manual CMUT | `setMute()` gated on an active call |
| 4 | `AT+CSDVC` and `AT+CLIP` are not saved across reboot; `AT+CLVL` is saved. | AT manual | Re-apply at every start; do not write `AT+CLVL` on every UI change |
| 5 | `AT+CLIP?` returned `ERROR` on the bench unit. | Bench | Track CLIP state in the library; never read it back |
| 6 | `AT+IPR` is temporary; `AT+IPREX` is the permanent baud setting (bench: 9600). Earlier docs said to use `AT+IPR` and `AT&W`. | AT manual IPR/IPREX; bench | Plan and two older docs corrected; baud switching added to the data stretch |
| 7 | At 9600 baud the link is about 1 KB/s, which limits any data (web) work. | Arithmetic from the baud rate | Raise via `AT+IPREX` before data work; enlarge the ESP32 RX buffer |
| 8 | `VOICE CALL: BEGIN` is printed when the call connects, and `ATD`'s `OK` means only "started". | AT manual ATD; bench | Documented in the plan |
| 9 | `BUSY` and `NO ANSWER` result codes are not described in this manual (only `NO CARRIER` for a failed call). | AT manual | Removed `BUSY` from the URC list; test real busy and unanswered calls before relying on them |
| 10 | SMS indexes start at 0; text-mode `+CMGL`/`+CMGR` have a fixed layout and timestamp with quarter-hour zone; senders can be short codes (the carrier's welcome text came from `3001`). | AT manual CMGR; bench | Format and short-code handling added |
| 11 | `AT+CPMS=?` offers `ME`, `MT`, `SM`, `SR`; `SM` has 30 slots. Full-storage behaviour is undocumented. | Bench | Storage options and a test added |
| 12 | `AT+CSCS` has `IRA`, `GSM`, `UCS2`; some ASCII characters differ in GSM 7-bit. | AT manual; bench | Character-set caution added |
| 13 | Waveshare lists an SMS error fix (`CSCS="IRA"`, `CSMP=17,167,0,0`), VoLTE setup (`at+voltesetting`), `AT+CHUP`, and call-audio tuning (`AT^PWRCTL`, `AT+CECM`, `AT+CECH`). | Waveshare wiki | Added as troubleshooting, a startup VoLTE check and optional tuning |
| 14 | `AT+VTS` works only in an active call; string form is quoted, comma-separated, up to 29 characters. | AT manual VTS; bench `AT+VTS=?` | DTMF behaviour documented |
| 15 | The HTTP(S) commands in this manual are `AT+HTTPINIT`, `HTTPPARA`, `HTTPACTION`, `HTTPREAD`, `HTTPTERM`, plus the `AT+CCH...` SSL/TCP group; the `AT+CHTTPS...` names written earlier are not here. | AT manual | Stretch section corrected |

Still **not verified** on hardware: incoming `RING` and `+CLIP` handling, DTMF, behaviour when message storage is full, busy and no-answer calls, `AT+CLCC=1` auto-reporting, special characters in GSM mode, `ME` storage capacity, and all data commands. They stay as milestones in the plan, not assumptions.

## Update from the full hardware probe (2026-10-18)

A complete read-only probe of the module is written up in [sim7600a-h-at-reference.md](sim7600a-h-at-reference.md). Changes it implies for this plan:

- **Per-command latency is fixed at about 0.2 s to the first byte and 0.41 s to finish**, even for `AT`. Budget serial time accordingly (a five-command status refresh is about 2 s).
- **SMS storage:** `AT+CPMS="ME","ME","ME"` gives 255 slots (the SIM has 30). `"MT"` is listed as supported but rejected with `ERROR`. Use `ME` as the library's default store, after testing full-storage behaviour and persistence.
- **Errors:** with `AT+CMEE=1` the library gets numeric codes (`321` invalid index, `303` operation not supported); with `2` it gets text. Plain `ERROR` also means a malformed argument. Parse both numeric and text forms.
- **Status bar data:** `AT+CIND?` returns battery, signal, service, call, roaming and SMS-full indicators in one read; `AT+CESQ` gives LTE RSRQ/RSRP without parsing `AT+CPSI?`.
- **Hang-up:** `AT+CVHU?` is 1, so `ATH` is ignored; the library already uses `AT+CHUP`.
- **Hidden commands:** the module has many commands the manual does not document (an HTTPS client, FTP, SMTP, IMS settings, NV and shell commands). The library must never expose raw AT passthrough without a block list; the dangerous ones are listed in the reference.
- **Not yet tested** and still milestones, not assumptions: incoming calls, `ATH`, DTMF, `AT+CLCC=1` auto-reporting, full SMS storage, data.

## Testing

1. **Host unit tests** (no hardware): feed the Parser recorded byte streams and assert on events. Capture real traces from the bench (including URCs interleaved with replies, the slow `+CMS ERROR`, and the boot banner) into fixtures. Cover the Parser, CommandQueue timeouts, and each feature module's state machine against the fake transport.
2. **Hardware-in-loop tests:** sketches that exercise real send/receive/call and print pass/fail, run by hand on the T-Deck with the HAT.
3. **Fault injection:** module silent for minutes, garbled bytes, reset mid-command, call ends during `ATD`.

## Milestones

1. **Skeleton and transport:** repo/library layout, `Stream` transport and fake transport, build in PlatformIO for the T-Deck.
2. **Parser and queue:** line reading, response vs URC split, one-command-in-flight queue with timeouts. Tests from bench traces.
3. **Network and startup:** startup sequence, registration, signal, operator. Example: status printout.
4. **SMS send:** `sendSms` with prompt handling and long timeout. Example and hardware test.
5. **SMS receive:** `+CMTI`, read, delete, startup scan. Needs the receive-side bench test from [next-steps.md](next-steps.md) first.
6. **Voice:** dial, hang up, answer, incoming call, call state machine, volume and mute.
7. **Docs and examples pass:** Doxygen, guide, API-to-AT table, all examples compile.
8. **Integrate** with the T-Deck phone UI (display and keyboard).
9. **Stretch:** data module after confirming the Ting plan and APN.

## Open decisions

- Library layout: a standalone PlatformIO library in its own folder in this repo (preferred, so it can be reused) vs inside the firmware project.
- C++ standard and whether to allow the STL (`std::function`, `std::string`) or stay with C strings and function pointers for a lean footprint on the ESP32-S3. Default proposal: C++17, fixed-size buffers, function pointers or a small callback type, no dynamic allocation in the parser.
- Whether the library runs in the main loop (`loop()` polling, simplest) or in its own FreeRTOS task (smoother for a UI, needs thread-safe queues). Start with polling.
- Default region and number formatting rules for the contact list.
- Where message history lives (SD card vs flash).
