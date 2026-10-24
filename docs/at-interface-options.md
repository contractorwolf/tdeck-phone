# Ways to send and receive AT commands

> **Status:** the project now uses the old SIM7600A-H HAT, which has a 3.3V UART header (see [parts/waveshare-sim7600x-hat-original.md](parts/waveshare-sim7600x-hat-original.md)). The A/B options below describe the **(B) V2 / G-H fallback**; the AT command sets apply to both modules.

How the T-Deck (or a PC, while bringing things up) can talk AT to the SIM7600G-H, using what the Waveshare HAT already has and by tapping the module's edge pads. Pad numbers and page references are from `reference/SIM7600G-H_SIM7600NA-H_Hardware_Design_V1.10.pdf` (V1.10).

Evidence level: statements about the module come from the SIMCom documents. Statements about the **V2 (B)** HAT are inferred from the photos and the older V1 schematic, and are marked *verify*.

## Summary

| # | Path | Needs soldering | Voltage | Good for | Verdict |
|---|---|---|---|---|---|
| A1 | HAT micro-USB to a PC | no | USB | Bring-up: SIM, registration, SMS and call tests, firmware and AT experiments | **Do first** |
| A2 | HAT hub upstream (micro-USB or D+/D- pads) with the T-Deck as USB host. USB1/2/3 are separate hub ports and do **not** reach the module | light | USB 5V | Research item | Fallback |
| A3 | HAT audio jack | no | analog | Voice audio only, not AT | Use for calls |
| A4 | Existing UART on the HAT | n/a | n/a | **None on the V2 (B)**: UART pins are unconnected | Not available |
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

### A2. HAT USB ports (USB1/USB2/USB3 and the D+/D- pads)

Source: Waveshare's (B) V2 schematic (`reference/Waveshare_SIM7600X_4G_HAT_B_V2_schematic.pdf`) and the product page ("USB1~USB2: USB-A connectors, USB3: solder pad", hub input via "D+/D- pogo pin" or "MicroUSB connector").

The board has an FE1.1S 4-port USB hub. How the ports map:

| Hub port | Goes to | Board label |
|---|---|---|
| Upstream | micro-USB `USB.J1`, and the D+/D- pogo pads P3/P4 (the two pads left of the audio jack, for a Pi Zero) | USB HUB, D-/D+ |
| Downstream 1 | the module's USB (pads 12/13) | (internal) |
| Downstream 2 | `H1`, a USB-A host connector footprint | **USB1** (per product page) |
| Downstream 3 | `H2`, a USB-A host connector footprint | **USB2** (per product page) |
| Downstream 4 | solder pads P5-P8: 5V, D4_N, D4_P, GND | **USB3** |

So USB1, USB2 and USB3 are **not connected to the module directly**. They are extra USB ports hanging off the same hub, meant for plugging other USB devices into a Pi. USB1 and USB2 are connector footprints (unpopulated on the bottom of your board; I can't tell from the photo what the edge half-holes are, so continuity-check them). Nothing on them reaches the module's UART.

The only way to reach the module from USB is the **upstream** side: micro-USB or the D+/D- pads. The T-Deck would have to act as the USB host there. Two catches: the T-Deck would have to supply 5V on VBUS (not confirmed that its board can), and the SIM7600 USB ports are vendor-specific serial interfaces rather than standard CDC-ACM, so the stock ESP-IDF CDC-ACM host driver is not expected to bind to them without custom bulk-endpoint code. Research item, not a plan.

### A3. 3.5mm audio jack

Carries call audio through the HAT's NAU8810 codec on the module PCM bus. It is not an AT path, but it is the simplest way to get voice audio while AT runs on another link. Calls are controlled with `ATD<number>;`, `ATA`, `ATH` and monitored with `AT+CLCC` and the `RING`/`+CLIP` URCs.

### A4. UART access the HAT already has: none on the V2 (B)

The (B) V2 schematic shows the module's UART pins (TXD 71, RXD 68, RTS 66, CTS 67, RI 69, DCD 70, DTR 72) as net-label stubs with **nothing connected to them**. There is no CP2102 and no level shifter on this board (those are on the older HAT, `reference/Waveshare_SIM7600G-H_4G_HAT_schematic_V1.pdf`). So there is no ready-made 3.3V UART to tap, and nothing on the board fights wires soldered to those pads. STATUS (49), RESET (4) and VDD_EXT (44) are likewise stubs. The dead-bug approach is the way in.

## Hijacking pads on the module edge

### B1. UART RXD/TXD (the main plan)

- Module is a DCE with a 7-wire UART: TXD pad 71, RXD pad 68, RTS 66, CTS 67, RI 69, DCD 70, DTR 72 (p.~8, 20).
- Logic is 1.8V. Level shifting is covered in [level-shifting.md](level-shifting.md).
- Default 115200 baud, 8N1. Supported rates 300 up to 4Mbps (p.34). 115200 is plenty for SMS and call control; raise it with `AT+IPREX=<rate>` (permanent; `AT+IPR` is **temporary** and reverts at reboot, per the AT manual) and use 460800 or 921600 only if running data or CMUX. On the A-H bench unit the permanent rate is 9600.
- Hardware flow control is off by default and can be enabled with `AT+IFC`. Not needed for SMS and voice at 115200.
- The module takes about 13-14s after power-up before the UART is ready, and 15-16s until STATUS goes high (p.29).
- Pads can be soldered at the corners of the LCC edge, with wire-wrap wire and a fine tip. Confirm pad numbers by continuity to a known net before soldering.

### B2. Adding control and notification lines

| Line | Pad | Use |
|---|---|---|
| PWRKEY | 3 | **On the V2 (B) it is tied to GND through R26 (0Ω), so the module powers on by itself when the HAT gets 5V.** The T-Deck only gets control if R26 is removed; otherwise skip this wire |
| RESET | 4 | Recovery when AT stops responding |
| STATUS | 49 | Tells the firmware the module has finished booting, before sending AT |
| RI | 69 | Falls on incoming SMS/URC and pulses for a voice call, so the T-Deck can sleep and wake on it (p.~30) |
| DTR | 72 | Pulled low to wake the module from sleep mode |

Start with GND, TXD, RXD and VDD_1V8 (PWRKEY is not needed, see above); add STATUS and RI as soon as AT works.

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
3. Read `AT+IPR?` and note the UART baud (the HAT has no UART of its own, see A4).
4. **B1:** wire a USB-UART adapter (3.3V FTDI or CP2102) to pads 68/71 through the level-shifting circuit and repeat step 1 from the PC. This tests the dead-bug and shifter without involving T-Deck firmware.
5. Move the same wires to the T-Deck serial pads (GPIO43/44) and test with firmware.
6. Add PWRKEY/STATUS/RI (B2) and move to a no-PC workflow.

## Risks to keep in mind

- Do not power the module from two different sources at once; if the micro-USB is attached and the T-Deck is also feeding anything, share only ground.
- On the V2 (B), nothing is wired to the module UART pads, so there should be nothing to fight your wires. Confirm with continuity on the pads before soldering.
- URCs (`RING`, `+CMTI`, `+CLIP`) can arrive at any time, including during a command response; the firmware must parse them asynchronously.
