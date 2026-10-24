# Waveshare SIM7600X 4G HAT (SIM7600A-H): the primary modem

Identified from `images/old-waveshare.jpg`. This is the **modem the project now uses**. It is a different board from the (B) V2 in [waveshare-sim7600x-hat.md](waveshare-sim7600x-hat.md), which is kept as a fallback. It already placed voice calls with the same Ting SIM over a UART from a microcontroller, so the carrier, SIM and voice path are proven on this hardware.

## What the photo shows

- Waveshare "SIM7600X 4G HAT" (original Raspberry Pi form factor, 40-pin header) carrying a **SIM7600A-H** module (North American variant; the P/N reads approximately S2-107BR-Z1W6U from a rotated label).
- MAIN, AUX and GNSS u.FL connectors, PWR and NET LEDs, a **PWRKEY push button**, a BOOT pad pair.
- **USB TO UART** micro-USB (CP2102 bridge) and a second **USB** micro-USB.
- A **UART JMP** block (rows A, B, C; positions CP2102 / PI / SIM7600X) that selects where the module UART is routed.
- The 40-pin Pi header, plus a **side header** labelled 3V3, PWR, D6, D4, Flight, ADC1, IO43, RI, DCD, DTR, CTS, RTS, RX, TX, GND, 5V.
- The earlier project wired the side header with four wires (red/black = 5V/GND, green/white = TX/RX) to a microcontroller on a perfboard.
- On the bench unit the tan plastic serial connector was **removed from the side header**, and the wires to the T-Deck go to the bare header pads instead (see `../../images/tdeck-serial-pads-wired-to-hat.png`). The silk labels on the pads are RXD, TXD, GND and 5V, next to the UART JMP block.

## Why this is the primary modem

- It exposes a **3.3V UART** (RX, TX, RTS, CTS, RI, DTR, DCD) on a header. The older V1 schematic (`reference/Waveshare_SIM7600G-H_4G_HAT_schematic_V1.pdf`) shows a TXB0108 level translator between the module and the header, so no divider or shifter is needed and nothing is dead-bugged.
- The T-Deck's 3.3V serial-pad UART connects straight to it: T-Deck TX (GPIO43) to header RX, T-Deck RX (GPIO44) from header TX, shared GND.
- Voice calls and the Ting SIM are already known to work on it. The G-H module on the (B) V2 is a different variant and IMEI with no such proof (see [../ting-and-voice.md](../ting-and-voice.md)).
- The PWR header pin and the PWRKEY button give power control, and RI gives ring/SMS wake, both at 3.3V.

## Wiring to the T-Deck (to confirm on the board)

| T-Deck | Old HAT side header | Notes |
|---|---|---|
| GPIO43 (TX) | RX | Cross-over: T-Deck TX goes to the HAT's RX |
| GPIO44 (RX) | TX | |
| GND | GND | Mandatory |
| a free GPIO (1, 2, 3 or 15) | RI | Optional ring/SMS wake |
| a free GPIO | PWR | Optional power control; check the polarity first |
| VCC pad (as built) | 5V | The T-Deck VCC pad is wired to this pin and is currently the HAT's only power (confirmed by the owner). The earlier plan was to power the HAT from its own 5V supply or USB; see [t-deck.md](t-deck.md) |

The UART JMP setting that routes the module to the side header (rather than to the CP2102 or the Pi header) is **not yet known**; read it off the board and the Waveshare wiki before wiring. The PWR pin polarity (schematic shows "PWR Control" through a transistor to PWRKEY) is also to be confirmed.

## Open questions

- Is the board free, or still in the other project?
- Which UART JMP positions did the earlier setup use, and at what baud? (Check `AT+IPR?` over the CP2102 micro-USB.)
- Does the side header's 3V3 pin supply or take power? Leave it unconnected until known.
- What is the SIM7600A-H's IMEI and does its VoLTE state persist? Read `AT+CGSN` and `AT+CEREG?`.

## Reference

- Waveshare wiki for this family: https://www.waveshare.com/wiki/SIM7600X_4G_%26_LTE_Cat-1_HAT
- The downloaded hardware design covers G(-H) and NA(-H); pad numbers match for the NA-H, which is the closest document to the A-H.
