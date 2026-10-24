# T-Deck Cellphone

Goal: a handheld phone. A LilyGO T-Deck running custom firmware talks over a UART to a Waveshare SIM7600 4G HAT, for SMS and voice calls. The primary modem is the **older Waveshare SIM7600X HAT with a SIM7600A-H**, which already placed calls with the user's Ting SIM and has a 3.3V UART header. The Waveshare (B) V2 with a SIM7600G-H (dead-bug plan) is the fallback.

## Status

Done:
- Parts identified from photos (T-Deck, both Waveshare HATs); datasheets, schematics and AT manual downloaded to `reference/`.
- Chose the old SIM7600A-H HAT as the modem. Its 3.3V header UART means no level shifting or dead-bugging. The (B) V2 / SIM7600G-H route is documented as a fallback.
- Bench-tested the A-H from a Mac over the CP2102 USB at 9600 baud: identified the module, settings and network, registered on Ting over LTE.
- Voice calls work (ringing, audio on the HAT's headset jack) and SMS sends and arrives, after fixing the line on the Ting account. See [board-test-findings.md](board-test-findings.md).
- Read the legacy XIAO sketch for what it shows about the hardware ([legacy/xiao_phone-notes.md](legacy/xiao_phone-notes.md)).
- Bring-up checklist written for the old board.

Next: see [next-steps.md](next-steps.md).

## Parts

| # | Part | Role | Notes |
|---|---|---|---|
| 1 | LilyGO T-Deck (ESP32-S3-WROOM-1, 2.8" LCD, T-Keyboard, SX1262 LoRa) | UI, keyboard, firmware host, AT-command master | 3.3V logic. See [parts/t-deck.md](parts/t-deck.md) |
| 2 | **Primary:** Waveshare SIM7600X 4G HAT (original) with SIM7600A-H | Cellular modem, SIM slot, antennas, 3.3V UART header | Proven for voice on Ting. See [parts/waveshare-sim7600x-hat-original.md](parts/waveshare-sim7600x-hat-original.md) |
| 3 | **Fallback:** Waveshare SIM7600X 4G HAT (B) V2 with SIM7600G-H | Same role, but no UART header; module logic is 1.8V | See [parts/waveshare-sim7600x-hat.md](parts/waveshare-sim7600x-hat.md) |

Photos are in `../images/` (T-Deck front/back, (B) V2 top/bottom, `old-waveshare.jpg`).

## Docs

- [parts/t-deck.md](parts/t-deck.md): the T-Deck, its UART header, GPIOs in use.
- [parts/waveshare-sim7600x-hat-original.md](parts/waveshare-sim7600x-hat-original.md): the primary modem and how to wire it to the T-Deck.
- [board-test-findings.md](board-test-findings.md): bench results on the A-H HAT (9600 baud, registered on Ting, SMS and voice both work).
- [ting-and-voice.md](ting-and-voice.md): why the A-H is chosen, the Ting/VoLTE caveats.
- [next-steps.md](next-steps.md): what is left to do, in order.
- [ui-design-options.md](ui-design-options.md): options and approach for the T-Deck UI (lists of conversations, contacts, files), with linked example screenshots.
- [bluetooth.md](bluetooth.md): the HAT has no usable Bluetooth; the module's `AT+BT…` command set, what it did on the real module, and what that means for headphones.
- [sim7600a-h-at-reference.md](sim7600a-h-at-reference.md): what the module actually does (observed), compared with the SIMCom manual and Waveshare wiki; hidden commands; findings for the library. Companion: [sim7600a-h-at-command-index.md](sim7600a-h-at-command-index.md) (896 command names) and [reference/at-probe/](reference/at-probe/) (sanitized raw results).
- [at-library-plan.md](at-library-plan.md): plan for the AT-command library (SMS and calls first, data as a stretch).
- [bring-up-checklist.md](bring-up-checklist.md): step-by-step order of work with pass conditions.
- [legacy/xiao_phone-notes.md](legacy/xiao_phone-notes.md): what the old XIAO sketch (`legacy/xiao_phone.ino`) shows about this modem: 9600 baud, call/SMS/GPS commands.
- [at-interface-options.md](at-interface-options.md): every way to send/receive AT commands, and the AT command lists.
- Fallback (B) V2 route: [parts/waveshare-sim7600x-hat.md](parts/waveshare-sim7600x-hat.md), [sim7600-pins-to-find.md](sim7600-pins-to-find.md), [level-shifting.md](level-shifting.md).
- [reference/SOURCES.md](reference/SOURCES.md): downloaded datasheets and where they came from.

## Key findings so far

- The A-H HAT's header UART is 3.3V (level shifting done on the board), so the T-Deck Grove UART connects directly: no shifter and no soldering to the module.
- The T-Deck's 4-pin Grove header is GPIO43 (TX) and GPIO44 (RX) per LilyGO's `utilities.h`.
- For the fallback: the (B) V2 module UART is 1.8V and unconnected, PWRKEY is tied low via R26, and a divider plus a real shifter would be needed (see level-shifting.md).
- Voice on US carriers needs VoLTE; the A-H is proven on Ting, the G-H is not.

## Open questions

- Receive side not yet tested (`RING`, `+CLIP`, `+CMTI`).
- Which UART JMP position did the earlier setup use? (The baud is 9600.)
- PWR header pin polarity and whether it is safe to drive from a 3.3V GPIO.
- Confirm GPIO43/44 on this T-Deck revision against the schematic.
- Voice audio path: the HAT's jack (easy) vs PCM to the T-Deck codec (harder).
