# Bring-up checklist

Order of work from "nothing soldered" to "T-Deck sends an SMS". Each phase has a pass condition; do not move on until it passes. Background: [at-interface-options.md](at-interface-options.md), [sim7600-pins-to-find.md](sim7600-pins-to-find.md), [level-shifting.md](level-shifting.md).

Pad numbers are module LCC pads from the SIMCom hardware design (V1.10), not HAT silkscreen. Facts about the Waveshare (B) V2 board come from its schematic (`reference/Waveshare_SIM7600X_4G_HAT_B_V2_schematic.pdf`).

## Phase 0: Gather

- [ ] Active SIM (nano? check the slot size; Waveshare lists 1.8V/3V SIMs), PIN disabled or known, SMS and voice enabled on the plan.
- [ ] A SIM that works on this module's bands. The G-H module has global bands; confirm your carrier's LTE bands are covered.
- [ ] **Voice risk to check now:** many carriers have shut down 2G/3G, so voice may need VoLTE. The AT manual describes no VoLTE enable command, so I could not confirm the G-H does VoLTE. Check with SIMCom/Waveshare and your carrier before building the voice side. SMS is less affected, since LTE-only SMS also generally works over IMS, but test it.
- [ ] Antenna on the **MAIN** u.FL (and AUX if available). Never power the modem without an antenna.
- [ ] Headset with mic for the 3.5mm jack (4-pole, CTIA/OMTP may matter; try both if no audio).
- [ ] Stable 5V supply or USB charger for the HAT (not the T-Deck).
- [ ] Tools: multimeter with continuity, fine soldering iron and tip, flux, 30 AWG enamel or wire-wrap wire, magnification, Kapton tape.
- [ ] USB-UART adapter with **3.3V** I/O (FTDI/CP2102), plus the level-shift parts from [level-shifting.md](level-shifting.md).
- [ ] Python: `venv/bin/pip install pyserial` (or any terminal such as `screen`, `minicom`).

## Phase 1: Prove the modem over micro-USB (no soldering)

1. [ ] Antenna and SIM in, then connect the HAT to the computer by micro-USB. The green PWR LED should light; the module auto-powers on because PWRKEY is tied low on this board.
2. [ ] Wait about 15-20 s. The red NET LED starts blinking.
3. [ ] Find the serial ports: macOS `ls /dev/cu.*`, Linux `ls /dev/ttyUSB*` (the AT port is often `ttyUSB2`). If nothing appears on macOS, the SIMCom driver may be needed; use a Linux machine or Windows if so.
4. [ ] Open each port at 115200 and send `AT`. The port that answers `OK` is the AT port: `venv/bin/python -m serial.tools.miniterm <port> 115200`.
5. [ ] Identify and check the network:

   | Command | Expect |
   |---|---|
   | `ATE1` | `OK` (echo on) |
   | `ATI` | SIMCOM, SIM7600G-H, revision |
   | `AT+CPIN?` | `+CPIN: READY` |
   | `AT+CSQ` | first number 10-31 (99 = no signal) |
   | `AT+CREG?` / `AT+CEREG?` | second number 1 (home) or 5 (roaming) |
   | `AT+COPS?` | your carrier name |
   | `AT+CNUM` | your number (may be empty on some SIMs) |

6. [ ] Record what `AT+IPR?`, `AT+IFC?` and `AT+CMUX?` return.
7. [ ] SMS test: `AT+CMGF=1`, `AT+CSCS="GSM"`, `AT+CMGS="<your number>"`, type the text, press Ctrl-Z. Expect `+CMGS: <n>` then `OK`. Send one to the SIM and read it with `AT+CMGL="ALL"` (set `AT+CNMI=2,1` to get `+CMTI:` notifications).
8. [ ] **Pass:** SIM ready, registered, one SMS sent and one received.

If it fails: no `OK` on any port means wrong port or driver; `+CPIN: SIM PIN` means unlock with `AT+CPIN="1234"`; `CREG: 0,2` forever means antenna or band problem.

## Phase 2: Voice over the headset jack

1. [ ] Plug in the headset. `AT+CSDVC?` (1 = handset, the default).
2. [ ] Outgoing: `ATD<number>;` (the semicolon is required for voice). Check `AT+CLCC` while ringing, speak, hang up with `ATH`.
3. [ ] Incoming: call the SIM from another phone and watch for `RING` and `+CLIP:` (enable with `AT+CLIP=1`). Answer with `ATA`.
4. [ ] Volume: `AT+CLVL=<0-5>`; mute: `AT+CMUT=1`.
5. [ ] **Pass:** you hear and are heard both ways. If calls drop immediately or `NO CARRIER` appears right away, suspect the voice/VoLTE issue from Phase 0.

## Phase 3: Verify the board before soldering (HAT unpowered)

1. [ ] Unplug USB and wait for the LEDs to go dark.
2. [ ] Find pad 1 of the LCC edge and confirm numbering against the pin assignment figure in the hardware design (V1.10 p.19). Mark pads 68 (RXD), 71 (TXD), 15 (VDD_1V8) and a GND pad nearby with a fine marker or tape.
3. [ ] Continuity: pad 3 (PWRKEY) to GND should beep (R26 0Ω). This confirms the schematic matches your board.
4. [ ] Continuity: pads 68 and 71 to the micro-USB, the USB pads and each other should all be open.
5. [ ] Continuity: GND pad to the HAT GND pad near the SIM slot.
6. [ ] Check the USB1/USB2 edge pads (optional): see at-interface-options.md A2.
7. [ ] **Pass:** PWRKEY shorted to GND, UART pads open to everything.

## Phase 4: Build and test the level-shift circuit on a breadboard

1. [ ] Build the TX divider (1.0k / 1.2k) and the RX BSS138 shifter per [level-shifting.md](level-shifting.md), with the 1.8V rail and the 3.3V rail supplied by a bench source (do **not** use the module yet).
2. [ ] With 1.8V and 3.3V applied: drive the 3.3V side TX input to 3.3V and confirm the divider output reads about 1.8V.
3. [ ] Drive the 1.8V side with a 1.8V signal (or tie it to 1.8V and to GND) and confirm the 3.3V-side output follows: about 3.3V when high, about 0V when low.
4. [ ] **Pass:** both directions behave at idle levels. Do not connect to the module until both pass.

## Phase 5: Dead-bug the UART (still on the PC)

1. [ ] HAT powered off. Solder fine wires to pad 71 (TXD), pad 68 (RXD) and a nearby GND pad. Add pad 15 (VDD_1V8) as the 1.8V reference for the shifter. Strain-relieve each wire with Kapton tape and keep them short and away from the antenna.
2. [ ] Re-check continuity: pad 68 and 71 not shorted to neighbours or to each other, GND wire to GND.
3. [ ] Power the HAT over micro-USB. After about 15 s measure pad 15: expect about 1.8V.
4. [ ] With a multimeter or scope, pad 71 (TXD) should idle at about 1.8V. If it reads 0V or 3.3V, stop and recheck.
5. [ ] Connect the level shifter between the pads and the USB-UART adapter (adapter TX to the divider, divider to pad 68; pad 71 through the shifter to the adapter RX; grounds common).
6. [ ] On the PC, open the adapter at 115200 and send `AT`. Expect `OK`.
7. [ ] Repeat the Phase 1 commands over this UART (at least `ATI`, `AT+CSQ`, one SMS).
8. [ ] **Pass:** the same results as over USB, now over the dead-bug UART.

If `AT` gives no answer, swap TX/RX lines at the adapter, check the shifter, and confirm the module finished booting (wait 20 s). Garbage characters mean a baud mismatch; check `AT+IPR?` over USB.

## Phase 6: Move to the T-Deck

1. [ ] Confirm T-Deck Grove UART pins from the schematic (GPIO43 TX / GPIO44 RX per LilyGO code) and that nothing else on the board uses them.
2. [ ] Write a minimal sketch: `Serial1` on those pins at 115200, set `BOARD_POWERON` (GPIO10) high, forward the T-Deck USB serial to the modem and back.
3. [ ] Send `AT` and the Phase 1 commands from the T-Deck serial monitor.
4. [ ] **Pass:** AT works from the T-Deck. Then write the firmware: SMS send/receive and call control (see [at-interface-options.md](at-interface-options.md) for the command lists).

## Phase 7: Add the control lines (optional)

1. [ ] STATUS (pad 49) through a 1.8V to 3.3V shifter to a free T-Deck GPIO (1, 2, 3 or 15) so the firmware knows when the modem is ready.
2. [ ] RI (pad 69) the same way, to wake the T-Deck on SMS/call.
3. [ ] RESET (pad 4) via the open-drain transistor.
4. [ ] PWRKEY control only if you remove R26 and add the transistor driver. Otherwise leave it as is.

## Stop conditions (do not proceed)

- Any module pin measured above about 2.0V when it should be 1.8V logic.
- The module gets warm or the HAT browns out at power-up (check the 5V supply).
- Wires shorting to neighbouring LCC pads.
