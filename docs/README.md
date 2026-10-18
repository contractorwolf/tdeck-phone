# T-Deck Cellphone

Goal: a handheld phone. A LilyGO T-Deck running custom firmware talks over a UART to a Waveshare SIM7600 4G HAT, for SMS and voice calls. The primary modem is the **older Waveshare SIM7600X HAT with a SIM7600A-H**, which already placed calls with the user's Ting SIM and has a 3.3V UART header. The Waveshare (B) V2 with a SIM7600G-H (dead-bug plan) is the fallback.

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
- [board-test-findings.md](board-test-findings.md): bench results on the A-H HAT (9600 baud, registered on Ting, voice works, SMS failing).
- [ting-and-voice.md](ting-and-voice.md): why the A-H is chosen, the Ting/VoLTE caveats.
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

- Why does SMS fail with `+CMS ERROR: Unknown error` on this SIM? (Voice works; see board-test-findings.md.)
- Which UART JMP position did the earlier setup use? (The baud is 9600.)
- PWR header pin polarity and whether it is safe to drive from a 3.3V GPIO.
- Confirm GPIO43/44 on this T-Deck revision against the schematic.
- Voice audio path: the HAT's jack (easy) vs PCM to the T-Deck codec (harder).
