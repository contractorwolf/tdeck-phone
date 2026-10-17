# Waveshare SIM7600X 4G HAT (B) V2

Identified from `images/PXL_20261017_063428644.jpg` (top) and `PXL_20261017_063437686.jpg` (bottom, silk: "SIM7600X 4G HAT (B) V2").

## What is on the board

Top side:
- SIMCom **SIM7600G-H** module (P/N S2-1097D-Z307J, FCC ID 2AJYU-8PYA007). LTE Cat-4 with global bands and GNSS.
- Three u.FL connectors: MAIN, AUX, GNSS.
- Micro-USB labelled USB HUB, and a large TO-263 buck regulator for the 5V to module supply (Waveshare lists an SPX29302 power chip for this board).
- The module's LCC edge pads run around all four sides of the module and are only partly routed out.

Bottom side:
- SIM card slot, 3.5mm audio jack with a NAU8810 audio codec (Waveshare spec for the B variant), 330uF bulk capacitor.
- Pads labelled USB3 (5V, DM, DP, GND), USB1 / USB2 connection points, "D-/D+" pads near the jack.
- 5V and GND pads (right of the SIM slot).
- NET and PWR LEDs.
- A 28-pin SSOP chip, probably the FE1.1S USB hub (Waveshare lists one).
- A QFN chip near the middle, probably the audio codec.

## Interfaces

The HAT exposes the module through USB (hub chip) and audio (jack). The module's own UART pins are **not** broken out on a header in these photos, which is why the plan is to dead-bug to the module pads. Module pad numbers are in [../sim7600-pins-to-find.md](../sim7600-pins-to-find.md).

## Power

- Input 5V (micro-USB or the 5V/GND pads). The regulator makes the 3.4-4.2V VBAT the module needs (3.4V min, 3.8V typ, 4.2V max; it must not sag below 3.4V during a transmit burst, per the hardware design power section).
- The module draws large current bursts when transmitting (see the current-consumption tables in the hardware design), so power the HAT from a real 5V supply or USB charger, not from the T-Deck.
- Share ground with the T-Deck.

## Things to verify before soldering

- Whether the module UART (pads 68, 71) is also connected on this board to the USB hub / a USB-serial chip or to a header. The older V1 HAT schematic (`reference/Waveshare_SIM7600G-H_4G_HAT_schematic_V1.pdf`) shows a CP2102 on UART TX/RX plus a NAU8810 on PCM; the V2 (B) board uses a hub instead, so check continuity from the pads.
- Whether PWRKEY (pad 3) is tied to a pulse circuit or to ground on this board, i.e. whether the module powers on by itself when 5V is applied.
- Whether the audio codec on PCM can be left in place while the PCM pads are not touched.

## Reference

- Waveshare wiki: https://www.waveshare.com/wiki/SIM7600G-H_4G_HAT
- Product page: https://www.waveshare.com/product/iot-communication/sim7600g-h-4g-hat-b.htm
- V1 schematic: `reference/Waveshare_SIM7600G-H_4G_HAT_schematic_V1.pdf` (a V2 schematic PDF was not downloadable from the wiki's file page, see SOURCES.md)
