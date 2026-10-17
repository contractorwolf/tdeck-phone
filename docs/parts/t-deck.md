# LilyGO T-Deck

Identified from `images/PXL_20261017_061914260.jpg` (front) and `PXL_20261017_061919865.jpg` (back).

## What is on the board

- ESP32-S3-WROOM-1 module (Espressif), 3.3V logic.
- 2.8" LCD with the LILYGO T-Keyboard (this unit has Thai key legends) and a trackball. The keyboard is a separate ESP32-C3 acting as an I2C slave.
- SX1262 LoRa radio (LORA32, 868/915MHz) with u.FL antenna connector. Not needed for this project.
- microSD slot, BAT and SPEAKR JST connectors, USB-C, RST button.
- 4-pin header at the top right, silk-labelled RX / TX / VCC / GND. This is the Grove (HY2.0-4P) UART port and is the intended attachment point for the modem.

## Pins relevant to this project

From `reference/LilyGO_T-Deck_utilities.h` (LilyGO's own example code):

| Function | GPIO | Notes |
|---|---|---|
| Grove UART TX | 43 | named `BOARD_GPS_TX_PIN`; is the ESP32-S3 UART0 default TX |
| Grove UART RX | 44 | named `BOARD_GPS_RX_PIN` |
| Peripheral power enable | 10 | `BOARD_POWERON`, must be HIGH to power peripherals |
| I2C SDA / SCL | 18 / 8 | keyboard, touch, other I2C parts |
| I2S WS / BCK / DOUT | 5 / 7 / 6 | speaker output |
| Mic (ES7210) MCLK / LRCK / SCK / DIN | 48 / 21 / 47 / 14 | on-board microphone codec |
| Free breakout GPIOs | 1, 2, 3, 15 | `BOARD_TBOX_G01..G04`; candidates for PWRKEY, RESET, STATUS, RI |
| SPI MOSI / MISO / SCK | 41 / 38 / 40 | shared by LCD, SD, LoRa |

Caveats: GPIO43/44 are also UART0 (the USB serial console on some builds), so firmware must use a different UART instance or disable console logging on them. This table comes from example code, not a verified schematic reading; confirm against `reference/LilyGO_T-Deck_schematic.pdf`.

## Electrical

- GPIO input-high threshold is 0.75 x VDD = 2.475V at 3.3V (ESP32-S3 datasheet, DC characteristics). A 1.8V signal is not reliably read as high.
- GPIO outputs are 3.3V and must never be connected straight to the module's 1.8V pins.
- Powered from USB-C or a LiPo on the BAT connector. The T-Deck should not power the modem; the HAT runs from its own supply.

## Audio

The T-Deck has an I2S speaker output and an ES7210 mic codec. For calls, the simplest route is to leave audio on the HAT's codec and 3.5mm jack (see the HAT doc). Routing call audio through the T-Deck would use the module's PCM pins (73-76) and is a later phase.
