# SIM7600G-H pads to find and dead-bug

> **Fallback only:** this dead-bug pad list is for the (B) V2 / SIM7600G-H. The primary route uses the old A-H HAT's header and needs none of it.

Source: `reference/SIM7600G-H_SIM7600NA-H_Hardware_Design_V1.10.pdf` (pin map and signal tables p.19-20, power on/off timing p.29-30, UART level note p.34). The pad numbers match the V1.01 document for the G-H. All module logic pins are **1.8V** (VIH min 1.26V, max 2.0V).

Pad numbers are the module's LCC pad numbers, not HAT silkscreen labels. Verify pad 1 orientation and every pad by continuity to a known net (GND pads) and with a scope or meter before soldering. Use fine (30 AWG wire-wrap) wire and strain-relieve it.

## Required for texting

| Signal | Pad | Dir (module) | Level | Purpose | Status |
|---|---|---|---|---|---|
| TXD | 71 | out | 1.8V | Module to T-Deck RX (GPIO44). Needs a shifter up to 3.3V | to find |
| RXD | 68 | in | 1.8V | T-Deck TX (GPIO43) to module. Divider or shifter down to 1.8V | to find |
| GND | 1, 2, 5, 10, 14, ... | | | Common ground with T-Deck, use the pad nearest the UART pads | to find |
| PWRKEY | 3 | in, active low, internal pull-up to 1.8V | 1.8V | **Tied to GND via R26 (0Ω) on the V2 (B), so the module auto-powers on.** Skip this wire for now; to control power from the T-Deck remove R26, then pull low 100-500ms (on) / ~2.5s (off) with an open-drain transistor only | optional |
| STATUS | 49 | out | 1.8V | High = powered on and firmware ready (15-16s after power-up). Lets firmware know when to send AT | to find |

## Strongly wanted

| Signal | Pad | Dir | Level | Purpose | Status |
|---|---|---|---|---|---|
| RESET | 4 | in, active low, 40k pull-up to 1.8V | 1.8V | Emergency reset, 100-500ms low. Open-drain transistor only | to find |
| RI | 69 | out | 1.8V | Goes low on incoming SMS/URC, and pulses on incoming call. Wake and ring trigger for the T-Deck | to find |
| DTR | 72 | in, pull-up | 1.8V | Pulled low to wake the module from sleep | to find |
| VDD_1V8 | 15 | power out | 1.8V | 1.8V reference output, 50mA max. Use as the low-side supply of the level shifter | to find |
| RTS / CTS | 66 / 67 | out / in | 1.8V | Hardware flow control, optional at 115200 baud | to find |
| DCD | 70 | out | 1.8V | Carrier detect, unused | skip |

## Voice calls: two options

| Option | Pads | Notes |
|---|---|---|
| A. HAT audio jack (recommended first) | none | The HAT's NAU8810 codec on the module's PCM bus drives the 3.5mm jack. Plug in a headset, no extra wiring. Run calls with `ATD`, `ATA`, `ATH` |
| B. PCM digital audio to the T-Deck | PCM_OUT 73, PCM_IN 74, PCM_SYNC 75, PCM_CLK 76 | All 1.8V. Would need a PCM/I2S path to the T-Deck's mic/speaker and shifting on all four lines; the codec on the HAT would have to be disconnected. Later phase |

## Optional / fallbacks

| Signal | Pad | Notes |
|---|---|---|
| USB_DP / USB_DN | 13 / 12 | Already routed on the HAT via its hub and micro-USB. Not a good phone link; see [at-interface-options.md](at-interface-options.md) (A2) for why host mode from the T-Deck is a research item |
| DBG_TXD / DBG_RXD | 42 / 106 | Debug log UART, ignore |
| NETLIGHT | 51 | Network status LED output |
| FLIGHTMODE | 54 | Alternate airplane mode control |
| VBAT | 38, 39, 62, 63 | 3.4-4.2V module supply, powered through the HAT. Do not touch |

## Minimum wires for a first test

GND, TXD (71), RXD (68), VDD_1V8 (15) (PWRKEY not needed while R26 is fitted). Add STATUS (49) and RI (69) next. Six to eight dead-bug wires in total, plus RESET (4) if you want recovery.
