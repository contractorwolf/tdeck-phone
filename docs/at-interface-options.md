# Ways to send and receive AT commands

How the T-Deck (or a PC, while bringing things up) can talk AT to the SIM7600G-H, using what the Waveshare HAT already has and by tapping the module's edge pads. Pad numbers and page references are from `reference/SIM7600G-H_SIM7600NA-H_Hardware_Design_V1.10.pdf` (V1.10).

Evidence level: statements about the module come from the SIMCom documents. Statements about the **V2 (B)** HAT are inferred from the photos and the older V1 schematic, and are marked *verify*.

## Summary

| # | Path | Needs soldering | Voltage | Good for | Verdict |
|---|---|---|---|---|---|
| A1 | HAT micro-USB to a PC | no | USB | Bring-up: SIM, registration, SMS and call tests, firmware and AT experiments | **Do first** |
| A2 | HAT USB pads (USB1/2/3) to a small USB-host bridge board | light | USB 5V | Using the HAT as designed, with a separate host MCU | Fallback |
| A3 | HAT audio jack | no | analog | Voice audio only, not AT | Use for calls |
| A4 | Any HAT UART header, test points or populated level-shifter side *(verify)* | maybe | 3.3V | Skips the level shifter | Check on the board |
| B1 | Module UART pads 68 (RXD) / 71 (TXD) | yes, dead-bug | 1.8V | T-Deck firmware link | **Primary target** |
| B2 | B1 plus RI/DTR/STATUS/PWRKEY/RESET | yes | 1.8V | Real phone behaviour: ring wake, power control | Add after B1 works |
| B3 | B1 with `AT+CMUX` multiplexing | no extra | 1.8V | Separate virtual channels (AT, call state, data) over one UART | Optional software layer |
| B4 | Module USB_DP/USB_DN pads 13/12 | yes | USB | Same as A2 | Not recommended |
| B5 | DBG_TXD / DBG_RXD pads 42 / 106 | yes | 1.8V | Module log output only, **not** an AT port | Skip |

## What the HAT already gives you (no module pads)

### A1. Micro-USB "USB HUB" to a PC

- The module has a USB 2.0 high-speed interface that can be used to send AT commands (hardware design p.~7, "USB 2.0 high speed interface, can used be send AT command, software debug..."). The HAT routes it through a hub chip to the micro-USB connector.
- A host OS sees several serial ports from one SIMCom device. One is the AT port, the others are diagnostics, GNSS NMEA and a PPP/modem port. On macOS look for `/dev/cu.usbserial-*` or `/dev/cu.usbmodem*`; on Linux `/dev/ttyUSB0-3` (AT is usually `ttyUSB2`). Try each port with `AT` and see which returns `OK`.
- Use it to prove the SIM, antenna and network, to practise the exact SMS and voice AT sequences, and to read the module's current settings (`AT+IPR?`, `AT+IFC?`, `AT+CMUX?`) before wiring the UART.
- Power: this is also the easiest 5V input for the HAT.
- `AT+CUSBPIDSWITCH` can change the USB composition if a port set is missing (AT manual).

### A2. HAT USB pads (USB1/USB2/USB3) to a separate host

- The bottom of the HAT has pads labelled USB1, USB2, USB3 (5V, DM, DP, GND) and D-/D+ near the jack. These are most likely downstream ports of the on-board hub *(verify with continuity to the micro-USB shield/pins)*.
- Could feed a small USB-host bridge (e.g. an ESP32-S3 or Raspberry Pi Pico acting as host) that translates to UART/BLE/Wi-Fi for the T-Deck. This adds a second MCU, so it is a fallback only.
- **T-Deck direct USB host:** the ESP32-S3 has native USB with a host stack, and the T-Deck's USB-C is wired to it, so a USB-C OTG adapter to the HAT is conceivable. Two catches: the T-Deck would have to supply 5V on VBUS (not confirmed that its board can), and the SIM7600 USB ports are vendor-specific serial interfaces rather than standard CDC-ACM, so the stock ESP-IDF CDC-ACM host driver is not expected to bind to them without custom bulk-endpoint code. Treat as a research item, not a plan.

### A3. 3.5mm audio jack

Carries call audio through the HAT's NAU8810 codec on the module PCM bus. It is not an AT path, but it is the simplest way to get voice audio while AT runs on another link. Calls are controlled with `ATD<number>;`, `ATA`, `ATH` and monitored with `AT+CLCC` and the `RING`/`+CLIP` URCs.

### A4. UART access that the HAT may already have *(verify)*

The V1 HAT schematic (`reference/Waveshare_SIM7600G-H_4G_HAT_schematic_V1.pdf`) shows the module UART passing through a **TXB0108** level shifter to a **CP2102** USB-UART chip and to GPIO header nets named U_TX / U_RX, with PWR_KEY, RESET and STATUS nets as well. The V2 (B) board swaps the USB path for a hub, but may keep a shifted 3.3V UART on a header footprint or on test points. If it does, that gives a 3.3V UART with **no level shifter to build** and PWR_KEY/RESET/STATUS control. Check the board for any unpopulated header footprint, test pad row or vias near the TXB-type shifter, and ring them out to module pads 68/71. This could replace most of the dead-bug work.

## Hijacking pads on the module edge

### B1. UART RXD/TXD (the main plan)

- Module is a DCE with a 7-wire UART: TXD pad 71, RXD pad 68, RTS 66, CTS 67, RI 69, DCD 70, DTR 72 (p.~8, 20).
- Logic is 1.8V. Level shifting is covered in [level-shifting.md](level-shifting.md).
- Default 115200 baud, 8N1. Supported rates 300 up to 4Mbps (p.34). 115200 is plenty for SMS and call control; raise it with `AT+IPR=<rate>` (use 460800 or 921600 only if running data or CMUX, and shift accordingly).
- Hardware flow control is off by default and can be enabled with `AT+IFC`. Not needed for SMS and voice at 115200.
- The module takes about 13-14s after PWRKEY before the UART is ready, and 15-16s until STATUS goes high (p.29).
- Pads can be soldered at the corners of the LCC edge, with wire-wrap wire and a fine tip. Confirm pad numbers by continuity to a known net before soldering.

### B2. Adding control and notification lines

| Line | Pad | Use |
|---|---|---|
| PWRKEY | 3 | T-Deck can power the modem on and off; needs an open-drain driver |
| RESET | 4 | Recovery when AT stops responding |
| STATUS | 49 | Tells the firmware the module has finished booting, before sending AT |
| RI | 69 | Falls on incoming SMS/URC and pulses for a voice call, so the T-Deck can sleep and wake on it (p.~30) |
| DTR | 72 | Pulled low to wake the module from sleep mode |

Start with GND, TXD, RXD, PWRKEY and VDD_1V8; add STATUS and RI as soon as AT works.

### B3. `AT+CMUX` (software only, no additional pads)

`AT+CMUX` runs a GSM 07.10 multiplexer on the UART so one pair of wires behaves as several virtual serial channels (AT manual, "Enable the multiplexer over the UART"). A channel for commands and a channel for URCs/call state keeps incoming `RING`/SMS notifications from interleaving with command responses. Worth considering later; adds firmware complexity, and `AT+CATR` can route URCs to a chosen port without it.

### B4. USB_DP / USB_DN (pads 13 / 12)

Direct access is possible, but the same bus is already routed through the HAT hub and micro-USB, so tapping pads would create a stub on a 480Mbps differential pair for no gain over A1/A2.

### B5. Debug UART (pads 42 / 106)

Labelled "Log output / Log input" in the pin table. Not a general AT port; ignore.

### Not useful for host AT

SPI (pads 6-9, optional Bluetooth UART function), I2C (SCL/SDA pads 55/56, the module is the master), SDIO, HSIC and SGMII. Skip these.

## Bring-up order (recommended)

1. **A1:** plug the HAT into a computer by micro-USB, find the AT port, run `AT`, `ATI`, `AT+CPIN?`, `AT+CSQ`, `AT+CREG?` (use `AT+CEREG?` on LTE), `AT+COPS?`.
2. Practise SMS: `AT+CMGF=1`, `AT+CMGS="<number>"` (end with Ctrl-Z, 0x1A), `AT+CNMI=2,1`, `AT+CMGL="ALL"`. Practise a call: `ATD<number>;`, `AT+CLCC`, `ATH`. Use the headset in the HAT jack.
3. Check what the HAT and V2 board expose (A4). Read `AT+IPR?` and note the UART baud.
4. **B1:** wire a USB-UART adapter (3.3V FTDI or CP2102) to pads 68/71 through the level-shifting circuit and repeat step 1 from the PC. This tests the dead-bug and shifter without involving T-Deck firmware.
5. Move the same wires to the T-Deck Grove pins (GPIO43/44) and test with firmware.
6. Add PWRKEY/STATUS/RI (B2) and move to a no-PC workflow.

## Risks to keep in mind

- Do not power the module from two different sources at once; if the micro-USB is attached and the T-Deck is also feeding anything, share only ground.
- Whatever the HAT already connects to pads 68/71 (CP2102/TXB0108 in V1) may load or fight the dead-bug wires. If you find such a connection, lift or cut it, or use the A4 point instead.
- URCs (`RING`, `+CMTI`, `+CLIP`) can arrive at any time, including during a command response; the firmware must parse them asynchronously.
