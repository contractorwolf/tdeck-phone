# Bench test findings: old SIM7600A-H HAT

Tested 2026-10-17 from a Mac over the HAT's "USB TO UART" micro-USB (CP2102, appeared as `/dev/cu.usbserial-0001`), with the Ting SIM and the MAIN antenna attached. Commands came from pyserial in the repo venv. The module IMEI and the phone numbers used are deliberately not recorded here.

## Module and settings

| Item | Result |
|---|---|
| Module | SIMCOM SIM7600A-H, revision `SIM7600A-H_V1.1` |
| UART | **9600 baud** (`AT+IPR?` returns 9600, fixed). No reply at 115200. Matches the legacy XIAO sketch, see [legacy/xiao_phone-notes.md](legacy/xiao_phone-notes.md) |
| Flow control | `AT+IFC?` 0,0 (off) |
| SMS format | `AT+CMGF?` 1 (text mode already set) |
| Audio device | `AT+CSDVC?` 1 (handset) |
| GNSS | `AT+CGPS?` 1,1 (already on) |
| VoLTE | `at+voltesetting?` returns 1 (enabled). The SIMCom AT manual has no such command; Waveshare's wiki documents it |
| SIM | `+CPIN: READY`, `AT+CNUM` empty (normal) |
| PDP contexts | 1 `hologram` (active), 2 `ims` (active), 3 `sos` |
| Hardware | Needs the antenna to register. Before it was attached: `CREG 0,2`, no operator |

## Network

With the antenna on: registered (`CREG`/`CEREG` 0,1), operator `"Ting"` over LTE, MCC-MNC 310-260 (T-Mobile US), band 2, `CSQ` 20, RSRP about -106 dBm. The SMSC read back as T-Mobile's.

## Result: SMS and voice both work (2026-10-17, later)

After the SIM's line was fixed on the Ting side (below), with the same module and settings:

- **Registered** on Ting over LTE, `AT+CNUM` now returns the line's number (it was empty before).
- **Voice:** `ATD+<E.164>;` now rings the far end (`AT+CLCC` state 2 dialing, state 3 alerting, state 0 active) and call audio was heard on a headset in the HAT's 3.5mm jack.
- **SMS:** `AT+CMGF=1`, `AT+CMGS="+1<number>"`, text, Ctrl-Z returned `+CMGS: <n>` and `OK` within seconds, and the text arrived. No `CSMP`/`CSCS` changes were needed (`AT+CSMP?` stayed `17,,0,0`).

## What was wrong before

Voice reached a carrier menu ("Boost or Ting?") instead of the phone, and every SMS attempt failed with `+CMS ERROR: Unknown error` (in several number formats), after the module sat unresponsive for up to a few minutes. The module settings matched the Waveshare wiki throughout. The likely cause was the line on the Ting account: it had been registered with a wrong IMEI that could not be edited (not proven; it worked once the line was redone). Re-running Ting's device setup with the correct IMEI (Ting's checker accepted the module as compatible), the original SIM's ICCID and "Move a number" fixed it.

## Ting account notes

- Device setup is at `https://tingmobile.com/accounts/device-settings` then Activate a device: IMEI check, SIM type (physical, or "I already have a Ting SIM" with the ICCID), coverage (address), number.
- The IMEI and ICCID checks are plain page loads with the value in the URL, not API calls, so there is nothing to watch on the network.
- "Continue to billing" on the number step **placed the order** and showed a confirmation page; there was no separate billing screen. Treat that button as final.
- The number step has a hidden "Get a new number" area-code dropdown (default 720) and a "Keep my number" phone field. Neither applies to "Move a number".
- The 19-digit ICCID must be typed as text. Passing it through a numeric form-fill rounded it (`...75717` became `...76000`) and Ting rejected it with "ICCID Validation failed". The 19-digit value from `AT+CICCID` passes the Luhn check as is.
- A SIM from another source can fail Ting's ICCID check even though its ICCID and IMSI look like the same issuer.

## Notes for the firmware

- Handle a long `AT+CMGS` wait: with a bad line the module can be unresponsive for 2+ minutes, so use a timeout of at least 120 s and do not send other commands while waiting.
- A pending SMS send blocks other commands, including `ATD`.
- Dial with `ATD+<E.164>;` and hang up with `AT+CHUP`; poll `AT+CLCC` for state (3 is ringing, 0 is active). A `NO CARRIER` after a short call means the far end ended it.
- `AT+CNUM` is empty on some lines; do not rely on it.
- Follow Waveshare's wiki for this HAT (https://www.waveshare.com/wiki/SIM7600X_4G_%26_LTE_Cat-1_HAT): it documents the `CSMP`/`CSCS` SMS fix, the VoLTE commands (`at+voltesetting`), `AT+CHUP` and the audio tuning commands.

## Next

- Wire the T-Deck Grove UART (GPIO43/44) to the HAT side header at 9600 baud (checklist Phase 3 and 4).
- Test receive: call and text the line from another phone and watch for `RING`, `+CLIP` and `+CMTI`.
