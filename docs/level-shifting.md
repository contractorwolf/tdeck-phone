# Level shifting and control circuits

The module is 1.8V logic (SIMCom hardware design p.34: "The MODULE UART is 1.8V voltage interface... level shifter circuits should be used"). The T-Deck ESP32-S3 is 3.3V logic with VIH = 0.75 x 3.3V = 2.475V.

## Do not connect directly

- 3.3V from the T-Deck into module pad 68 (RXD) exceeds the 2.0V input maximum and can damage the module.
- 1.8V from module pad 71 (TXD) into the T-Deck RX is below 2.475V and will read unreliably.

## 1. T-Deck TX (3.3V) to module RXD (1.8V): resistor divider

```
T-Deck GPIO43 (TX) ---[ R1 1.0k ]---+--- module pad 68 (RXD)
                                    |
                                 [ R2 1.2k ]
                                    |
                                   GND
```

Vout = 3.3 x R2 / (R1 + R2) = 3.3 x 1.2 / 2.2 = 1.80V. Fine at 115200 baud. (2.2k / 2.7k gives 1.82V with less loading if preferred.)

## 2. Module TXD (1.8V) to T-Deck RX (3.3V): level shifter

A divider cannot step a voltage up, so use one of these. Option A is the simplest to build.

A. BSS138 N-MOSFET shifter for this one line (the standard bidirectional circuit, which also works for RI, STATUS, etc.):

```
 1.8V (module VDD_1V8, pad 15)            3.3V (T-Deck 3V3)
        |                                       |
      [10k]                                   [10k]
        |                                       |
        +---- Source        BSS138      Drain --+---- T-Deck GPIO44 (RX)
 module TXD (pad 71)          Gate to 1.8V rail
```

Source connects to the 1.8V-side signal, Drain to the 3.3V-side signal, Gate to the 1.8V supply. Each side has a 10k pull-up to its own supply.

B. One IC: 74AUP1T45 / SN74AVC2T245 style translator (module side 1.8V from VDD_1V8, T-Deck side 3.3V) or the TXB0104 that SIMCom's reference design uses (TXB0108 in the hardware design, Figure 15). Add 100nF on each supply. Note that the TXB family can misbehave on lines with weak pull-ups; fine for UART.

Check VDD_1V8 (pad 15) can supply this: it is rated 50mA max, and the shifter draws microamps.

## 3. PWRKEY and RESET: open-drain driver only

**Note for the Waveshare V2 (B):** PWRKEY (pad 3) is tied to ground through R26 (0Ω), so the module powers on by itself and this driver is not needed unless you remove R26. Never drive pad 3 high either way.

PWRKEY (pad 3) and RESET (pad 4) are pulled up to 1.8V inside the module. Never drive them high from 3.3V. Pull them low with a transistor:

```
T-Deck GPIO (e.g. 1) ---[ 1k ]---- Base  NPN (2N3904 / MMBT3904)
                                   Collector ---- module pad 3 (PWRKEY)   (or pad 4 for RESET)
                                   Emitter   ---- GND
         [ 100k from base to GND keeps it off at boot ]
```

- Power on: GPIO high for 100-500ms (the transistor pulls PWRKEY low), then low again. STATUS (pad 49) goes high about 15s later.
- Power off: hold ~2.5s, or send `AT+CPOF`.
- RESET: hold 100-500ms.
- Use the same BSS138 shifter in the 1.8V to 3.3V direction for STATUS (pad 49) and RI (pad 69).

## 4. Power and ground

- Common ground between the T-Deck and the HAT is mandatory. Connect it at the module GND pad nearest the UART pads, and at the HAT 5V/GND pads.
- Power the HAT from its own 5V supply or USB, not from the T-Deck.
- Keep the dead-bug UART wires short and twisted with ground. Do not leave the PWRKEY wire floating while the module is off.

## 5. Bill of materials

- 1x 1.0k and 1x 1.2k resistor (divider, TX direction).
- 1x BSS138 (or a BSS138 breakout), 2x 10k pull-ups (RX direction). More for RI/STATUS.
- 1-2x MMBT3904 / 2N3904 plus 1k and 100k resistors (PWRKEY, RESET).
- Optional: TXB0104/TXB0108 breakout, 100nF capacitors.
- 30 AWG wire-wrap wire, flux, fine soldering tip.
