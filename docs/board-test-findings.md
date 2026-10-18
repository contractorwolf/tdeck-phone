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

## Voice

`ATD+<E.164 number>;` works. The call goes dialing (state 2) to active (state 0) within about 3 s, with `VOICE CALL: BEGIN`, and `AT+CHUP` ends it (`VOICE CALL: END: <duration>`). Done twice to the owner's cell phone; both times a carrier voice menu answered ("calling for Boost or Ting? press 2 for Ting") rather than the phone ringing. That confirms the SIM, IMS and VoLTE voice path work over LTE, but not yet audio to a person. Not understood yet: possibly number routing for the destination.

## SMS: failing

Sequence used, which matches the Waveshare wiki: `AT+CMGF=1`, `AT+CMGS="+1<number>"`, wait for `>`, text, Ctrl-Z. The module gives the `>` prompt, then after a delay of up to a few minutes (during which it ignores AT) returns `+CMS ERROR: Unknown error`. The Waveshare FAQ fix (`AT+CSCS="IRA"`, `AT+CSMP=17,167,0,0`) was applied and did not change the result. SMSC is `+12063130004`.

Reading: configuration matches the vendor docs and voice works over IMS, so the failure looks specific to SMS on this SIM, plan or IMEI. Not yet known.

## Next

- Call and text the SIM from the cell phone to test the receive side (`RING`, `+CLIP`, `+CMTI`).
- Ask Ting whether SMS and voice are provisioned for this SIM and IMEI.
- Try a second destination number, ideally on another carrier.

## Notes for the firmware

- Handle a long `AT+CMGS` wait: the module can be unresponsive for 2+ minutes. Use a timeout of at least 120 s, and do not send other commands while waiting.
- Dial with `ATD+<E.164>;` and hang up with `AT+CHUP`; poll `AT+CLCC` for state.
- Follow Waveshare's wiki for this HAT (https://www.waveshare.com/wiki/SIM7600X_4G_%26_LTE_Cat-1_HAT): it documents the `CSMP`/`CSCS` SMS fix, the VoLTE commands, `AT+CHUP` and the audio tuning commands.
