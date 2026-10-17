# T-Deck Cellphone

Goal: a handheld phone. A LilyGO T-Deck running custom firmware talks over a dead-bug serial connection directly to the SIM7600G-H module on a Waveshare 4G HAT, for SMS and voice calls.

## Parts

| # | Part | Role | Notes |
|---|---|---|---|
| 1 | LilyGO T-Deck (ESP32-S3-WROOM-1, 2.8" LCD, T-Keyboard, SX1262 LoRa) | UI, keyboard, firmware host, AT-command master | 3.3V logic. See [parts/t-deck.md](parts/t-deck.md) |
| 2 | Waveshare SIM7600X 4G HAT (B) V2 with SIM7600G-H module | Cellular modem, SIM slot, antennas, power regulation | Module logic is 1.8V. See [parts/waveshare-sim7600x-hat.md](parts/waveshare-sim7600x-hat.md) |

Photos are in `../images/` (front/back of the T-Deck, top/bottom of the HAT).

## Docs

- [parts/t-deck.md](parts/t-deck.md): what the T-Deck is, the UART header, GPIOs in use.
- [parts/waveshare-sim7600x-hat.md](parts/waveshare-sim7600x-hat.md): what the HAT is and what it does and does not break out.
- [sim7600-pins-to-find.md](sim7600-pins-to-find.md): the module pads to dead-bug, with pad numbers and page citations.
- [at-interface-options.md](at-interface-options.md): every way to send/receive AT commands, using the HAT's own interfaces and the module edge pads, plus a bring-up order.
- [bring-up-checklist.md](bring-up-checklist.md): step-by-step order of work with pass conditions.
- [level-shifting.md](level-shifting.md): the 3.3V to/from 1.8V circuit, PWRKEY/RESET drivers, power and grounding.
- [reference/SOURCES.md](reference/SOURCES.md): downloaded datasheets and where they came from.

## Key findings so far

- The module UART is 1.8V. The T-Deck is 3.3V. T-Deck TX into the module can use a resistor divider, but module TX into the T-Deck **cannot** (1.8V is below the ESP32-S3 input-high threshold of 2.475V), so that direction needs a real level shifter.
- The module can supply 1.8V (VDD_1V8, pad 15, 50mA max), which is the reference for the shifter.
- The T-Deck's 4-pin Grove header (RX/TX/VCC/GND) is GPIO43 (TX) and GPIO44 (RX) per LilyGO's `utilities.h`, where they are named as the GPS pins.
- The HAT has its own NAU8810 audio codec and 3.5mm jack on the module's PCM bus, so voice calls can use the jack with no extra wiring.

## Open questions

- Confirm GPIO43/44 on this board revision against the schematic (a LilyGO forum user reported pin numbers from one schematic version not working).
- The HAT's module UART may also be wired to an onboard USB-serial bridge or header (the older HAT schematic shows one). Check the V2 (B) board for a path that would fight the dead-bug wires, and use a continuity check on the pads before soldering.
- Final voice audio path: HAT jack (easy) vs PCM to the T-Deck codec (harder; needs I2S/PCM work).
- Photo-identified pad positions on the module: the LCC pads are on the underside edges, so pad 1 orientation must be confirmed against the hardware design drawing before probing.
- SMS and audio application notes did not download (see SOURCES.md). The AT manual covers the commands needed.
