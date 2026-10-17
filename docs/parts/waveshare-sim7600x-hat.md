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
- Pads labelled USB3 (5V, DM, DP, GND), USB1 / USB2 connector footprints, "D-/D+" pogo pads near the jack. Per the V2 schematic these are all USB hub ports, not module pins; see [../at-interface-options.md](../at-interface-options.md) A2.
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

## What the (B) V2 schematic says

Source: `reference/Waveshare_SIM7600X_4G_HAT_B_V2_schematic.pdf` (from the Waveshare wiki).

- **USB:** FE1.1S hub. Upstream is micro-USB or the D+/D- pogo pads. Downstream: port 1 is the module's USB, ports 2/3 are the USB1/USB2 connector footprints, port 4 is the USB3 solder pads.
- **UART:** the module's UART pins (68, 71, 66, 67, 69, 70, 72) are not connected to anything on the board.
- **PWRKEY (pad 3):** tied to GND through R26 (0Ω), so the module powers on as soon as VBAT is present. To control power from the T-Deck, remove R26.
- **RESET (4), STATUS (49), VDD_EXT (44):** net-label stubs only, free.
- **Audio:** NAU8810 codec on the module's PCM pads (73-76), configured over the module's I2C (pads 55/56), outputs to the 3.5mm jack. Leave these pads alone if you keep the jack.
- **VDD_1V8 (pad 15)** feeds the board's 1.8V rail (codec and I2C pull-ups), so budget the 50mA limit when also using it for the level shifter.
- USB_VBUS (pad 11) is tied to the 5V rail.

## Still to verify on the physical board

- That R26 is populated (look for a 0Ω part near the PWRKEY pad; it is not visible in the photos).
- Continuity of USB1/USB2 edge pads (see at-interface-options.md A2).
- Pad 1 orientation and each pad number by continuity.

## Reference

- Waveshare wiki: https://www.waveshare.com/wiki/SIM7600G-H_4G_HAT
- Product page: https://www.waveshare.com/product/iot-communication/sim7600g-h-4g-hat-b.htm
- V2 (B) schematic: `reference/Waveshare_SIM7600X_4G_HAT_B_V2_schematic.pdf` (also V1 (B): `..._B_V1_schematic.pdf`)
- Older non-(B) HAT schematic (has CP2102 + TXB0108, not this board): `reference/Waveshare_SIM7600G-H_4G_HAT_schematic_V1.pdf`