# SIM7600A-H AT reference: what the module actually does

A working reference for the SIMCom SIM7600A-H on the older Waveshare SIM7600X 4G HAT, written from **bench observation** and checked against the SIMCom AT Command Manual V3.00 and the Waveshare wiki. Where the module and the documents disagree, the module wins and the difference is called out. It feeds the [AT library plan](at-library-plan.md).

- Companion file: [sim7600a-h-at-command-index.md](sim7600a-h-at-command-index.md), a generated table of every command name found (896 rows) with what the module answered.
- Raw (sanitized) data: [reference/at-probe/](reference/at-probe/). Tools that produced it: [../tools/at-probe/](../tools/at-probe/).

## 1. How to read this

Every statement carries one of three labels.

| Label | Meaning |
|---|---|
| **Observed** | I sent the command to the real module and saw this. |
| **Documented** | Stated in the SIMCom manual or the Waveshare wiki, not independently seen. |
| **Unverified** | A guess or an inference. Treat as a lead, not a fact. |

Rules I followed while probing, so you can trust what was and was not tested:

- Only read-only forms were sent for exploration: `AT<cmd>=?` (what values are allowed) and `AT<cmd>?` (current value), plus the execute forms of known information commands (`ATI`, `AT+CGMR` and similar).
- **Never sent:** anything that resets, powers off, rewrites NV memory or firmware, opens a shell, sends a message, starts a network connection or places a call during probing. 190 command names from `AT+CLAC` were excluded for this reason (section 14).
- Settings I changed while testing are listed with their restore status in section 16.

Limits of this work: one module, one firmware, one carrier line, one antenna position, and a 9600 baud serial link. Behaviour on other firmware, SIMs or networks may differ. I did not test incoming calls, incoming-call handling, hang-up with `ATH`, DTMF, data connections or low-power modes.

## 2. The module under test

| Item | Observed value |
|---|---|
| Module | SIMCOM SIM7600A-H (North American variant, LTE Cat-4) |
| Firmware (`AT+GMR`, `AT+CGMR`) | `LE11B03SIM7600A-H` |
| Build (`AT+CSUB`) | `B03V07`, `MDM9x07_A-H_22_V1.10_190221` (a February 2019 build) |
| `ATI` | `Manufacturer: SIMCOM INCORPORATED`, `Model: SIMCOM_SIM7600A-H`, `Revision: SIM7600A-H_V1.1`, `SVN: 03`, the IMEI, `+GCAP: +CGSM` |
| Board | Waveshare SIM7600X 4G HAT (original, Raspberry Pi form factor), connected through its CP2102 USB-UART |
| Serial link | 9600 baud, `AT+IPREX?` = 9600 (permanent) |
| SIM / carrier | Ting (MVNO). The network reports `"Ting"`, LTE, on the T-Mobile US network |
| Power rail read (`AT+CBC`) | 3.883 V |
| PA temperature (`AT+CPATEMP`) | 25 °C |
| Test date | 2026-10-17 and 2026-10-18 |

The firmware is old (2019), which matters: a command in a newer SIMCom manual may not exist here, and the V3.00 manual (2021) is not a perfect match for this build.

## 3. Key findings

Ranked by how much they affect firmware.

1. **`AT+CLAC` is incomplete and the module has far more commands than documented.** It lists 723 names, 426 of which are not in the manual's text. It also **omits** commands that clearly work (`+CPIN`, `+CMGS`, `+CMGR`, `+CGMI`, `+COPS` and others), so it cannot be used as a full inventory. Sections 14 and 15.
2. **SMS storage is bigger in module memory.** `AT+CPMS="ME","ME","ME"` gives **255 slots**, against 30 on the SIM. The `"MT"` storage is listed as supported by `AT+CPMS=?` but is **rejected** with `ERROR`.
3. **`ATH` is configured to be ignored** (`AT+CVHU?` = 1, per the manual: "ATH ignored but OK given"). Use `AT+CHUP` to hang up. **Observed** in section 20.6: `ATH` returns `OK` but the call stays up.
4. **VoLTE is enabled** (`at+voltesetting?` = 1) and the module has an active `ims` PDP context. `at+voltesetting` is not in the SIMCom manual; it is documented only on the Waveshare wiki. Voice and SMS worked on this setup.
5. **Every command takes about 0.2 s before the first reply byte and about 0.41 s to finish** at 9600 baud, even for `AT`. Plan for a ceiling of about 2 to 3 simple commands per second.
6. **Error format depends on `AT+CMEE`.** `0` gives a bare `ERROR`, `1` gives numeric codes (`+CMS ERROR: 321`), `2` gives text (`+CMS ERROR: Invalid memory index`). Malformed arguments (`AT+CMGR=abc`) give a bare `ERROR` in every mode.
7. **A bad or unprovisioned line makes SMS stall for minutes** before failing with `+CMS ERROR: Unknown error`, and the module ignores `AT` during the stall. Seen earlier in this project, fixed by correcting the line on the carrier's side.
8. **The module keeps network time:** `AT+CCLK?` returns local time with a quarter-hour zone offset (observed `-24`, i.e. UTC-6), and `AT+CTZU?` = 1 (automatic time zone update on).
9. **GNSS is on by default** (`AT+CGPS?` = `1,1`) and answers `+CGPSINFO` and `+CGNSSINFO`, empty until it has a fix.
10. **A big hidden surface exists**: an HTTPS stack (`+CHTTPS…`), FTP and SMTP clients, IMS configuration (`+IMSREGDB`, `+IMSSMSCFG`, `+IMSSIPCFG`), diagnostics (`+CCED`, `+CSNINFO`, `+VZWRSRP`), temperature and voltage reads, and dangerous items (`+BOOTLDR`, `+SHCMD`, `+CSHELL`, `+CUSBADB`, `+CNVW`). Section 15.

## 4. Sources and what could not be obtained

| Source | Used for |
|---|---|
| SIMCom SIM7500_SIM7600 Series AT Command Manual V3.00 (512 pp) | Command definitions, value ranges, `<stat>` tables |
| SIMCom SIM7X00 UART Application Note V1.00 (2016) | UART, flow control, DTR sleep, RI behaviour, CMUX. Older than this module (written for SIM7600C/CE); treat as general guidance |
| SIMCom GNSS Application Note V2.00 (2020) | GNSS background |
| SIMCom TCP/IP Application Note V2.00 | IP command names |
| SIMCom SMS notes V2.00 and V1.00, SAT V1.00, FTP(S) V2.00, MQTT(S) V2.00 and ATC V1.01, SSL V2.00, SIM800 HTTP compatibility V1.00, USB audio V1.03 (second research pass, section 20) | SMS flow, SIM toolkit, and the command names of the optional stacks |
| Waveshare wiki for the SIM7600X HAT | HAT-specific commands, audio tuning, VoLTE enable, FAQ fixes |
| The module itself | Everything marked Observed |

**Could not obtain** (SIMCom lists them but their download buttons are JavaScript placeholders with no direct URL): the V3.00 notes for HTTP(S), TTS, USB audio, GNSS, SMS and FTP(S), UART V2.00 and TCP/IP V3.00. Older Waveshare-hosted copies of SMS, FTP(S), MQTT(S), SSL and USB audio were found in the second pass (section 20); an HTTP(S) or TTS note was not. Repeated searches found no AT manual newer than V3.00. See [reference/SOURCES.md](reference/SOURCES.md). The hidden HTTPS, FTP and SMTP commands in section 15 are therefore undocumented here beyond the shapes the module reports.

## 5. Serial interface behaviour

**Observed** unless marked.

| Behaviour | Result |
|---|---|
| Framing | Commands end with CR (`\r`); `\r\n` also works. Replies are `\r\n<text>\r\n`, finished by `\r\nOK\r\n` or `\r\nERROR\r\n` |
| Echo | On by default. The module echoes the command line (`AT` then a blank `\r\r\n`). `ATE0` turns it off (reply is just `OK`), `ATE1` restores it |
| Case | Case-insensitive: `at`, `At`, `AT` all give `OK` |
| Trailing spaces | Ignored (`AT   ` gives `OK`) |
| Unknown command | Bare `ERROR`, for `AT+X`, `AT+X=?` and `AT+X?` alike |
| Latency | First byte about **0.20 s** after the command, complete about **0.41 s**, for `AT` and `AT+CSQ` alike (15 samples, range 0.40 to 0.42 s) |
| Baud rate | 9600, fixed. `AT+IPR=?` and `AT+IPREX=?` both list 0, 300 up to 3686400 |
| Persistent vs temporary baud | `AT+IPREX` is **permanent**; `AT+IPR` is **temporary** (reverts at reboot). Documented in the manual; the module accepts both |
| Auto-baud | `AT+IPREX=0` enables it (documented in the UART note). Not tried |
| Character frame (`AT+ICF?`) | `3,3` |
| Flow control (`AT+IFC?`) | `0,0`, off. The UART note says `AT+IFC=2,2` enables RTS/CTS and is stored automatically (documented) |
| Multiplexer (`AT+CMUX?`) | `0,0,5,118,0,0,600`; `AT+CMUX=?` lists a basic-mode multiplexer. Documented: `AT+CMUX=0` creates 4 virtual ports, 2 of which carry AT |
| Sleep (`AT+CSCLK?`) | `0` (DTR sleep off) — and a **separate** `AT^CSCLK?` returns `1` |
| Result code style | Verbose text (`OK`). `ATV`/`ATQ`/`ATX` were not changed |
| URC prefix on registration | Registration URCs only appear if enabled with `AT+CREG=1`, `AT+CEREG=1` and so on |

### Boot output

After a power-up (and after a SIM swap) the module printed, in order: `RDY`, `+CPIN: READY`, `SMS DONE`, `PB DONE`. **Observed.** It then accepted `AT`. The module hardware design says UART is ready about 13 to 14 s after power-up (Documented). SMS and phonebook are only ready after `SMS DONE` and `PB DONE`.

### Error codes

`AT+CMEE=<0|1|2>` selects the error style. **Observed:**

| Test | `CMEE=0` | `CMEE=1` | `CMEE=2` |
|---|---|---|---|
| `AT+CMGR=999` (index out of range) | `ERROR` | `+CMS ERROR: 321` | `+CMS ERROR: Invalid memory index` |
| `AT+CMGD=999` | `ERROR` | `+CMS ERROR: 321` | `+CMS ERROR: Invalid memory index` |
| `AT+CNMI=9` | not tried | `+CMS ERROR: 303` | `+CMS ERROR: Operation not supported` |
| `AT+CSQ=5` (write to a read-only command) | not tried | not tried | `+CME ERROR: operation not allowed` |
| `AT+CMGR=-1`, `AT+CMGR=abc` | `ERROR` | `ERROR` | `ERROR` |
| `AT+CSCS="FOO"` | not tried | not tried | `ERROR` |
| `AT+CPIN=1111` on an unlocked SIM | `ERROR` | | |

Other `+CMS ERROR` text seen in this project: `Unknown error` (long stall, bad line). `AT+CACM?` and `AT+CAMM?` answered `+CME ERROR: SIM busy`. `AT+CMEE` was still `2` after the HAT was power-cycled, so it looks stored (Unverified: I did not isolate it).

## 6. Identity and information commands

| Command | Observed answer (identifiers removed) |
|---|---|
| `ATI` | `Manufacturer: SIMCOM INCORPORATED`, `Model: SIMCOM_SIM7600A-H`, `Revision: SIM7600A-H_V1.1`, `SVN: 03`, `IMEI: <15 digits>`, `+GCAP: +CGSM`, `OK` |
| `AT+GMI`, `AT+CGMI` | `SIMCOM INCORPORATED` |
| `AT+GMM`, `AT+CGMM` | `SIMCOM_SIM7600A-H` |
| `AT+GMR`, `AT+CGMR` | `+GMR: LE11B03SIM7600A-H` (CGMR prefixes `+CGMR:`) |
| `AT+CSUB` | `+CSUB: B03V07` then `+CSUB: MDM9x07_A-H_22_V1.10_190221` |
| `AT+GSN`, `AT+CGSN` | the 15-digit IMEI |
| `AT+SIMCOMATI` | `Manufacturer`, `Model`, `Revision`, build string, `QCN:`, `SVN`, `IMEI`, `MEID` lines (undocumented command) |
| `AT+CINQSN?` | a hardware serial string starting `MP` (undocumented; not reproduced here) |
| `AT+CIMI` | the 15-digit IMSI |
| `AT+CICCID`, `AT+CCID` | `+ICCID: <19 digits>` / `+CCID: <19 digits>` (both work; the ICCID passes the Luhn check as shown) |
| `AT+CNUM` | `+CNUM: "","<number>",129` once the SIM stored its number; empty on the previous SIM |
| `AT+CPAS=?` | `(0,3,4)` activity status values (execute form not run) |
| `AT+CBC` | `+CBC: 3.883V` (a supply voltage only, no charge-state fields) |
| `AT+CPATEMP` | `+CPATEMP: 25` (undocumented; Celsius) |
| `AT+CCPUTEMP` | execute form `ERROR`; `AT+CCPUTEMP=?` lists `(0-5)` so it needs an argument |
| `AT+CADC` | `ERROR`; `AT+CADC0=?` lists `(0,2)` and `AT+CADC2` is listed |
| `AT+CVAUXV?`, `AT+CVAUXS?` | `2850`, `1` (auxiliary supply voltage setting, range 1700 to 3050 mV) |

## 7. Network and registration

### Registration state

| Command | Observed | Notes |
|---|---|---|
| `AT+CREG?` / `AT+CGREG?` / `AT+CEREG?` | `0,1` each (home network) | Second number: 1 home, 2 searching, 5 roaming, 0 not registered, 3 denied, 4 unknown. `AT+CEREG=?` lists `(0-2)`, so `AT+CEREG=1` turns on `+CEREG:` URCs |
| Before the antenna was attached | `0,2` and `+COPS: 0` | Searching, no operator |
| No SIM inserted | `+CEREG: 0,4` | Seen when the SIM was out |
| `AT+COPS?` | `+COPS: 0,0,"Ting",7` | Mode 0 automatic, long operator name, access technology 7 = E-UTRAN (LTE). **`AT+COPS=?` was not run** (it scans networks for minutes) |
| `AT+CSQ` | `+CSQ: 19,99` | RSSI 19 is about -75 dBm (`-113 + 2*n`); BER 99 is unknown. Range per the manual: 0 is -113 dBm or less, 31 is -51 or more, 99 unknown |
| `AT+CESQ` | `+CESQ: 99,99,255,255,11,33` | Extended signal quality. The unused 2G/3G fields are 99/255; the LTE fields report RSRQ and RSRP indexes. `AT+CESQ=?` gives the ranges |
| `AT+CSQFMT?` | `1` | Signal format |
| `AT+CPSI?` | `+CPSI: LTE,Online,<mcc-mnc>,<tac>,<cell id>,<pcid>,EUTRAN-BAND<n>,<earfcn>,<ul bw>,<dl bw>,<rsrq×10>,<rsrp×10>,<rssi×10>,<snr>` | Observed on LTE bands 2 and 4 on different readings; RSRP about -104 to -110 dBm. Cell identifiers are not recorded here |
| `AT+CNSMOD?` | `+CNSMOD: 0,8` | System mode 8 is LTE |
| `AT+PSRAT?` | `+PSRAT: LTE FDD` | Undocumented |
| `AT+CIND?` | `+CIND: 0,3,1,0,0,0,1,0` | Indicator list: `battchg`, `signal`, `service`, `call`, `roam`, `smsfull`, `GPRS coverage`, `callsetup` (undocumented here). `AT+CMER=?` is `(0-3),(0),(0),(0-1),(0-1)` |
| `AT+CCINFO` | `+CCINFO: NOT IN GSM or CDMA` | Only reports on 2G/CDMA |
| `AT+CCED?`, `AT+CSNINFO?`, `AT+VZWRSRP?`, `AT+VZWRSRQ?` | Cell, neighbour and signal detail on LTE | Undocumented; the output contains cell IDs, so it is not reproduced. `+VZWRSRP`/`+VZWRSRQ` list the serving and neighbour cells' dBm and dB |
| `AT+MONI?` | read form returns nothing; `=?` returns `+MONI: 0,0` | |

### Network mode and bands

- `AT+CNMP?` returns `2` (automatic). `AT+CNMP=?` on this unit lists `(2,9,10,13,14,19,22,38,39,48,54,59,60,63,67)`. The manual's meanings: 2 automatic, 13 GSM only, 14 WCDMA only, 38 **LTE only**, 48 any but LTE, 39 GSM+WCDMA+LTE, 54 WCDMA+LTE, 19 GSM+WCDMA, and CDMA/EVDO/TDS variants (9, 10, 22, 59, 60, 63, 67) that this North American unit lists although it has no such radios. The Waveshare wiki uses 2, 13, 38 and 48.
- `AT+CNBP?` returns three hexadecimal band masks (2G/3G, LTE, and a third for other bands). The Waveshare FAQ writes sample `AT+CNBP=` values to restrict bands for speed tuning; **I did not change them**.
- `AT+NETMODE?` returns `2` and `AT+NETMODE=?` lists `(1,2)` (undocumented). `AT+WS46=?` lists `(12,22,25,28,29)` and `AT+WS46?` returns `25` (undocumented wireless-system selection, 25 is the 3GPP "all" value in the standard).
- `AT+CEMODE?` returns `1`; `=?` lists `(0-3)` (undocumented; the 3GPP UE mode of operation for EPS: 1 is "CS/PS mode 1", data-centric with voice preferred on CS fallback, as I read it, **Unverified**).
- `AT+CGSMS?` returns `1` (SMS over circuit-switched) and `=?` lists `(0-3)`. SMS still worked on LTE, because the module also has `AT+IMSSMSCFG` (section 9).

### Time

- `AT+CCLK?` returns `+CCLK: "yy/MM/dd,hh:mm:ss±zz"` with `zz` in quarter hours (observed `-24`, UTC-6 for a Colorado line). The time is set by the network.
- `AT+CTZU?` returns `1` (auto update on). `AT+CTZR?` returns `0`.
- `AT+CNTP?` returns an NTP server and timezone (`+CNTP: <server>,0`) preconfigured to a public server (not reproduced). `AT+CNTP=?` shows `255,(-47~48)`. Not used.

## 8. SIM and phonebook

| Command | Observed |
|---|---|
| `AT+CPIN?` | `+CPIN: READY` (no PIN). With no SIM: `+CME ERROR: SIM not inserted` (CMEE=2) |
| `AT+CPBS?` | `+CPBS: "SM",3,100` (SIM phonebook, 3 used of 100) |
| `AT+CPBS=?` | `("SM","DC","FD","LD","MC","ME","RC","EN","ON")` |
| `AT+CPBR=?` | `+CPBR: (1-100),40,14` on `SM` (index range, 40-character number, 14-character name) |
| `AT+CSPN?` | `+CSPN: "",0` (no service-provider name stored on this SIM) |
| `AT+CPOL`, `AT+COPN` | respond to `=?` (listing forms not run) |
| `AT+SIMSTATUS?` | Several `CARD` and `APP` lines describing the SIM and its applications, with identifiers (undocumented; not reproduced) |
| `AT^CARDMODE?` | `^CARDMODE: 2` (Huawei-compatible command) |
| `AT+UIMHOTSWAPON?` | `0`; `AT+UIMHOTSWAPLEVEL?` returns `1`. Hot-swap of the SIM is **off**. Swapping the SIM with power on is not safe; always power down |

Note: selecting `AT+CPBS="ON"` (own numbers) was used once to read `AT+CNUM`; it was restored to `"SM"`.

## 9. SMS

### Settings, defaults and ranges

| Command | Observed | Range or notes |
|---|---|---|
| `AT+CMGF?` | `1` (text mode) | `AT+CMGF=?` is `(0-1)` |
| `AT+CSCS?` | `"IRA"` | `AT+CSCS=?` is `("IRA","GSM","UCS2")` |
| `AT+CSMP?` | `17,,0,0` (validity period empty) | The Waveshare FAQ fix sets `17,167,0,0`. Reverted to `17,,0,0` after the SIM was swapped, so it is SIM or session specific (Unverified which) |
| `AT+CSCA?` | `+CSCA: "<carrier SMSC>",145` | The carrier's SMS centre, already set by the SIM |
| `AT+CSMS?` | `0,1,1,1` | Service 0; MT, MO and broadcast supported |
| `AT+CNMI?` | `2,1,0,0,0` | Notify new message by storage index (`+CMTI`). `AT+CNMI=?` is `(0,1,2),(0,1,2,3),(0,2),(0,1,2),(0,1)` |
| `AT+CSDH?` | `0` | Show text-mode headers. `1` adds length and type fields to listings |
| `AT+CMMS?` | `0` | More messages to send; `=?` is `(0-2)` (undocumented) |
| `AT+IMSSMSCFG?` | `1,0` | IMS SMS configuration (undocumented; meaning Unverified, but it is the path that lets SMS go over IMS on an LTE-only network) |
| `AT+CGSMS?` | `1` | MO SMS service; `=?` is `(0-3)` |
| `AT+CSAS`, `AT+CRES` | `=?` returns `0` | Save/restore SMS settings to the SIM (undocumented) |
| `AT+CMGSI`, `AT+CMGRMI` | `=?` returns `(2-5)` and `(4)` | Undocumented SMS variants; not run |

### Storage

`AT+CPMS?` returns `<mem1>,used,total,<mem2>,used,total,<mem3>,used,total` for read/delete, write/send and receive storage. `AT+CPMS=?` lists `("ME","MT","SM","SR")` for the first two and `("ME","SM")` for the third.

| Storage | Observed capacity |
|---|---|
| `"SM"` (SIM) | **30** slots (default) |
| `"ME"` (module memory) | **255** slots |
| `"MT"` | listed as supported but `AT+CPMS="MT","MT","MT"` returned `ERROR` |
| `"SR"` | not tried |

Switching storage with `AT+CPMS="ME","ME","ME"` worked and was restored to `"SM"`. Whether `ME` storage survives a power cycle and whether received messages go there when selected were not tested. What happens when storage is full is **unverified**. SMS indexes start at **0**.

### Reading

- `AT+CMGL="ALL"` lists messages as `+CMGL: <index>,"<stat>","<sender>","","yy/MM/dd,hh:mm:ss±zz"` followed by the body on the next line. With `AT+CSDH=1` the header gains `,<length>,<…>,"<sca>",<type>,<…>` fields. Observed on a carrier welcome message.
- `AT+CMGR=<index>` returns `+CMGR: "<stat>","<sender>","","<timestamp>"` then the body. With CSDH=1 the same extra fields are added.
- `<stat>` strings: `"REC UNREAD"`, `"REC READ"`, `"STO UNSENT"`, `"STO SENT"`, `"ALL"` (documented).
- **`AT+CMGL="ALL"` marked an unread message as `"REC READ"`.** The message first listed as `"REC UNREAD"` and later as `"REC READ"` after a listing with `"ALL"`. Whether the read mark came from the `CMGL` call or another read is not isolated.
- **Senders can be short codes**, not phone numbers: the carrier's welcome text came from a four-digit short code. A parser must accept short codes and alphanumeric senders.
- Deleting: `AT+CMGD=<index>[,<delflag>]`, with `delflag` 1 to 4 clearing groups (4 deletes everything including unread) (documented); the invalid index error is `+CMS ERROR: Invalid memory index`.

### Sending

Worked on a correctly provisioned line: `AT+CMGF=1`, `AT+CMGS="+1<number>"`, wait for the `> ` prompt, text, Ctrl-Z (`0x1A`), reply `+CMGS: <n>` then `OK` within seconds. On a mis-provisioned line the same command stalled for up to several minutes with the module ignoring `AT`, then returned `+CMS ERROR: Unknown error`. See [board-test-findings.md](board-test-findings.md).

## 10. Voice calls and audio

### Dialing, state and hang-up

| Command | Observed or documented |
|---|---|
| `ATD+<E.164>;` | Observed to start a call. `OK` returns immediately and means only "started". The `;` is mandatory for voice (documented) |
| `AT+CLCC` | Observed: with a call it lists `+CLCC: <id>,<dir>,<stat>,<mode>,<mpty>,"<number>",<type>`. Observed states in a real call: `2` dialing, then `3` alerting (ringing), then `0` active. Documented full list: 0 active, 1 held, 2 dialing, 3 alerting, 4 incoming, 5 waiting, 6 disconnect. With no call it returns just `OK` (observed). `AT+CLCC=1` enables automatic reports (supported: `AT+CLCC=?` is `(0-1)`; not tried) |
| `VOICE CALL: BEGIN` | Printed when the call **connects** (observed at the alerting-to-active change) |
| `VOICE CALL: END: <hhmmss>` | Printed when the call ends, with the duration (observed, including `000004`) |
| `NO CARRIER` | Printed when the far end ended the call (observed) |
| `AT+CHUP` | Observed to end a call (`VOICE CALL: END: <time>` then `OK`); with no call it returns just `OK`. Documented: may print several `VOICE CALL: END` lines and may take up to 120 s |
| `ATH` | **Not tested.** `AT+CVHU?` returns `1`, which the manual says makes `ATH` and DTR-drop ignored (with `OK` still returned). Use `AT+CHUP` |
| `ATA` | Observed: answers an incoming call in about 0.2 s (section 20.6) |
| `AT+CLIP=1` | `OK`; `AT+CLIP=?` is `(0-1)`. **`AT+CLIP?` returns `ERROR`** on this unit. The manual lists it as `NO_SAVE`, so send it after every boot |
| `AT+CRC?` | `0` (extended RING format off) |
| `AT+CVHU?` | `1` |
| `AT+CCWA?` | `+CCWA: 0` (call waiting off). `AT+CHLD=?` is `(0,1,1x,2,2x,3,4)` |
| `AT+CLIR=?` | `(0-2)`; `AT+CLIR?` errors |
| `AT+COLP=?` | `(0-1)` |
| `AT+CUSD?` | `0`; `AT+CUSD=?` is `(0-2)` (USSD notifications) |
| `AT+CSSN?` | `+CSSN: 0,0` |
| `AT+CCFC=?` | `(0,1,2,3,4,5)` (call forwarding); `AT+CLCK=?` lists facility locks (`"SC"`, `"AB"`, `"AO"` and others) |
| `AT+MORING?` | `0` |
| `AT+CDIP=?` | `(0-1)` (called-line ID presentation, undocumented here) |

### DTMF and tones

- `AT+VTS=<char>[,<duration>]` sends a tone during an active call; `AT+VTS=?` lists `(0-9,*,#,A,B,C,D)`. A quoted comma-separated string is allowed up to 29 characters (documented). **Tried on live calls**: accepted with `OK` and the far-end menu appeared to react (section 20.9).
- `AT+VTD?` returns `0` (default tone duration); `=?` is `(0-255)` in tenths of a second.
- Undocumented: `AT+DTMFCTL?` returns `1,1`; `AT+VOLTEDTMFCFG?` returns `200,4000`; `AT+CPTONETEST=?` shows `(1-100)(0-9,A-F,*,#)(50-100)`.

### Audio

| Command | Observed |
|---|---|
| `AT+CSDVC?` | `1` (handset, which on this HAT is the 3.5 mm jack). `AT+CSDVC=?` is `(0,1,3)`: 0 is closed (only after `AT+CODECCTL=1`), 1 handset, 3 speaker. `NO_SAVE` per the manual |
| `AT+CLVL?` | `4`; range `(0-5)`. **Saved** across reboots (manual), so do not write on every key press |
| `AT+CMUT?`, `AT+VMUTE?` | both `0`. `AT+CMUT` only works during a connected call and resets when calls end (documented) |
| `AT+CMICGAIN?` / `AT+COUTGAIN?` | `3` and `8`; both range `(0-8)` |
| `AT+CODECCTL?` | `+CCODECCTL: 0` (note the extra `C` in the reply) |
| `AT+CPCMREG?` | `0` (PCM interface registration off). `AT+CPCMMODE?` is `0,1`; `AT+CPCMBANDWIDTH?` is `0,1`. The PCM bus is not used when audio goes to the HAT's jack |
| `AT+CECM?` | `0` (echo cancellation mode); `AT+CECH?` errors on read |
| `AT+CACDBFN=?` | `(Handset_cal.acdb,Handset_dingxin.acdb,Handset_tianmai.acdb)`: three audio calibration files stored on the module |
| `AT+CCMXPLAYWAV=?` | `(1-2),(0-255)` (play a WAV file; the Waveshare wiki uses `"E:/rec.wav"`) |
| `AT+CREC=?` | `OK`; the Waveshare wiki records calls with `AT+CREC=3,"e:/rec.wav"` and `AT+CREC=0`. **Not tried** |
| `AT+PLAYRINGTONE?` | `+PLAYRINGTONE: 0, /system/ring/telephone_Ring.mp3` (a built-in ringtone path, undocumented) |
| `AT+CTTS=?` | `OK`; `AT+CTTS?` returns `+CTTS: 0`; `AT+CTTSPARAM?` returns `2,3,0,1,1` (text-to-speech exists, undocumented here, not tried) |
| `AT+CVMOD?` | `3`; `=?` is `(0-3)` (voice mode, undocumented) |
| Waveshare tuning | `AT^PWRCTL=0,1,3` at startup (TDD noise), `AT+CECM=1`, `AT+CECH=0x500` during a call. `AT^PWRCTL?` and `AT+CECH?` give `ERROR` on read, so they are write-only. Not tried |

### VoLTE

- `at+voltesetting?` returns `+VOLTESETTING: 1`; `=?` is `(0-1)`. (Command is absent from the SIMCom manual.)
- The Waveshare wiki enables VoLTE with `at+voltesetting=1` **and** an NV write, `at+cnv=/nv/item_files/modem/mmode/ue_usage_setting,1,01,1`. Not run: the setting was already `1`.
- PDP contexts: `AT+CGDCONT?` shows context 1 (data, APN set earlier), context 2 `ims` and context 3 `sos`. `AT+CGACT?` shows 1 and 2 active, 3 inactive. The `ims` bearer is up.
- IMS configuration reads (undocumented): `AT+IMSREGDB?` returns a list of numbers, `AT+IMSSMSCFG?` returns `1,0`, `AT+IMSSIPCFG?` returns three timer-like values `2000,16000,32000`. Meanings are **unverified**.
- Calls and SMS worked end to end with this configuration on the T-Mobile network through Ting.

## 11. Data and IP

PDP contexts and addressing, **observed**:

- `AT+CGATT?` returns `1` (attached). `AT+CGACT?`: contexts 1 and 2 active. `AT+CGPADDR` lists each context's address (a private-range IPv4 for context 1 and an IPv6 for the `ims` context; addresses not recorded). `AT+CGCONTRDP=1` returns the bearer's APN, address and DNS servers.
- The APN in context 1 is `hologram`, carried over from an earlier setup. Whether the Ting plan includes data, and Ting's correct APN, were **not checked**.
- `AT+CGEQOS?` is empty; `AT+CGEQOS=?` shows `(1-24,100-179),(0-9),…`.
- `AT+CGDCONT=?` allows contexts 1 to 24 and 100 to 179 with types `IP`, `IPV6`, `IPV4V6`.
- `AT+CSOCKSETPN?` returns `1` (the context used by the IP stack); `AT+CGSOCKCONT?` mirrors the PDP list; `AT+CSOCKAUTH?` has three entries.
- `AT$QCRMCALL` (NDIS dial-up, per the Waveshare wiki) exists but was excluded from probing.

Command families, with what the module reports:

| Family | Commands that responded |
|---|---|
| TCP/UDP sockets (documented in the TCP/IP note) | `AT+NETOPEN?` returns `+NETOPEN: 0` (network closed); `AT+CIPOPEN=?` shows `(0-9),("TCP","UDP")` and `AT+CIPOPEN?` lists ten connection slots; `AT+CIPCLOSE=?` is `(0-9)`; `AT+CIPSEND` was not probed; `+CIPMODE?` is `0`; `+CIPHEAD?` is `1`; `+CIPSRIP?` is `1`; `+CIPRXGET?` is `0`; `+CIPCCFG?` is `10,0,0,1,0,0,500`; `+CTCPKA?` is `0,120,10`; `+CTCPFIN?` is `0` |
| HTTP (documented in the manual) | `AT+HTTPACTION=?` shows `(0-3)` (GET, POST, HEAD, DELETE per the manual); `AT+HTTPREAD?` returns `+HTTPREAD: LEN,0`; `+HTTPINIT` and `+HTTPTERM` are execute-only (both read-only forms answer `ERROR`); `+HTTPPARA`, `+HTTPDATA`, `+HTTPHEAD` give empty replies to the read-only forms. Not exercised |
| Alternate HTTP stack (**undocumented**) | `+CHTTPACT=?` shows `"ADDRESS",(1-65535)` (a `+CHTTPACT` style request command); `+HTTPOPEN`, `+HTTPCMD`, `+HTTPRECV=?` shows `offset,length`; `+HTTPSTATUS?` returns `GET,0,0,0`; `+HTTPCLOSE`, `+HTTPAUTHKEY` |
| HTTPS (**undocumented**) | `+CHTTPSSTART`, `+CHTTPSSTOP`, `+CHTTPSOPSE=?` shows `"ADDRESS",(1-65535)[,(1-2)]`, `+CHTTPSCLSE`, `+CHTTPSSEND`, `+CHTTPSRECV?` returns `LEN,0`, `+CHTTPSSTATE`, `+CHTTPSHEAD?` returns `0`, `+CHTTPSSLCFG=?` shows `(0-9)`, `+CHTTPSDL?` returns `0` |
| SSL / TLS (documented plus extras) | `+CSSLCFG` (full list of `sslversion`, `authmode` and so on), `+CCHSET`, `+CCHMODE`, `+CCHOPEN`, `+CCERTVERIFY?` returns `0`, `+CSSLCA`, `+CSSLCERT`, `+CSSLKEY` (ten empty certificate slots 0 to 9), `+CGTP?` returns `1,1,,443,,443` (an undocumented setting listing port 443) |
| FTP / FTPS | `+CFTP…` commands are listed in `AT+CLAC`: `+CFTPSERV`, `+CFTPUN`, `+CFTPPW`, `+CFTPGETFILE`, `+CFTPPUTFILE`, `+CFTPGET`, `+CFTPPUT`, `+CFTPLIST`, `+CFTPMKD`, `+CFTPRMD`, `+CFTPDELE`, `+CFTPGETSIZE` and more. **Not probed** |
| SMTP e-mail (**undocumented**) | `+SMTPSRV`, `+SMTPAUTH`, `+SMTPFROM`, `+SMTPRCPT`, `+SMTPSUB`, `+SMTPBODY`, `+SMTPBCH`, `+SMTPFILE`, `+SMTPSEND`, `+SMTPSTOP`. **Not probed** |
| MQTT | The `+CMQTT…` family is documented (`+CMQTTSTART` is execute-only, both read-only forms answer `ERROR`); `+CMQTTCNCTTIMEOUT` and `+CMQTTPING` (undocumented) respond to `=?`; the connection commands were **not probed** |
| Other IP helpers | `+CPING=?` shows `IP address,(1,2),(1-100),(4-188),(1000-…)`; `+CDNSSRV`, `+CDNSGIP`; `+CIPFILTERSET`, `+CINICMPALLOW?` returns `0x00000000FFFFFFFF`; `+CUPURE?` returns `1`; `+CNETSTART?` returns `0`; `+NETACT?` returns `0`; `+CPPPACT?` returns `0` |
| Time | `+CNTP` (NTP sync, `AT+CNTP?` shows a preset server) |

Practical note for the AT library: the serial link, not the module, is the limit. At 9600 baud a data transfer gets about 1 KB per second. `AT+IPREX=<speed>` raises it permanently (documented, supported values to 3,686,400).

## 12. GNSS and location

| Command | Observed |
|---|---|
| `AT+CGPS?` | `+CGPS: 1,1` (GNSS on, standalone mode). It is on by default or left on from earlier use |
| `AT+CGPSINFO` | `+CGPSINFO: ,,,,,,,,` (no fix, nine empty fields) |
| `AT+CGNSSINFO` | `+CGNSSINFO: ,,,,,,,,,,,,,,,` (no fix, 16 empty fields) |
| `AT+CGPSINFOCFG?` | `+CGPSINFOCFG: 0,0` (no automatic reports) |
| `AT+CGPSNMEA?` | `+CGPSNMEA: 63` |
| `AT+CGPSMODE=?` / `?` | `(0-2)` / `2` |
| `AT+CGPSURL?`, `+CGPSSSL?`, `+CGPSXE?`, `+CGPSXD?` | assisted-GNSS settings (`"0"`, `1`, `0`, `0`) |
| `AT+CLBS=?` | `(1,2,3,4,9),(1-24,100-179),(-180.000000-180.000000),(-90.000000-90.000000),(0,1)` (cell-based location service; the Waveshare wiki uses `AT+CNETSTART` then `AT+CLBS=1` and `=2`) |

No GNSS antenna was attached, so no fix was expected; the output format is from the empty answers. The older XIAO sketch parsed `+CGNSSINFO` and worked with GNSS already on ([legacy/xiao_phone-notes.md](legacy/xiao_phone-notes.md)).

## 13. Hardware, power and file system

| Command | Observed |
|---|---|
| `AT+CBC` | `+CBC: 3.883V` |
| `AT+CPATEMP` | `+CPATEMP: 25` (PA temperature in °C; undocumented) |
| `AT+CPMUTEMP`, `AT+CCPUTEMP` | listed; `CCPUTEMP=?` shows `(0-5)` |
| `AT+CADC`, `AT+CADC0`, `AT+CADC2` | ADC read commands, listed; `CADC0=?` shows `(0,2)` |
| `AT+CVAUXV`, `AT+CVAUXS` | auxiliary voltage setting `2850` mV and state `1` |
| `AT+CFUN?` | `+CFUN: 1` (full functionality). `AT+CFUN` writes were not sent |
| `AT+CPSMS?` | `+CPSMS:0,,,"00011000","00001010"` (power saving mode off, timers preset); `AT+CEDRXS?` errors |
| `AT+CSCLK?` / `AT^CSCLK?` | `0` / `1` (see section 5) |
| `AT+CFGRIDTM?` | `60,120` (ring-indicator pulse timing in ms; `=?` shows `(10-6000),(20-6000)`) |
| `AT+CUARTSD`, `AT+CURCD`, `AT+CUARTLOG`, `AT+CUARTRM`, `AT+CCUART` | UART/RI helpers: `CCUART?` is `0`, `CUARTSD?` is `500`, `CURCD?` is `0`, `CUARTLOG?` is `0`, `CUARTRM?` is `0` |
| `AT+FSCD?` | `+FSCD: C:/` (current directory) |
| `AT+FSLS?` | `+FSLS: SUBDIRECTORIES:0,FILES:0` (empty; the Waveshare wiki references `D:` and `E:` drives) |
| `AT+CSDCARDISOK?` | `0` (no card) |
| `AT+CTISCLK=?` | `(0,1)` |

Ring indicator behaviour (UART note, documented, older module): RI goes low on a call until connected, about 120 ms on an SMS, 60 ms on a URC if `AT+CFGRI` enables it. The HAT's side header exposes RI.

## 14. Command inventory: what `AT+CLAC` and the manual say

| Item | Count |
|---|---|
| Commands `AT+CLAC` lists | **723** |
| Of those, not found in the AT manual or the TCP/IP note text | **426** |
| Commands in the manual's table of contents | 408 sections |
| Manual commands the module's `AT+CLAC` omits but that **respond** to a read-only form | 97 (`+CPIN`, `+CMGS`, `+CMGR`, `+CMGL`, `+COPS`, `+CNMI`, `+CPMS`, `+CHUP`, `+CLIP`, `+CGSN`, `+CIMI`, `+CNUM` and more) |
| Manual commands that give `ERROR` to both read-only forms (so absent, or execute-only) | 35, mostly Bluetooth (`+BT…`), eCall (`+CECALL…`) and Wi-Fi/MiFi (`+CW…`) names, plus a few execute-only ones such as `+CPMUTEMP` |
| Names I added from other SIMCom modules that responded | `+CDIP`, `+CIND`, `+CMER`, `+CMMS`, `+CPLS`, `+CVMOD`, `+CSDF`, `+CHSN` |

**Lesson:** `AT+CLAC` is a partial list of this module's commands, and the manual documents a sizeable subset of the real command set. An unknown name and an execute-only command both answer a bare `ERROR` to `=?` and `?`, so the absence of a reply is not proof a command is absent.

### Names excluded from probing (190)

Never sent in any form, because they exist and some are dangerous:
- **Reset and power:** `+CRESET`, `+CPOF`, `+CPOWD`, `+DMRESET`, `$QCPWRDN`, `$MYPOWEROFF`.
- **Bootloader and firmware:** `+BOOTLDR`, `+FOTA…`, `+COTA…`, `+CODOWNLOAD`, `+CFOTASWITCH`.
- **NV and factory memory:** `+CNV`, `+CNVR`, `+CNVW`, `+CNVRWE`, `+CQCNV`, `+CAPFS`, `+CBACKUPEFS2`, `+CRESTORECOUNT`, `+CDMPOWER`, `+DMSET`, `+DMCONFIGEXT`, `+DMSESSION`, `+FACTORYSESSION`.
- **Shells and debug ports:** `+SHCMD`, `+CSHELL`, `+CUSBADB`, `+CUSBMSS`, `+CUSBAUDIO`, `+CUSBPIDSWITCH`, `+CDEVPORT`, `+CPORTDELAY`, `+CEDUMP`, `+CMDLOG`.
- **Test and calibration:** `+CTEST`, `+CLOOPBACK`, `+CLEDITST`, `+CTAT`, `+CREDTEA`.
- **Sending and connecting:** `+CMGS`, `+CMGSEX`, `+CMSS`, `+CMGW…`, `+CIPSEND`, `+CHTTPSSEND`, `+SMTP…`, `+CFTP…`, `+HTTPCMD`, `+CCSEND`, `+CMPUB`, `+CMCONN`, `+SSLOPEN`.
- **SIM access:** `+CSIM`, `+CRSM`, `+CGLA`, `+CRLA`, `+SIMAUTH`, `+ISIMAUTH`, `+SSAPDU`, `+SCHO`, `+SCHC`.
- **Network side effects:** `+COPS` (a `=?` scan blocks for minutes), `+CNETSCAN`, `+CELLLOCK`, `+CFUN`, `+ENTERSLEEP`, `+NETDORM`.
- **Vendor families of unknown purpose:** `$QC…` (Qualcomm-style), `$MY…` (a customised set that includes `$MYBAND`, `$MYNET…`, `$MYFTP…`, `$MYPING`, `$MYGPSPOS`), `+AYLA…`, `+BT…`.

The full list is in [reference/at-probe/not_probed_for_safety.txt](reference/at-probe/not_probed_for_safety.txt). The `$`-prefixed names and the basic `AT&` and `ATx` commands were not probed at all.

## 15. Hidden and undocumented commands

Commands the module lists or answers that the manual V3.00 does not document. **Observed** means the module gave a real answer to a read-only form; none of these were written to.

### Useful for the phone

| Command | Observed | Why it matters |
|---|---|---|
| `+VOLTESETTING` | `?` returns `1`; `=?` `(0-1)` | VoLTE on/off. Documented by Waveshare, not SIMCom |
| `+IMSREGDB`, `+IMSSMSCFG`, `+IMSSIPCFG` | read returns numbers (`+IMSSMSCFG: 1,0`) | IMS/SMS-over-IMS settings; the likely control for LTE-only SMS |
| `+CIND`, `+CMER` | indicator list and values | Battery, signal, service, call, roaming, SMS full indicators in one read |
| `+CESQ`, `+CSQFMT` | extended LTE signal | RSRP/RSRQ without parsing `+CPSI` |
| `+CCED`, `+CSNINFO`, `+VZWRSRP`, `+VZWRSRQ`, `+CPSI` | cell and neighbour info | Diagnostics and signal bars; includes cell identifiers |
| `+CPATEMP`, `+CBC`, `+CVAUXV` | temperature, voltage | Health readout |
| `+SIMCOMATI`, `+CSUB`, `+CINQSN` | firmware and hardware serial | Identification |
| `+CPLS`, `+CMMS`, `+CDIP`, `+CSDF`, `+CVMOD`, `+CHSN` | settings with small ranges | Preferred operator list, more-messages-to-send, called-line presentation, date format, voice mode |
| `+PLAYRINGTONE`, `+CTTS`, `+CTTSPARAM`, `+CCMXPLAYWAV`, `+CREC`, `+CACDBFN` | audio playback, recording, text-to-speech, calibration files | On-module audio features (ringtone, TTS, call recording) |
| `+DTMFCTL`, `+VOLTEDTMFCFG`, `+CPTONETEST` | tone settings | DTMF handling under VoLTE |
| `+CEMODE`, `+PSRAT`, `+NETMODE`, `+WS46` | mode of operation and access technology | Network mode selection |

### Network and IP extras

`+CHTTPACT`, `+HTTPOPEN`, `+HTTPCMD`, `+HTTPRECV`, `+HTTPSTATUS`, `+CHTTPS…` (an HTTPS client: `START`, `STOP`, `OPSE`, `CLSE`, `SEND`, `RECV`, `STATE`, `HEAD`, `SSLCFG`, `DL`), `+CSSLCA`/`+CSSLCERT`/`+CSSLKEY`, `+CCERTVERIFY`, `+CGTP`, `+CGSOCKCONT`, `+CSOCKAUTH`, `+CIPACT`, `+CIPQSEND`, `+CIPSTAT`, `+CIPENSRXGET`, `+CTCPFIN`, `+CTCPKA`, `+CUPURE`, `+CINICMPALLOW`, `+CTEUTP`, `+CIPSWTMD`, `+CIPFILTERSET`, `+CDNSSRV`, `+NETACT`, `+CPPPACT`, `+CNETSTART`, `+CNETSTOP`, `+CNETIPADDR`, `+CWNETPREF`, `+CBDS`, `+CMQTTPING`, `+CMQTTCNCTTIMEOUT`, and the SMTP and FTP groups.

### Huawei/other-vendor compatible commands

`^SYSCONFIG?` returns `2,0,1,2`; `^CARDMODE?` returns `2`; `^MODE?` returns `0`; `^DSCI?` returns `0`; `^BODYSARON?` returns `0` with `^BODYSARGSM`, `^BODYSARWCDMA`, `^BODYSARLTE` (SAR power back-off tables); `^CURCREAD?` returns `0,0`; `*CNTI?` returns `0,NONE`; `^SYSINFO` and `^SPN` answer `ERROR` to both forms.

### Diagnostics and engineering

`+CCINFO`, `+MONI`, `+CGSI`, `+CGSETI`, `+CUSI`, `+CUSETI`, `+CRUPSI`, `+CRUSET`, `+CMSSN`, `+CLARFCN`, `+CLGCELL`, `+CLUARFCN`, `+CLUCELL`, `+CLEARFCN`, `+CLECELL`, `+CSURV…`, `+CSGSNR`, `+CMSCR`, `+CWRRCR`, `+CLTEREJTIMER`, `+CEXTPLMNBAND`, `+SLTEREJINFO`, `+SBARCELL`, `+SBASECFG`, `+SDBGINFO`, `+CNSVS…`. Most answer only some forms; none were written to.

### UART and platform

`+CCUART`, `+CUARTSD`, `+CURCD`, `+CUARTLOG`, `+CUARTRM`, `+CFGRIDTM`, `+CSDCARDISOK`, `+CTISCLK`, `+CPCMMODE`, `+SLEEPINFO` and friends; the file system (`+FSCD`, `+FSLS`, `+FSLST`, `+FSDEL`, `+CFTRANRX`) with `C:` as the default drive and 0 files.

### Dangerous commands that exist (listed by the module, not probed)

`+BOOTLDR`, `+SHCMD`, `+CSHELL`, `+CUSBADB`, `+CNV`, `+CNVR`, `+CNVW`, `+CNVRWE`, `+CQCNV`, `+CAPFS`, `+CRESET`, `+CPOF`, `+DMRESET`, `+FACTORYSESSION`, `+CTEST`, `+CEDUMP`. These can brick the module, change its identity or open a root shell. Do not call them casually, and never from library code.

## 16. Settings changed during testing

| Setting | Change | Restored? |
|---|---|---|
| `AT+CSMP` | `17,167,0,0` (Waveshare FAQ fix) | Read back as `17,,0,0` after the SIM swap |
| `AT+CSCS` | `"GSM"`, then `"IRA"` | Reads `"IRA"` |
| `AT+CMEE` | `2` for verbose errors (also toggled 0, 1, 2 during error tests) | Left at `2` (also the useful setting) |
| `AT+CPMS` | `"ME"`, `"MT"` (rejected), then back to `"SM"` | Yes, `"SM"` |
| `AT+CPBS` | `"ON"` for `AT+CNUM` | Yes, restored to `"SM"` |
| `AT+CSDH` | `1` for header tests | Yes, `0` |
| `AT+CLIP` | `1` | Left on (not saved across reboot) |
| `AT+CRC` | `1` (incoming call test) | Reset to `0` by the power cycle in 20.10 |
| `AT+CLVL` | `3`, then module default `4` was observed | Reads `4` |
| Echo (`ATE0`/`ATE1`) | toggled | Yes, echo on |

All other probing used `=?` and `?` forms or execute forms of read-only information commands. The module kept working throughout and was never reset or power-cycled by me during probing.

## 17. Differences between the documents and the module

| Topic | Documents say | Module does |
|---|---|---|
| `AT+CPMS="MT"` | Listed as supported by `AT+CPMS=?` | Rejected with `ERROR` |
| SMS capacity | Waveshare FAQ: "generally 50" on the SIM; the module shows 30 for `SM` | `SM` = 30, `ME` = 255 |
| `AT+CLIP?` | Manual defines a read command | `ERROR` on this unit |
| `ATH` | Normal hang-up | Configured to be ignored (`CVHU` = 1); use `AT+CHUP` |
| Default baud | UART note and hardware design: 115200 | 9600 on this unit (stored with `AT+IPREX`) |
| `AT+IPR` vs `AT+IPREX` | Older docs here said `AT+IPR` + `AT&W` | `AT+IPR` is temporary; `AT+IPREX` is permanent |
| `AT+CLAC` | Meant to list all commands | Omits many working commands |
| `AT+CNMP` values | Waveshare lists 2, 13, 38, 48 | This unit supports 15 values, including CDMA and TDS modes it has no radio for |
| `AT+CCINFO` | Reports cell info | Says `NOT IN GSM or CDMA` on LTE |
| VoLTE | SIMCom manual has no command | `at+voltesetting` exists and is `1` |
| HTTPS commands | AT manual chapter 13 (HTTPS) | The module also has a different `+CHTTPS…` family not in the manual; the manual's HTTP commands also respond |
| Response time | Not stated | 0.2 s to first byte for every command |
| `AT+CHUP` response time | Up to 120 s | Immediate with no call |

## 18. Recommendations for the AT library

These feed [at-library-plan.md](at-library-plan.md).

1. Use `AT+CHUP` for hang-up; treat `ATH` as unreliable.
2. Re-send the volatile settings at startup: `ATE0`, `AT+CMGF=1`, `AT+CSCS`, `AT+CSMP`, `AT+CLIP=1`, `AT+CRC=1`, `AT+CSDVC`, `AT+CLVL`. `AT+CPMS`, `AT+CNMI` and `AT+CTZU` are kept across power loss (section 20.10). `AT+CMEE` boots as `2`.
3. Do not read back `AT+CLIP?`; track the state in software.
4. Budget about 0.4 s per command; a status refresh of five commands costs 2 s of serial time.
5. Parse by line, with a state machine. `+CMGL` and `+CMGR` bodies sit on the next line, headers carry quarter-hour time zones, and senders may be short codes.
6. Support `ME` storage (255 slots) for SMS so the SIM's 30 slots do not fill; test the full-storage behaviour first.
7. Treat the long SMS stall as normal failure behaviour: give `AT+CMGS` a timeout of at least 120 s and allow no other commands while it runs.
8. Use `AT+CIND?` or `AT+CESQ` plus `AT+CREG?` for the status bar instead of the heavier `AT+CPSI?`.
9. For any data work, raise the serial rate with `AT+IPREX` first.
10. Keep dangerous commands (section 15) out of the library entirely; do not expose raw AT passthrough to user-facing code without a block list.

## 19. Still to test

Section 20.5 lists what the second pass left untested; the list below is the original one.

- Done in section 20.6: incoming calls (`+CRING`, `+CLIP`, `ATA`) and `ATH` against `AT+CHUP`.
- DTMF is covered in 20.9 (probably works). Still to test: `AT+CLCC=1` auto-reporting and call waiting.
- SMS: full-storage behaviour, `ME` storage persistence, receiving into `ME`, `AT+CMGD` flags, characters that differ between `IRA` and `GSM`, `UCS2`, long messages.
- Data: the Ting plan's APN and allowance, then `AT+CHTTPS…` and the manual's HTTP(S) commands. Raise the baud rate first.
- `AT+CLCC=1`, `AT+CEREG=1` and `AT+CREG=1` URC formats.
- GNSS with an antenna and a sky view.
- Power saving (`AT+CPSMS`, `AT+CSCLK`, DTR wake) and the RI pin timing.
- Done in 20.10: which settings survive a power cycle.
- The undocumented IMS commands (what the numbers in `+IMSREGDB` and `+IMSSIPCFG` mean).
- The newer SIMCom application notes (SMS, HTTP(S), FTP(S), MQTT(S), TTS, USB audio) once downloaded in a browser.

## 20. Second research pass: web search, extra names and value decoding

A follow-up pass looked for more commands on the web, probed the new names read-only, and decoded values the first pass left unexplained. Raw results for the names probed are in [reference/at-probe/probe2_extra_names.sanitized.json](reference/at-probe/probe2_extra_names.sanitized.json).

### 20.1 What the web search found

- No public list of hidden SIM7600 commands exists. Searches for the `+IMS...`, `+CHTTPS...`, `+CPATEMP` and `+VOLTESETTING` names found nothing beyond the Waveshare wiki's VoLTE entry. The hidden-command surface in section 15 stays documented only by what the module itself reports.
- A 2026 vulnerability entry (BDU:2026-00054) says a SIM7600G modem on firmware `LE20B03SIM7600M21-A` has an undocumented AT command that runs system commands as root. It names no command. On this unit the likely candidates are the commands `AT+CLAC` lists such as `+SHCMD`, `+CSHELL` and `+CUSBADB`, which I never sent. Do not expose raw AT passthrough on anything networked.
- SIMCom's older manuals (V1.01 to V1.12) are on mirrors; they add nothing for this firmware beyond what V3.00 covers.
- Waveshare-hosted SIMCom notes that were obtainable are now in `reference/` (see [SOURCES.md](reference/SOURCES.md)): SMS V2.00 and V1.00, SAT V1.00, FTP(S) V2.00, MQTT(S) V2.00 and ATC V1.01, SSL V2.00, SIM800 HTTP compatibility V1.00 and USB audio V1.03.
- The names in those notes, plus 318 names from the SIM7000 and A76xx manuals and the older SIM7600 V1.12 manual that are not in this project's index, gave 212 candidates after removing anything that opens, sends, connects, writes, deletes, powers or switches. Each got only `AT<cmd>=?` and `AT<cmd>?`. 40 gave a real answer, and most of those were standard 3GPP or SIMCom commands that the first pass had not tried (the table below). The other 172 answered `ERROR` to both forms, which does not prove they are absent (section 14).

### 20.2 Newly confirmed commands

| Command | Observed | Meaning / use |
|---|---|---|
| `AT+CCID` | `+CCID: <iccid>` | SIM card ID (same as `+CICCID`) |
| `AT+CGMM` / `AT+CGMR` | `SIMCOM_SIM7600A-H` / `+CGMR: LE11B03SIM7600A-H` | Model and firmware, execute form |
| `AT+CSUB` | `B03V07` and `MDM9x07_A-H_22_V1.10_190221` | Firmware build and baseband version |
| `AT+SIMCOMATI` | manufacturer, model, revision, build, `SVN: 03`, IMEI, `+GCAP: +CGSM`, `DeviceInfo: 173,170` | One-shot identity block |
| `AT+CBC` | `+CBC: 3.882V` | Module supply voltage (the HAT's rail, not a battery gauge) |
| `AT+CPATEMP` | `+CPATEMP: 28` | Power-amplifier temperature in degrees C (28 at idle on the bench) |
| `AT+CVAUXV?` | `+CVAUXV: 2850` | Auxiliary supply setting in millivolts |
| `AT+CCLK?` | `+CCLK: "yy/mm/dd,hh:mm:ss-zz"` | Network time, zone in quarter hours |
| `AT+CIND?` / `=?` | `+CIND: 0,3,1,0,0,0,1,0` | Indicators in order: `battchg` 0-5, `signal` 0-5, `service` 0-1, `call` 0-1, `roam` 0-1, `smsfull` 0-1, `GPRS coverage` 0-1, `callsetup` 0-3. Read: no battery, signal 3 of 5, in service, no call, home, SMS not full, packet coverage, no call setup |
| `AT+CMER?` | `0,0,0,0,0` | Indicator event reporting is off; set it for `+CIEV` URCs on change |
| `AT+CESQ` | `+CESQ: 99,99,255,255,4,24` | Fields: rxlev, ber, rscp, ecno (all 99 or 255 on LTE), RSRQ index 4, RSRP index 24. Index 24 is -116 dBm and matches `+VZWRSRP` (-116.2) |
| `AT+CSQFMT?` | `1` | Format selector for `+CSQ` (module accepts 0-1) |
| `AT+CEER` | `+CEER: ESM sync up with network` | Last extended error, readable without a call |
| `AT+CGPADDR=<cid>` | cid 1 `0.0.0.0`, cid 2 an IPv6 address | PDP address per context; 1-3 supported |
| `AT+CGCONTRDP=2` | `ims` APN, IPv6 address, gateways and DNS (P-CSCF) addresses | IMS bearer parameters (identifiers not recorded) |
| `AT+CGEQREQ?` / `AT+CGEQMIN?` | `2,2,0,0,0,0,2,0,"0E0","0E0",3,0,1,0,1` / empty | 3G QoS; irrelevant on LTE |
| `AT+CGEREP?` | `0,0` | Packet event reporting off |
| `AT+CGACT?` | `1,0` `2,1` `3,0` | Only the IMS context is active; the data context 1 is not (no data session) |
| `AT+CMUX?` | `0,0,5,118,0,0,600` | 27.010 multiplexer defaults; `=?` `(0),(0),(1-8),(1-1500),(0),(0),(2-1000)`. A way to get several logical channels over one UART, untested |
| `AT+CSVM?` | enabled, a voicemail number, 145 | Carrier-provisioned voicemail number (not recorded here). `AT+CPBR=1,3` shows the same entry as `"Voice Mail"` |
| `AT+CPBR=1,3` | `411`, `611` and a voicemail entry | The SIM's phonebook comes with three carrier entries; `=?` `(1-100),40,14` (100 slots, 40-char numbers, 14-char names) |
| `AT+CSTA?` | `129` | Default address type (unknown format) |
| `AT+CTZU?` / `AT+CTZR?` | `1` / `0` | Auto time-zone update on; zone change reporting off |
| `AT+CSCLK?` | `0` | Slow-clock sleep off (`=?` `(0-1)`) |
| `AT+CCWA?` | `0` | Call waiting off in the module's setting (`=?` `(0-1)`) |
| `AT+CRC?` | `0` | Extended `+CRING` format off; set 1 to see the call type |
| `AT+CSALPHA?` | `0` | Alpha-string handling for phonebook (0-1) |
| `AT+VTS=?` | `(0-9,*,#,A,B,C,D)` | DTMF tone characters; `AT+VTD?` is `0` (duration setting, `=?` `(0-255)`) |
| `AT+CHLD=?` | `(0,1,1x,2,2x,3,4)` | Hold, swap, conference and transfer actions are supported by the module (untested) |
| `AT+CTXVOL=?` / `AT+CRXVOL=?` | `(0x0000-0xFFFF)` | Transmit and receive digital gain; read forms give `ERROR` |
| `AT+CNSN=?` / `AT+CFNSMOD=?` | `(0x0000-0x7FFF)` / `(0x00FF,0x0073,0x00F3,0x01FF)` | Hex mask settings; meaning unknown |

### 20.3 Values decoded

| Item | Observed | Decoded |
|---|---|---|
| `AT+CNMP?` | `2`. `=?` `2,9,10,13,14,19,22,38,39,48,54,59,60,63,67` | Manual V3.00: 2 automatic, 13 GSM, 14 WCDMA, 38 LTE only, 39 GSM+WCDMA+LTE, 48 any but LTE, 54 WCDMA+LTE, 19 GSM+WCDMA, 9 CDMA, 10 EVDO, 22 CDMA+EVDO, 59 TDS-CDMA, 60 GSM+TDSCDMA, 63 GSM+WCDMA+TDSCDMA, 67 CDMA+EVDO+GSM+WCDMA+TDSCDMA. The value persists across resets, so a wrong write is permanent. Keep 2 |
| `AT+CNSMOD?` | `0,8` | `<n>=0` auto-report off, `<stat>` 8 is LTE (manual table: 0 none, 1 GSM, 2 GPRS, 3 EGPRS, 4 WCDMA, 5-7 HSPA types, 8 LTE) |
| `AT+PSRAT?` | `LTE FDD` | Packet radio access technology now in use |
| `AT+CPSI?` | `LTE,Online,310-260,<tac>,<cell>,<pci>,EUTRAN-BAND2,<earfcn>,4,4,-171,-1155,-797,8` | Field order from the manual: system mode, state, MCC-MNC, TAC, serving cell ID, PCI, band, EARFCN, DL bandwidth index, UL bandwidth index, RSRQ, RSRP, RSSI, RSSNR. On this firmware RSRQ, RSRP and RSSI are in **tenths** of dB/dBm (-17.1 dB, -115.5 dBm, -79.7 dBm). The manual's examples are GSM and WCDMA only, so the scaling is inferred by comparing with `+VZWRSRP` and `+CSNINFO` |
| `AT+CSNINFO?` | `SCELL, LTE,310-260,<tac>,<cell>,<pci>,EUTRAN-BAND2,<earfcn>,4,4,-18,-117,-80,8` then two `NCELL` lines | The same data in whole units, plus neighbour cells. Easier to parse than `+CPSI` |
| `AT+VZWRSRP?` / `AT+VZWRSRQ?` | `<pci>,<earfcn>,-116.20,...` / `...,-18.40,...` | Serving and neighbour RSRP and RSRQ as decimals, one line for both cells |
| `AT+WS46?` | `25`. `=?` `(12,22,25,28,29)` | 3GPP 27.007 meanings: 12 GSM, 22 UTRAN, 25 all 3GPP systems, 28 E-UTRAN only, 29 GSM+UTRAN. Not verified against SIMCom text |
| `AT+CEMODE?` | `1`. `=?` `(0-3)` | 3GPP 27.007 UE mode of operation: 0 PS mode 2, 1 CS/PS mode 1 (voice-centric), 2 CS/PS mode 2, 3 PS mode 1. 1 matches a phone that uses VoLTE. Not changed |
| `AT+NETMODE?` | `2` | Unknown. Not in V3.00 |
| `AT+CVMOD?` | `3`, `=?` `(0-3)` | Unknown. By the pattern on other SIMCom modules it is the voice mode preference (CS vs IMS), with 3 likely meaning IMS only. Not verified and not changed |
| `AT+CGSMS?` | `1`, `=?` `(0-3)` | 3GPP: which service carries MO SMS (0 PS, 1 CS, 2 PS preferred, 3 CS preferred). 1 is CS, yet SMS works on this LTE-only setup, so SMS is carried over IMS irrespective of this setting. Not changed |
| `AT+CSMS?` | `0,1,1,1` | Phase 2+ service 0; mobile-terminated, mobile-originated and broadcast all supported |
| `AT+CMVP?` | `255` | Message validity period setting; `=?` `(0-255)`. Meaning of 255 not verified |
| `AT+CSCB?` | `0,"",""` | Cell broadcast on (mode 0) with empty ID lists; `=?` `(0-1)` |
| `AT+CPMS?` | `"SM",2,30` (all three) | Two messages are stored on the SIM at the time of reading |
| `AT+CPSMS?` | `0,,,"00011000","00001010"` | PSM disabled, with the default TAU and active-time bit strings |
| `AT+CPLS?` | `0` with `=?` `0,1,2` | Preferred operator list selection (0 user, 1 operator, 2 HPLMN). `AT+CPOL?` answers `+CME ERROR: operation not allowed`, so the SIM has no list |
| `AT+CLCK="SC",2` | `+CLCK: 0` | Query form: SIM PIN lock is off |
| `AT^SYSCONFIG?` | `2,0,1,2` | Huawei-style fields mode, acquisition order, roaming, service domain. Community sources give 2 automatic, 0 automatic order, 1 roaming allowed, 2 circuit plus packet. Probably a compatibility stub; do not write to it |
| `AT^CARDMODE?` | `2` | Huawei convention: 0 no card, 1 SIM, 2 USIM |
| `AT^MODE?`, `AT^DSCI?`, `AT^CURCREAD?`, `AT*CNTI?` | `0`, `0`, `0,0`, `0,NONE` | Compatibility stubs. `AT*CNTI` `=?` is `(0-2)` and returns the technology name when queried with 0 on a registered network, but here it returned `NONE` |
| `AT+IMSREGDB?` | `272,17,1283,136,64,103,17,1280,136,64,512,17,0,0,0` | Unknown format. Looks like a table of registration values. Do not write it |
| `AT+IMSSMSCFG?` | `1,0` | Unknown. The first field is probably SMS-over-IMS enabled. Leave as is, since SMS works |
| `AT+IMSSIPCFG?` | `2000,16000,32000` | Unknown. Three numbers that look like SIP timers in milliseconds |
| `AT+DTMFCTL?` / `AT+VOLTEDTMFCFG?` | `1,1` / `200,4000` | DTMF handling flags and VoLTE DTMF timing (ranges `(1-400),(1-9999)`) |
| `AT+CHSN?` | `0,0,0,0` | Unknown. `=?` `(0-2,4,7),(0),(0),(0)` |
| `AT+STKFMT?` | `0` | SIM toolkit format is "decoded". `AT+STIN?`, `AT+STGI?`, `AT+STSM` answer `ERROR` until the SIM issues a proactive command |
| `AT+CCFC=0,2` and `AT+CCWA=1,2` | `ERROR` | The network-side supplementary-service queries (call forwarding and call waiting status) do not work through this module setup; likely they need XCAP over IMS, which the module does not provide |

### 20.4 What the notes add

- **SMS notes (V1.00, V2.00)** state that the `AT+CPMS` and `AT+CNMI` values "will be saved after the module restarts". That is not verified here (no restart). Re-send them at start-up anyway.
- They add commands the module accepts the read forms of: `AT+CMGMT` (change message status), `AT+CMGRD` (read and delete in one), `AT+CMSSEX` (send multiple from storage), `AT+CNMA` (acknowledge a new message, needed when `CNMI` mt is 2 with `+CSMS` 1). Their `=?` forms answer `OK` and nothing else. `AT+CMGP` is CDMA only and answers `ERROR`.
- They state SMS and call AT commands "conflict" and should not be interleaved. Observed behaviour agrees: a pending `AT+CMGS` blocks `ATD`.
- **SAT note:** the SIM toolkit commands are `AT+STIN`, `AT+STGI`, `AT+STGR`, `AT+STKFMT` and `AT+STSM` and `AT+STENV` (PDU format only). Menus, text and input arrive as UCS2 hex. The SIM would have to offer a toolkit menu; none was exercised.
- **FTP(S), MQTT(S), SSL and HTTP notes** define the command families `AT+CFTPS...`, `AT+CMQTT...` (the MQTT ATC V1.01 document spells one command `+CMQTTCONNNECT` with three N, a typo in that document), `AT+CSSLCFG` and the SIM800-compatible `AT+HTTPINIT`, `AT+HTTPPARA`, `AT+HTTPACTION`, `AT+HTTPREAD`, `AT+HTTPHEAD`. All of those names are in `AT+CLAC`, so the module has them, but no session was opened because nothing here has a data plan to test against. Treat their behaviour as documented only.
- **USB audio note:** `AT+CPCMREG` and `AT+CPCMFRM` (both read `0` and `0`, `=?` `(0-1)` for the latter) switch call audio to the module's USB or PCM path. Not relevant to the UART-and-headset setup.

### 20.5 What was not done

- Nothing destructive was sent. The 212 candidate names were screened with the same deny rule as the first pass, and only `=?` and `?` were issued to them, plus the execute forms of pure read commands listed in 20.2 and the two supplementary-service queries.
- Settings were not changed in the probing. The incoming-call test in 20.6 set `AT+CRC=1` (volatile).
- Needs a phone and a decision from the user, so not run (DTMF was later done, 20.9): a data session (the Ting APN is not known), a power cycle to check whether `AT+CPMS`, `AT+CNMI` and `AT+CMEE` persist, and the SIM toolkit menu.

### 20.6 Incoming calls and ATH (tested)

Two calls were placed to the Ting line from a cell phone with `AT+CLIP=1` and `AT+CRC=1` set. **Observed:**

| Step | Result |
|---|---|
| Ring | With `AT+CRC=1` each ring arrives as `+CRING: VOICE` (no plain `RING`), followed immediately by `+CLIP: "<number>",145,,,,0`. A second ring and a second `+CLIP` came about 6 s later |
| Caller ID | `+CLIP` carries the number in plain form with type 145 (international). It arrived on the first ring, so the library can show the caller before answering |
| `ATA` | Echoed, then `VOICE CALL: BEGIN` and `OK` within about 0.2 s. The call was connected |
| `AT+CLCC` while active | `+CLCC: 3,1,0,0,0,"<number>",145`: index 3, direction 1 (incoming), state 0 (active), voice, not multiparty. The index was 3, not 1, on this unit |
| `AT+CHUP` | Ended the call at once: `VOICE CALL: END: 000010` (duration `hhmmss`) then `OK`. After it `AT+CLCC` returns just `OK` |
| `ATH` | Replied `OK` after about 3 s but **did not hang up**: `AT+CLCC` afterwards still showed state 0. The call was only ended by the later `AT+CHUP` (total 16 s). This confirms `AT+CVHU` = 1 behaviour: `ATH` is acknowledged and ignored |
| Caller hangs up | Not tested in this pass; the first call was ended by the module. Expect `NO CARRIER` and `VOICE CALL: END` |

Library consequences: answer with `ATA`, end with `AT+CHUP` only, treat `+CLIP` as the incoming-call event, and expect a short pause before the `OK` for `ATH` if anyone sends it. Parse the `VOICE CALL: END: hhmmss` line for call duration.

### 20.7 DTMF (first attempt, no way to check)

During an answered incoming call from a cell phone, `AT+VTS` was sent with `AT+VTD?` at `0`:

| Command | Result |
|---|---|
| `AT+VTS=1,10` and `AT+VTS=2,10` | `OK`, after 1.03 s and 1.22 s, so the command blocks for the tone length (duration is in tenths of a second) |
| `AT+VTS="3,5,7,9"` and `AT+VTS="*,#"` | `OK` in 0.21 s each, i.e. immediately; the string form does not wait for the tones |
| `AT+VTS=Z` | `ERROR` |

The person on the far-end cell phone **heard no tones** for any of them. That does not show the module failed: a phone normally does not play received DTMF to its user, and on a VoLTE call DTMF may be carried as signalling events instead of in-band audio. The module has VoLTE DTMF settings (`AT+DTMFCTL?` returns `1,1`, `AT+VOLTEDTMFCFG?` returns `200,4000`). A real check needs a far end that reacts to digits, such as an automated menu. See 20.9 for the later result.

### 20.8 DTMF into an automated menu (second attempt, still inconclusive)

An outgoing call to a public toll-free sales line (on a Sunday, outside its listed hours) with `ATD+<number>;` connected in about 4 s (`AT+CLCC` state 2 dialing, then 0 active). A headset on the HAT's 3.5 mm jack played the normal greeting, so **received call audio works on outgoing calls too**. Then `AT+VTS=1,10` and `AT+VTS=2,10` each returned `OK`, and `AT+CHUP` ended the call at 25 s (`VOICE CALL: END: 000025`). The listener did not hear the digits locally (expected, the module does not play DTMF to its own earpiece) and did not report the menu reacting. The greeting may not have been a menu that accepts digits at that moment, so this proves nothing yet about DTMF delivery.

### 20.9 DTMF into an automated menu (third attempt, probably works)

An outgoing call to a public toll-free automated information line (listed by its operator as automated 24/7) connected in about 3 s. `AT+VTS=1,10` was sent 13 s after connect and `AT+VTS=0,10` 11 s later, each returning `OK`; `AT+CHUP` ended the call at 33 s. The listener, with a headset on the HAT's 3.5 mm jack, heard a menu and reported that the menu messages **seemed to change** after the digits, but heard no tone locally. That is **probably Observed**, not certain: it rests on one listener's impression, with no transcript. DTMF over the VoLTE call is accepted by the module (`OK`) and appears to reach the far end. The module does not play DTMF to its own earpiece, so a user interface that wants key-press feedback must make its own beep.

**Library consequences:** `AT+VTS=<digit>,<tenths of a second>` for single digits (the command waits for the tone to finish); the string form returns immediately; use `AT+VTD` only if you need a different default length.

### 20.10 Power-cycle persistence (tested)

Eleven settings were changed to non-default values, then the HAT's UART USB cable was unplugged for about 5 s and re-plugged. The module came back at 9600 baud, answered `AT` on the first try, and was registered (`CEREG 0,1`) with no extra steps. **Observed:**

| Setting | Test value | After power cycle |
|---|---|---|
| `AT+CPMS` | `"ME","ME","ME"` | **Kept** |
| `AT+CNMI` | `1,1,0,0,0` | **Kept** |
| `AT+CTZU` | `0` | **Kept** |
| `AT+CMEE` | `1` | Back to **`2`** (this unit boots with verbose errors on, not `0`) |
| `AT+CSMP` | `17,167,0,0` | Back to `17,,0,0` |
| `AT+CSCS` | `"GSM"` | Back to `"IRA"` |
| `AT+CRC` | `1` | Back to `0` |
| `AT+CLVL` | `2` | Back to `4` |
| `AT+CSDVC` | `3` | Back to `1` |
| `AT+CSDH` | `1` | Back to `0` |
| `AT+CCWA` | `1` | Back to `0` |

So **only the SMS storage, the new-message indication mode and the time-zone update flag are saved across a power loss**, matching the SMS note's wording for `CPMS` and `CNMI`. Everything else comes up at its default. The settings were restored afterwards (`CPMS` `"SM"`, `CNMI` `2,1,0,0,0`, `CTZU` `1`).

Other findings from this test:
- `AT+CCWA?` read `1` before the test although it had read `0` earlier. The only command sent in between was `AT+CCWA=1,2`, which answered `ERROR` but evidently turned call waiting on. Do not send the `=1,2` query form. After the restart it was `0` again.
- `AT+CLIP?` answers `ERROR` even after being set, so keep its state in software (as in section 18).
- Whether the PWR button was needed after the cable was re-plugged was not recorded.

**Library consequences:** the start-up sequence must send `AT+CMGF=1`, `AT+CSCS="IRA"` (or `"GSM"`), `AT+CLIP=1`, `AT+CRC=1` and `AT+CSDVC` on every boot, because they are not kept. `AT+CPMS` and `AT+CNMI` can be sent once but are cheap to resend. `AT+CMEE=2` is already the default.
