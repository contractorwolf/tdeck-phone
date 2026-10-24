# LilyGO T-Deck

Identified from `images/PXL_20261017_061914260.jpg` (front) and `PXL_20261017_061919865.jpg` (back). Wiring to the modem HAT: `images/tdeck-serial-pads-wired-to-hat.png`.

## What is on the board

- ESP32-S3-WROOM-1 module (Espressif), 3.3V logic.
- 2.8" LCD with the LILYGO T-Keyboard (this unit has Thai key legends) and a trackball. The keyboard is a separate ESP32-C3 acting as an I2C slave.
- SX1262 LoRa radio (LORA32, 868/915MHz) with u.FL antenna connector. Not needed for this project. On the bench unit the LoRa board is covered with blue tape; it is unused and none of the wiring below goes to it.
- microSD slot, BAT and SPEAKR JST connectors, USB-C, RST button.
- A 4-pad serial connector at the top right next to the RST button, silk-labelled **GND, VCC, TX, RX** (left to right in the photo). These are **bare solder pads in a silkscreen box, not a Grove/JST socket** (an earlier version of this doc called it a Grove HY2.0-4P port, which was wrong). It is on the T-Deck's main PCB, not on the LoRa board. It is the attachment point for the modem.

## How the modem HAT is connected today

`images/tdeck-serial-pads-wired-to-hat.png` shows four wires (black, red, green, white) soldered directly to the four serial pads, running to the SIM7600 HAT's side header (silk RXD, TXD, GND, 5V). On the HAT the tan plastic serial connector was removed, so the wires go to the bare header pads. The UART JMP jumpers on the HAT are visible in the photo.

**Power (confirmed by the owner):** the T-Deck **VCC** pad is connected to the HAT's **5V** pin, and **right now the HAT is powered only through that wire, from the T-Deck**. It has no separate 5V supply or USB power connected.

Not confirmed, so treat as open: which wire goes to which pad and which HAT pin (the wire colours are not a guarantee), what voltage the VCC pad actually carries (not measured), and how much current it can supply. The HAT's module draws large current bursts when it transmits, so expect brownouts or resets during network registration, calls and SMS sends if the pad cannot supply it. The earlier plan was that the HAT runs from its own 5V and the T-Deck does not power it; if the red wire joins T-Deck VCC to the HAT 5V pin, check which side supplies power before powering both from separate sources.

## Pins relevant to this project

From `reference/LilyGO_T-Deck_utilities.h` (LilyGO's own example code):

| Function | GPIO | Notes |
|---|---|---|
| Serial pad TX | 43 | named `BOARD_GPS_TX_PIN`; is the ESP32-S3 UART0 default TX |
| Serial pad RX | 44 | named `BOARD_GPS_RX_PIN` |
| Peripheral power enable | 10 | `BOARD_POWERON`, must be HIGH to power peripherals |
| I2C SDA / SCL | 18 / 8 | keyboard, touch, other I2C parts |
| I2S WS / BCK / DOUT | 5 / 7 / 6 | speaker output |
| Mic (ES7210) MCLK / LRCK / SCK / DIN | 48 / 21 / 47 / 14 | on-board microphone codec |
| Free breakout GPIOs | 1, 2, 3, 15 | `BOARD_TBOX_G01..G04`; candidates for PWRKEY, RESET, STATUS, RI |
| SPI MOSI / MISO / SCK | 41 / 38 / 40 | shared by LCD, SD, LoRa |

Caveats: the serial pad GPIO numbers (43/44) come from LilyGO's pin header (where they are named for the GPS port on the T-Deck Plus) and have not been checked on this board's pads with a meter or the schematic. GPIO43/44 are also UART0 (the USB serial console on some builds), so firmware must use a different UART instance or disable console logging on them. This table comes from example code, not a verified schematic reading; confirm against `reference/LilyGO_T-Deck_schematic.pdf`.

## Electrical

- GPIO input-high threshold is 0.75 x VDD = 2.475V at 3.3V (ESP32-S3 datasheet, DC characteristics). A 1.8V signal is not reliably read as high.
- GPIO outputs are 3.3V and must never be connected straight to the module's 1.8V pins.
- Powered from USB-C or a LiPo on the BAT connector. The earlier plan was that the T-Deck would not power the modem and the HAT would run from its own supply. As built, the HAT is powered only from the T-Deck VCC pad wired to the HAT 5V pin (see the wiring section above), so that plan no longer matches the hardware. Measure the VCC pad voltage and watch for resets when the modem transmits. Do not connect a second 5V source to the HAT while this wire is in place; the two supplies would be tied together.

## Audio

The T-Deck has an I2S speaker output and an ES7210 mic codec. For calls, the simplest route is to leave audio on the HAT's codec and 3.5mm jack (see the HAT doc). Routing call audio through the T-Deck would use the module's PCM pins (73-76) and is a later phase.
