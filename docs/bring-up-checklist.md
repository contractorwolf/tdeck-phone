# Bring-up checklist (old SIM7600A-H HAT)

Order of work from "nothing connected" to "T-Deck sends an SMS and places a call". Each phase has a pass condition; do not move on until it passes. Board: [parts/waveshare-sim7600x-hat-original.md](parts/waveshare-sim7600x-hat-original.md). AT commands: [at-interface-options.md](at-interface-options.md). Voice caveats: [ting-and-voice.md](ting-and-voice.md).

The fallback route for the (B) V2 / G-H (dead-bug UART, level shifting) is in [parts/waveshare-sim7600x-hat.md](parts/waveshare-sim7600x-hat.md), [sim7600-pins-to-find.md](sim7600-pins-to-find.md) and [level-shifting.md](level-shifting.md).

## Phase 0: Gather

- [ ] Confirm the old HAT is free to use.
- [ ] The Ting SIM (nano? check the slot), PIN disabled or known.
- [ ] Antenna on the **MAIN** u.FL (and AUX). Never power the modem without an antenna.
- [ ] Headset with mic for the 3.5mm jack (CTIA/OMTP may matter).
- [ ] Stable 5V supply or USB charger for the HAT (not the T-Deck).
- [ ] Jumper wires; multimeter.
- [ ] Python: `venv/bin/pip install pyserial` (or `screen`/`minicom`).

## Phase 1: Prove the modem from a PC (CP2102 micro-USB)

1. [ ] Set the UART JMP to route the module to the **CP2102** (check the Waveshare wiki for the exact positions).
2. [ ] Antenna and SIM in, connect the "USB TO UART" micro-USB. Press PWRKEY (button) if the PWR LED does not light. Wait 15-20 s for the NET LED to blink.
3. [ ] Find the port (`ls /dev/cu.*`; a CP210x driver may be needed on macOS) and open it: `venv/bin/python -m serial.tools.miniterm <port> 115200`.
4. [ ] Run these:

   | Command | Expect |
   |---|---|
   | `AT` | `OK` |
   | `ATI` | SIMCOM, SIM7600A-H |
   | `AT+CGSN` | the IMEI (note it) |
   | `AT+CPIN?` | `+CPIN: READY` |
   | `AT+CSQ` | first number 10-31 |
   | `AT+CREG?` / `AT+CEREG?` | second number 1 or 5 |
   | `AT+COPS?` | Ting's network |
   | `AT+IPR?` | the baud rate (note it) |

5. [ ] SMS: `AT+CMGF=1`, `AT+CSCS="GSM"`, `AT+CMGS="<your number>"`, text, Ctrl-Z. Send one to the SIM and read it with `AT+CMGL="ALL"` (`AT+CNMI=2,1` for `+CMTI:` notices).
6. [ ] **Pass:** SIM ready, registered, one SMS sent and one received.

## Phase 2: Voice over the headset jack

1. [ ] Headset in. `AT+CSDVC?`.
2. [ ] Outgoing: `ATD<number>;` then `AT+CLCC`, speak, `ATH`.
3. [ ] Incoming: call the SIM, watch for `RING` and `+CLIP:` (`AT+CLIP=1`), answer with `ATA`.
4. [ ] Volume `AT+CLVL=<0-5>`, mute `AT+CMUT=1`.
5. [ ] **Pass:** audio both ways. Immediate `NO CARRIER` points to the VoLTE/provisioning issue in [ting-and-voice.md](ting-and-voice.md).

## Phase 3: Route the UART to the side header and test with a USB-UART adapter

1. [ ] Move the UART JMP to route the module to the **side header** (RX/TX). Confirm the positions against the wiki or by continuity.
2. [ ] Measure the header TX idle voltage with the HAT on: it should be about 3.3V. If it is not, stop.
3. [ ] Connect a 3.3V USB-UART adapter: adapter TX to header RX, adapter RX to header TX, GND to GND.
4. [ ] Send `AT` at the baud from Phase 1. **Pass:** `OK`, and `ATI` repeats.

## Phase 4: Connect the T-Deck

1. [ ] Confirm the Grove UART pins from the T-Deck schematic (GPIO43 TX / GPIO44 RX) and that nothing else uses them.
2. [ ] Wire T-Deck TX to header RX, T-Deck RX to header TX, GND to GND. Keep the HAT on its own 5V.
3. [ ] Minimal sketch: `Serial1` on those pins at the HAT baud, set `BOARD_POWERON` (GPIO10) high, forward the USB serial to the modem and back.
4. [ ] **Pass:** `AT`, `ATI` and the Phase 1 commands work from the T-Deck serial monitor. Then write the firmware: SMS send/receive and call control.

## Phase 5: Optional control lines

1. [ ] RI to a free T-Deck GPIO (1, 2, 3 or 15) to wake on SMS or a call.
2. [ ] PWR to a free GPIO for power control, after confirming the polarity and the transistor in the schematic.
3. [ ] Voice from the T-Deck: swap the jack for PCM to the T-Deck codec (later research).

## Stop conditions

- Header TX or RX measures above 3.3V or an unexpected voltage.
- The HAT gets warm or browns out at power-up (check the 5V supply).
- Call attempts fail at once with `NO CARRIER` on the A-H as well; reconsider the SIM and plan.
