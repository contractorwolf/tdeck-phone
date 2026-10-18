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

- UART 9600 baud, 8N1, no flow control (`AT+IPR?` returns 9600). Must be configurable.
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
3. `AT+CMGF=1`, `AT+CSCS="GSM"`, `AT+CLIP=1`, `AT+CNMI=2,1` (notify new SMS by index), `AT+CMEE=2` (verbose errors).
4. `AT+CPIN?` then poll `AT+CEREG?` until registered; read `AT+COPS?`, `AT+CSQ`.
5. Emit `onNetworkChanged`. Re-poll registration and signal on a slow timer.
6. If the module resets or stops answering, re-run the sequence (watch for the boot messages `RDY`, `+CPIN: READY`, `SMS DONE`).

## SMS module

- **Send:** two-phase command. Send `AT+CMGS="<num>"`, wait for the `> ` prompt (short timeout), send the text then Ctrl-Z, wait for `+CMGS: <mr>` + `OK` (timeout 150 s). While a send is pending the queue holds other commands, since the module will not accept them. Retry policy: do not blindly retry (a retry can double-send); report the failure and let the UI offer resend.
- **Receive:** on `+CMTI: "SM",<index>` read the message with `AT+CMGR=<index>`, parse the header (sender, timestamp) and body, raise `onSmsReceived`, optionally delete it. On startup list stored messages with `AT+CMGL="ALL"` to catch any missed.
- **Storage:** SIM storage is small (about 30 messages on the SIM, per `AT+CPMS?`); delete after delivery to the app, and keep history on the T-Deck's SD card or flash.
- **Later:** long (concatenated) and non-ASCII messages need PDU mode (`AT+CMGF=0`); keep the Sms module's encode/decode behind one interface so PDU can replace text mode without touching the API.

## Call module

- State machine: `Idle`, `Dialing`, `Ringing` (remote), `Active`, `Incoming`, `Ended`. Driven by `AT+CLCC` polling (about once a second while a call exists) **and** by URCs (`RING`, `+CLIP`, `VOICE CALL: BEGIN`, `VOICE CALL: END`, `NO CARRIER`, `BUSY`), so a call ending is noticed quickly.
- `dial()` is refused unless the network is registered and no call is in progress. `hangUp()` is always allowed.
- Incoming: `RING` then `+CLIP: "<number>",<type>,...`; raise `onIncomingCall` once per call (RING repeats every few seconds).
- End reasons carried in the event: local hangup, remote hangup (`NO CARRIER` or `VOICE CALL: END`), busy, failed.
- Audio: version 1 routes audio through the HAT's headset jack (module default, `AT+CSDVC=1`). The library exposes volume and mute and leaves the audio path selectable, so a later PCM/I2S route to the T-Deck's speaker and mic can slot in without changing the call API.
- Optional: DTMF tones during a call (`AT+VTS`), to be checked against the AT manual.

## Network module

- Registration state from `AT+CEREG?` (and `+CEREG:` URCs if enabled with `AT+CEREG=1`), signal from `AT+CSQ` converted to bars, operator from `AT+COPS?`, own number from `AT+CNUM` if present, technology and band from `AT+CPSI?` for diagnostics.
- Exposes a single `NetworkState` struct for the status bar.

## Stretch goal: mobile data (web)

Treated as a separate module added after SMS and voice are solid; the layering above does not change.

- **First, find out what the Ting plan includes.** Check the plan on the Ting account (data allowance, APN). The module's PDP context 1 currently uses the APN `hologram` from an earlier setup, which may be wrong for Ting; the correct APN has to come from Ting. Do not assume data works.
- If data is available: set the APN (`AT+CGDCONT`), bring up the data bearer, then use the module's built-in TCP/IP or HTTP(S) AT commands (see the SIM7500_SIM7600_SIM7800_TCPIP_App_Note in `reference/` and the HTTP chapters of the AT manual). Offering `Data::httpGet(url, callback)` and a raw TCP socket interface covers the realistic uses (simple web fetch, an API call, maybe a lightweight text-only browser).
- Data shares the single UART with AT, so a long transfer competes with SMS and call control. Either serialise through the same command queue and accept the latency, or look at `AT+CMUX` later for separate channels.
- Cost and plan caps: add a usage counter and a hard off switch so the phone cannot run up charges.

## Documentation plan

- **Doxygen comments** on every public type and function, written for a firmware author who does not know AT: purpose, parameters, blocking behaviour (none), the AT commands used, error cases.
- **Inline comments** explaining *why* for anything non-obvious (the 150 s send timeout, the `> ` prompt handling, why one command at a time).
- **`docs/at-library/`** (or the library README): a short user guide, an architecture page (the diagram above), a table mapping each API call to its AT commands, and a troubleshooting page seeded from [board-test-findings.md](board-test-findings.md).
- **Examples** (PlatformIO/Arduino): serial-monitor modem bridge, send an SMS, receive SMS, place and answer a call, status display. Each example compiles on its own.
- A **CHANGELOG** and a short statement of tested hardware (SIM7600A-H on the Waveshare HAT, T-Deck).

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
