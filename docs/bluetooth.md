# Bluetooth on the SIM7600 HAT

Short answer: **the Waveshare SIM7600X HAT with the SIM7600A-H has no working Bluetooth.** The module's firmware carries a Bluetooth AT command set, but nothing on this board is a Bluetooth radio, and a hardware test on the real module (2026-10-23) showed the Bluetooth process refuses to start. Even if a radio were added, the AT command set offers only a serial port profile (SPP) and a BLE GATT server, so it could not drive wireless headphones.

Sources are marked **Observed** (sent to the module), **Documented** (SIMCom AT Command Manual V3.00 chapter 28, or the SIMCom hardware design) or **Inferred**.

## 1. What the hardware has

- **The HAT:** the Waveshare schematics in [reference/](reference/SOURCES.md) show the SIM7600 module, a USB-to-UART bridge, audio codec, SIM and antenna connectors. A text search of the (B) V2 and the older G-H HAT schematics for "Bluetooth", "BT" and "CSR" found nothing (**Documented**; it would miss a part drawn only as an image, and the (B) V1 schematic was not searched). The Waveshare wiki's feature list for the HAT, as read earlier in this project, does not mention Bluetooth.
- **The module:** the SIMCom hardware design for the SIM7600G-H and NA-H family describes Bluetooth only as an *option on spare pads*, meant to connect an external chip (**Documented**). The A-H is in the same family; its own hardware design was not obtainable, so this is **Inferred** for the A-H.

| Module pad | Default function | Bluetooth option |
|---|---|---|
| `SPI_MISO` (7) and `SPI_MOSI` (8) | SPI | "Optional: BT uart" |
| `USIM_DET` (53) | USIM card detect | `BT_EN` |
| `COEX3` (86) | Wi-Fi/LTE coexistence | `BT_PCM_CLK` |
| `GPIO41` (52) | GPIO | `BT_PCM_SYNC` |
| `GPIO77` (87) | GPIO | `BT_PCM_OUT` |
| `FLIGHTMODE` | Flight-mode input | `BT_PCM_IN` |

These pads carry a UART, an enable line and a PCM audio bus to an external Bluetooth controller. The AT manual describes `AT+BTPOWER` as starting the "bt csr app", which points at a CSR-brand controller. The same hardware design also describes an SDIO interface "for WLAN solution with W58 module", with a note that a special software version is needed. Both are options a product maker can wire up; this HAT does not.

## 2. What the module does (Observed, 2026-10-23)

Module: SIM7600A-H, firmware `LE11B03SIM7600A-H` (build `B03V07`). Only the read-only forms were sent first; then one power-on attempt.

| Command | Result |
|---|---|
| `AT+CLAC` | Lists 25 Bluetooth names: `+BT`, `+BTPOWER`, `+BTHOST`, `+BTPIN`, `+BTSETSCAN`, `+BTSCAN`, `+BTIOCAP`, `+BTPAIR`, `+BTUNPAIR`, `+BTPAIRED`, `+BTSPPSRV`, `+BTSPPPROF`, `+BTSPPCONN`, `+BTSPPSEND`, `+BTGATTREG`, `+BTGATTACT`, `+BTGATTCREDB`, `+BTGATTCRESRV`, `+BTGATTCRECHAR`, `+BTGATTCRECHARDES`, `+BTGATTSRVADD`, `+BTGATTREADCFM`, `+BTGATTWRCFM`, `+BTGATTNOTIFY`, `+BTGATTSENDIND` |
| `AT+BTPOWER=?` / `AT+BTPOWER?` | `+BTPOWER: (0-1)` / `+BTPOWER: 0` (stopped) |
| `AT+BTSCAN=?` | `+BTSCAN: (0-1),(0-1),(6-60)` |
| `AT+BTIOCAP=?` | `+BTIOCAP: (0-3)` |
| `AT+BTPAIR=?`, `AT+BTUNPAIR=?` | `+BTPAIR: (index)`, `+BTUNPAIR: (index)` |
| `AT+BTSPPSRV=?`, `AT+BTSPPCONN=?` | `+BTSPPSRV: (0-1)`, `+BTSPPCONN: (0-1)` |
| `AT+BTPIN=?`, `AT+BTSETSCAN=?` | `OK` with no parameter list. These two are not in the manual |
| `AT+BTPOWER=1` | `OK`, **but `AT+BTPOWER?` still reads `0`**: the Bluetooth process did not start |
| `AT+BTHOST?`, `AT+BTPAIRED?`, `AT+BTSCAN=1,0,6` (after the power-on attempt) | `ERROR` |
| `AT+BTPOWER=0` | `ERROR` (already off) |
| All other `+BTGATT...` and `+BTSPP...` names, read forms | `ERROR` |

The firmware therefore has the command parser and the Bluetooth stack code, and `AT+BTPOWER=1` is accepted, but with no Bluetooth controller behind it the radio never comes up. Nothing was left changed: `AT+BTPOWER?` is `0`, and the module kept answering `AT` normally afterwards.

## 3. Command reference (Documented, manual V3.00 chapter 28)

None of these can be used on this HAT. They are listed so the library plan can say precisely what was ruled out.

### 3.1 Power, name, scan, pairing

| Command | Forms | Purpose and parameters |
|---|---|---|
| `AT+BTPOWER` | `=?` `?` `=<flag>[,<debug_switch>]` | `<flag>` 0 stop, 1 start the Bluetooth application. `<debug_switch>` only 1, saves a log file; not allowed when stopping |
| `AT+BTHOST` | `?` `=<btname>` | Local Bluetooth name (max 64 chars) and MAC. Default name is `SIM7600_BT_` plus the last three MAC bytes. Read returns `+BTHOST: <name>, <mac>` |
| `AT+BTSCAN` | `=?` `=<doscan>[,<mode>[,<timeout>]]` | Scan for devices. `<doscan>` 1 scan, 0 stop; `<mode>` 0 show paired devices, 1 hide them; `<timeout>` 6 to 60 s, default 10. Results arrive as `+BTSCAN: <status>,<index>,<name>,<mac>,<rssi>`, ending with `+BTSCAN: 1` |
| `AT+BTIOCAP` | `=?` `=<mode>` | Pairing capability: 0 display only, 1 display and yes/no, 2 keyboard only, 3 none |
| `AT+BTPAIR` | `=?` `=0,<scan index>` / `=<mode>,<accept>[,<passkey>]` | Start pairing with a scanned device, or accept a request. Pairing triggers `+BTPAIRING: <mode>,<name>,<mac>[,<passkey>]`; reply with the accept form (mode 1 compare, 2 passkey, 3 rebond, 4 notify, 5 just works, 6 pin code). Result `+BTPAIR: <0 fail or 1 success>[,<name>,<mac>]`. Timeout about 30 s |
| `AT+BTUNPAIR` | `=?` `=<paired index>` | Remove a paired device. `+BTUNPAIR: <0 or 1>` |
| `AT+BTPAIRED` | `?` | List paired devices: `+BTPAIRED: <count>,<index>,<name>,<mac>` |
| `AT+BTPIN`, `AT+BTSETSCAN` | | In `AT+CLAC` only. Not in the manual. Purpose unknown (probably a pin code and a scan-visibility setting) |

### 3.2 Serial port profile (SPP)

| Command | Purpose |
|---|---|
| `AT+BTSPPSRV=<flag>` / `?` | Start (1) or stop (0) the SPP server. Read returns `+BTSPPSRV: <status>` |
| `AT+BTSPPPROF=<index>` | Ask whether paired device `<index>` (from `AT+BTPAIRED`) offers SPP: `+BTSPPPROF: <0 or 1>` |
| `AT+BTSPPCONN=<action>[,<paired index>]` / `?` | Connect (1) or disconnect (0). Result `+BTSPPCONN: <status>[,<max frame size>][,<mac>]`; the URC also appears when a remote device connects |
| `AT+BTSPPSEND=<data>` | Send data, **UCS2 hex** (for example `003100320033` is "123"). Result `+BTSPPSEND: <1 sent or 0 fail>` |
| `+BTSPPRECV: <len>,<data>` | Incoming data URC, UCS2 hex, 0 to 100 bytes |

### 3.3 BLE GATT server

The sequence in the manual is: register, activate, create database, add service, add characteristic, add the descriptor, add the database to the server. Each step answers `+<command>: <status>` (1 success).

| Command | Purpose |
|---|---|
| `AT+BTGATTREG=<0\|1>` | Unregister or register the GATT application |
| `AT+BTGATTACT[=<auto_broadcast>[,<preferredMTU>]]` | Activate. `<auto_broadcast>` 1 re-activates after a connection closes; MTU 24 to 512 (default 23) |
| `AT+BTGATTCREDB` | Create the database |
| `AT+BTGATTCRESRV=<uuid>` | Create a service (4 or 32 hex chars) |
| `AT+BTGATTCRECHAR=<uuid>,<property>,<permission>` | Create a characteristic; returns a handle |
| `AT+BTGATTCRECHARDES` | Create the characteristic descriptor |
| `AT+BTGATTSRVADD` | Add the database to the server |
| `AT+BTGATTREADCFM=<respCode>,<data>` | Answer a client read (`+BTGATTREADIND: <handle>,<maxlen>`) |
| `AT+BTGATTWRCFM=<result>` | Answer a client write (`+BTGATTWRIND: <handle>,<data hex>`) |
| `AT+BTGATTNOTIFY=<handle>,<data>` | Send a notification (up to 20 bytes) |
| `AT+BTGATTSENDIND=<handle>,<data>` | Send an indication (up to 20 bytes) |
| `+BTGATTCONN: <status>,<mac>` | URC: client connected (1) or disconnected (0) |

The GATT server role only: the module cannot be a BLE client (no scan-and-connect to a peripheral).

## 4. What this means for the project

- **Do not plan any feature on the module's Bluetooth.** No radio, so no AT command here will work. The library plan should not wrap these commands.
- **Headphones.** Even with a controller attached, the manual lists no A2DP (music), HFP/HSP (headset calls) or AVRCP commands, only SPP and a GATT server. The earlier answer about headphones stands: the HAT has no Bluetooth audio. Call audio leaves the HAT through its 3.5 mm jack, and the module's PCM pins.
- **The T-Deck.** Its ESP32-S3 has BLE only (no Bluetooth Classic), so it cannot be an A2DP source either. It can still do BLE for a phone app or a BLE keyboard, independent of the modem.
- **Options for wireless headphones** (not researched in depth, listed as starting points): put the HAT's headset jack into a small battery Bluetooth transmitter that supports an audio-in jack; or add a separate Bluetooth audio module that supports HFP and A2DP and feed it from the HAT's audio output; or use a wired headset. Each needs a hardware decision and was not tested.
- **Dangerous?** No. The Bluetooth commands are not on the list of commands to avoid; `AT+BTPOWER=1` simply failed to start anything.

## 5. Still unknown

- Whether the A-H variant's own hardware design differs from the G-H family (its document was not available).
- What `+BTPIN` and `+BTSETSCAN` do.
- Why `AT+BTPOWER=1` returns `OK` without starting; presumably the "csr app" exits at once when the controller does not answer.
