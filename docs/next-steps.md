# Next steps

What is left, in order. Done so far is in [README.md](README.md) and [board-test-findings.md](board-test-findings.md). Hardware steps follow [bring-up-checklist.md](bring-up-checklist.md).

## 1. Wire the T-Deck to the HAT

- [ ] Read the UART JMP setting on the old HAT and confirm it routes the module to the side header (the bench test used the CP2102 path). Confirm positions on the Waveshare wiki or by continuity.
- [ ] Measure the header TX idle voltage with the HAT on (expect about 3.3V) before connecting the T-Deck.
- [ ] Confirm the T-Deck Grove UART pins (GPIO43 TX, GPIO44 RX) against the T-Deck schematic and that nothing else on the board uses them.
- [ ] Connect T-Deck TX to header RX, T-Deck RX to header TX, GND to GND. Keep the HAT on its own 5V supply.
- [ ] Minimal sketch: `Serial1` at 9600 baud, `BOARD_POWERON` (GPIO10) high, forward USB serial to the modem and back. Pass: `AT`, `ATI`, `AT+CSQ` work from the T-Deck.

## 2. Test the receive side

- [ ] Call the line from another phone: expect `RING` and `+CLIP:` (`AT+CLIP=1`), answer with `ATA`, hang up with `AT+CHUP`.
- [ ] Text the line: expect `+CMTI:` (`AT+CNMI=2,1`), read with `AT+CMGL="ALL"` or `AT+CMGR=<n>`.

## 3. Firmware: AT layer

Built as a reusable library; the design, API draft and milestones are in [at-library-plan.md](at-library-plan.md). The checklist below is the short form.

- [ ] Line-based serial reader with a state machine; handle URCs (`RING`, `+CLIP`, `+CMTI`, `+CMGS`, `NO CARRIER`, `VOICE CALL: BEGIN/END`) at any time.
- [ ] Startup sequence: `AT`, `ATE0`, `AT+CMGF=1`, `AT+CSCS="GSM"`, `AT+CLIP=1`, `AT+CNMI=2,1`, wait for registration (`AT+CEREG?`).
- [ ] SMS send: wait for the `> ` prompt, Ctrl-Z, wait up to 120 s for `+CMGS:`; no other commands while pending.
- [ ] Calls: `ATD+<E.164>;`, `AT+CLCC` polling, `AT+CHUP`, `ATA`; volume `AT+CLVL`, mute `AT+CMUT`.
- [ ] Timeouts and error handling for `+CMS ERROR`, `+CME ERROR`, `ERROR`, `NO CARRIER`, `BUSY`.

## 4. Firmware: phone UI

- [ ] Keyboard input (T-Keyboard), display layout, contact list, message list and compose screens.
- [ ] Incoming call and message notifications.
- [ ] Call screen with answer, hang up, mute, volume.

## 5. Audio

- [ ] Decide the call audio path: the HAT's 3.5mm jack and a headset (works now), or the T-Deck speaker and mic fed from the module's PCM bus (needs I2S/PCM work).
- [ ] Bluetooth is not available on the HAT or the S3 (no Classic radio); a wired headset or a separate Bluetooth audio board are the options.

## 6. Control and power

- [ ] RI to a free T-Deck GPIO (1, 2, 3 or 15) for wake on ring/SMS.
- [ ] PWR pin control from the T-Deck, after confirming the polarity in the schematic and that it is safe to drive from 3.3V.
- [ ] Power budget: the HAT needs its own 5V supply during transmit bursts; plan the battery and regulator for a handheld.

## 7. Housekeeping

- [ ] Push the local commits.
- [ ] Decide about history: a phone number is in an earlier pushed commit (`3ba3d3d`). Rewrite history, or leave it.
- [ ] Optional: GNSS (`AT+CGPS=1`, `AT+CGNSSINFO`) for location in messages.
- [ ] Optional: revisit the (B) V2 / SIM7600G-H dead-bug route only if the old HAT is needed elsewhere; check the G-H's IMEI with Ting first.
