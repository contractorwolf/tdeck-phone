# Notes from the legacy XIAO phone sketch

Source: [xiao_phone.ino](xiao_phone.ino), a 480-line Arduino sketch from the earlier project where a Seeed XIAO drove the old Waveshare SIM7600X HAT (SIM7600A-H) over UART. It is legacy code and a rough prototype, so these notes treat it as evidence of what worked on this hardware, not as a design to copy. The user confirmed this setup placed voice calls with the Ting SIM.

## What the sketch does

- Two push buttons: a **call button** dials a hardcoded number, and a **text button** requests the current GPS position, which is then texted to the same number as a Google Maps link.
- Polls GPS every 30 s and keeps the latest fix.
- A USB serial debug console with single-letter commands: `c` call (disabled), `t` text (disabled), `g` GPS, `x` connection (`AT+COPS?`), `d` capabilities (`AT+GCAP`), `p` product info (`ATI`), `#` phone number (`AT+CNUM`). Anything else is echoed.
- Everything the modem sends is printed back to the console, unparsed except for GPS.

## Hardware facts it reveals

| Item | Value in the sketch | Why it matters |
|---|---|---|
| Modem UART | `Serial1.begin(9600)`, 250 ms timeout | The old HAT was talking at **9600 baud**, not the module default of 115200 in our docs. Either the module was set with `AT+IPR=9600` or something in the path forces it. Read `AT+IPR?` over the CP2102 USB before wiring the T-Deck, and use the answer instead of assuming 115200 |
| Debug UART | `Serial.begin(9600)` | Only the IDE console |
| Call button | pin 3, `INPUT`, active **high** | No internal pull-up used, so it had an external pull-down |
| Text button | pin 4, `INPUT`, active high | A comment says to avoid pins that could be SDA, so the buttons were on pins shared with I2C and "requires rewire" |
| LED | pin 13 | Declared and set as output, never used |
| Wiring | not in the sketch | The photo shows four wires (5V, GND, TX, RX) from the HAT side header, so no PWR, RI or flow-control lines were used |

Things the sketch never does, which tells us what the old setup did **not** need:

- No PWRKEY/PWR pin control. The module was powered on some other way (the board's button, or on at power-up).
- No hardware flow control, no RI wake.
- No startup AT handshake, no echo setting, no SIM or registration checks.

## AT commands that were used and worked

### Voice call

```
ATD+15555550123;
```

(Number shown with the digits redacted from the pattern: `ATD+<country code><number>;`.) Notable points:

- Uses **E.164 format with a leading `+` and country code 1**, and the **trailing `;`** required for voice calls.
- Sent with `println`, so terminated by CR LF.
- It never sends `ATH` (hang up), `ATA` (answer), `AT+CLCC` or `AT+CLIP`, and it does not react to `RING`. Hang-up and incoming calls were therefore either done elsewhere or not handled. The T-Deck firmware needs all of those. See [../at-interface-options.md](../at-interface-options.md).
- Audio went through the HAT, presumably the 3.5mm jack. The sketch has no audio configuration (`AT+CSDVC`, `AT+CLVL`), so the module defaults were enough.

### SMS

```
AT+CMGS="<number>"        (println)
delay(200)                (stand-in for waiting for the "> " prompt)
<text> @<millis>          (print, no newline)
0x1A                      (Ctrl-Z)
```

- The destination is in quotes, and the number here is bare digits (`15555550123`), without a `+`.
- The code **never sets `AT+CMGF=1`** (text mode). Either the module was left in text mode from an earlier manual command, or it is a default on that firmware. It is not safe to rely on: the T-Deck firmware should set `AT+CMGF=1` (and `AT+CSCS="GSM"`) at startup. Worth verifying with `AT+CMGF?` on the A-H.
- The prompt wait is a blind 200 ms delay. The commented-out Adafruit FONA code at the end of the file shows the better pattern: send `AT+CMGS`, wait for the `> ` prompt, send text and Ctrl-Z, then wait **up to 10 seconds** for `+CMGS:` and then `OK`.
- The sketch appends ` @<millis>` to each message, probably so the receiver would not drop duplicates. Not needed.

### Diagnostics

`ATI`, `AT+GCAP`, `AT+COPS?`, `AT+CNUM`. The same commands are in the bring-up checklist, so the sketch's console doubles as a record of the commands that gave useful replies.

### GPS

```
AT+CGNSSINFO
```

- Polled every 30 s and parsed from the reply line `+CGNSSINFO: ...`.
- The sketch **never starts GNSS** (`AT+CGPS=1`), so it must have been started by hand or persisted. On a power cycle `AT+CGNSSINFO` would return an empty fix. Our firmware must send `AT+CGPS=1` itself if GPS is wanted.
- A fix is detected only by reply length (between 80 and 90 characters). An empty reply (`,,,,,,,,`) is shorter, so the length is a crude "has fix" test. Fragile.
- Latitude and longitude are cut out as the 11 characters before `,N,` and `,W,`, then converted to degrees plus decimal minutes for a Maps link (`https://www.google.com/maps/place/<lat>,-<lon>`). This hardcodes north latitude, west longitude and a longitude under 100 degrees. It is fine for the eastern US only.
- This confirms the **A-H GNSS works with a GNSS antenna on the HAT** and returns `+CGNSSINFO`, which could be a later feature (location in a message) for the T-Deck.

## Behaviour and bugs worth not repeating

- **Whole-chunk reads.** `Serial1.readString()` with a 250 ms timeout reads whatever has arrived, then `trim()`. A response split across the timeout, or an unsolicited message arriving mid-response, will be mangled. The T-Deck code should read line by line, with a state machine that handles URCs (`RING`, `+CLIP`, `+CMTI`, `+CMGS`) at any time.
- **Fixed-offset trimming.** `getMessageBody` drops the last 5 characters assuming a trailing OK and newline. It breaks on any other reply.
- **Blocking delays** in the SMS path, and button reads in the main loop with a time-based debounce that sets the next time to `now + 1000`. It works but is clumsy; the T-Deck has a keyboard so none of the button code carries over.
- The `incoming == "c"` and `"t"` debug paths are disabled, so calls and texts could be triggered only from the buttons.
- **Hardcoded, unvalidated destination number**, and `ATD` with no check of the reply. No error handling at all (`ERROR`, `+CMS ERROR`, `NO CARRIER`, `BUSY`).
- A comment says "NEED TO REWRITE THIS FOR SIM7600", and the last half of the file is commented-out reference code pasted from an Adafruit FONA library and from a SIM900 forum post (that one uses curly quotes, `NewSoftSerial` and CR line endings, and would not compile). The FONA `sendSMS` is the only part worth reading, for the response handling.

## What to carry into the T-Deck project

1. **Check the baud first.** The old setup ran at 9600. Read `AT+IPR?` over the CP2102 and note the answer in [../bring-up-checklist.md](../bring-up-checklist.md) Phase 1. If it is 9600, either keep it for the T-Deck or set 115200 with `AT+IPR=115200` and `AT&W`.
2. **Dial with `ATD+<E.164>;`**, as in the sketch, and add hang-up, answer, `AT+CLCC`, `AT+CLIP=1` and `RING` handling.
3. **SMS:** set `AT+CMGF=1` explicitly, wait for the `> ` prompt, send Ctrl-Z, wait up to 10 s for `+CMGS:` then `OK`.
4. **GNSS** works on this module: `AT+CGPS=1` then `AT+CGNSSINFO`. Optional feature.
5. **Power and control lines were unnecessary** for the old prototype (only 5V, GND, TX, RX). The T-Deck can start the same way and add PWR and RI later.
6. The hardware was wired at 5V/GND/TX/RX from a 3.3V-logic XIAO (the board model is not stated in the sketch; `Serial1` and pin numbers suggest a SAMD21 or ESP32-C3 XIAO). Since that worked, it supports the plan to connect the T-Deck's 3.3V Grove UART straight to the header, but still measure the header voltage first.

## Open questions this raises

- Was the module configured to 9600 by `AT+IPR=9600`, or does the UART JMP path or the board's autobaud explain it?
- Which UART JMP position was used?
- How was the module powered on (button, always on)?
- Was `AT+CMGF=1` / `AT+CGPS=1` set manually, and are those settings saved in the module (`AT&W`)?
- Which XIAO variant was it (pin 13 as LED and `Serial1` imply the SAMD21 version)?
