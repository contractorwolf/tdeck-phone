# LVGL on the T-Deck: research for the mock phone UI

Goal: a flashable app with a homescreen and mock Messages, Calls, Contacts and Settings screens. No modem connected yet. Keep it simple.

## Recommendation

- Arduino framework built and flashed with `arduino-cli` (and the Arduino MCP when it connects; it failed to connect in this session), LovyanGFX for the display, LVGL for the UI. No PlatformIO. This matches what `ui-design-options.md` already leans toward.
- Start with LVGL 9.3. Fall back to 8.4 (LilyGO's tested version) only if 9.3 fights the Arduino core. Meshtastic device-ui runs LVGL 9.3.0 on the T-Deck, so v9 is viable.
- Skip the desktop simulator for now. Flash straight to the device.
- UI code talks to a tiny `Phone` interface. Today it is a mock with hard-coded data. Later it is the AT library from `at-library-plan.md`.

## Hardware facts that matter for the UI

- Display: 320x240 ST7789, 16-bit colour, driven by LovyanGFX. SPI MOSI 41, MISO 38, SCK 40, CS 12, DC 11, backlight 42. The SPI bus is shared with SD and LoRa.
- No touch. Input is the keyboard (separate ESP32-C3, I2C address 0x55, SDA 18, SCL 8, interrupt GPIO46) and the trackball.
- Trackball pins: G01=3, G02=2, G03=15, G04=1, click on GPIO0 (BOOT). Which pin is which direction is not documented in our reference header, so find out on the device.
- GPIO10 (`BOARD_POWERON`) must be HIGH at boot or peripherals are off on battery.
- GPIO0 stops working as the click button while the microphone is active.
- Memory: 8MB OPI PSRAM, so buffers and screens are not a concern at this size.

## Versions

- LilyGO's example pins LVGL 8.4.0 and arduino-esp32 2.0.14, and says newer cores may break TFT_eSPI (their `LilyGo_T_Deck` object uses LovyanGFX).
- This machine has `esp32:esp32` core 3.3.0, LovyanGFX 1.1.16 and LVGL 8.3.0-dev. The LVGL is old and the core is much newer than LilyGO's. Install LVGL 9.3.x with `arduino-cli lib install lvgl@9.3.0`. Whether to also install core 2.0.14 alongside 3.3.0 is decided by whether the first build works on 3.3.0.
- arduino-cli board settings from LilyGO's Arduino IDE list: board `esp32:esp32:esp32s3`, `USBMode=hwcdc`, `CDCOnBoot=cdc`, `FlashSize=16M`, `PSRAM=opi`, `PartitionScheme=default_16MB` (check the exact option names with `arduino-cli board details -b esp32:esp32:esp32s3`).
- Meshtastic device-ui depends on `lvgl/lvgl` v9.3.0.
- Not verified: LVGL 9.3 plus LovyanGFX on the core you end up using. That is the first thing to prove on hardware.
- Our modem link uses serial pads 43/44, which are also the ESP32-S3 UART0 defaults. Use a different UART or disable console logging there. USB CDC on boot already moves the console off those pins.

## LVGL concepts we need

- Display: create it with `lv_display_create`, use partial render mode with two buffers of about one tenth of the screen, allocated in PSRAM. Forum reports show full-screen double buffers run slowly on ESP32-S3. The flush callback pushes pixels with LovyanGFX and then calls `lv_display_flush_ready`.
- Tick and loop: give LVGL a millisecond tick (`lv_tick_set_cb(millis)`) and call `lv_timer_handler()` from `loop()`. Single-threaded, so no locking is needed.
- Input: one keypad input device (`LV_INDEV_TYPE_KEYPAD`). The read callback returns an `LV_KEY_*` code and pressed/released. Map trackball up/down/left/right to `LV_KEY_UP/DOWN/LEFT/RIGHT`, click to `LV_KEY_ENTER`, keyboard Backspace or Esc to `LV_KEY_ESC`, and typed characters straight through for text areas.
- Focus: put every interactive widget into an `lv_group` and assign it to the input device. The focused widget gets the keys and the `LV_STATE_FOCUSED` style, which is the highlight the user sees. Make one group per screen so focus resets cleanly.
- Screens: one `lv_obj` screen per app, switched with `lv_screen_load`. Esc returns to home. Avoid tileview and swipe navigation, since there is no touch. The third-party `lvgl_nav_kit` component adds a page stack but is not needed for four screens.
- Lists: `lv_list` is enough for mock messages and contacts. Real message history later needs a windowed list. That is already noted in `ui-design-options.md`.
- Theme: one small theme or style file for colours and fonts. Check that the built-in Montserrat font sizes enabled in `lv_conf.h` are readable at 320x240.

## Suggested mock screens

- Home: status bar (fake signal, battery, time) and a grid of 4 to 6 big icons or labelled buttons: Messages, Calls, Contacts, Settings.
- Messages: conversation list, thread view, compose box.
- Calls: dial pad and recent calls list.
- Contacts: list with a detail view.
- Settings: simple list of fake toggles.

## Open questions

- LVGL 9.3 or 8.4 in practice (decided by the first hardware test).
- Trackball direction pin mapping and whether keyboard key codes match the LilyGO `Keyboard_T_Deck_Master` example.
- Whether to add an SDL desktop simulator later. It needs `brew install sdl2 cmake`, and LVGL's `lv_port_pc_vscode` project is the usual starting point. It is the main benefit once screens are numerous.

## Sources

- [LilyGO T-Deck quick start](https://wiki.lilygo.cc/products/t-deck-series/t-deck/quick-start)
- [LilyGO T-Deck repo](https://github.com/Xinyuan-LilyGO/T-Deck) (`lvgl_example`, `Keyboard_T_Deck_Master`, `utilities.h`)
- [Meshtastic device-ui](https://github.com/meshtastic/device-ui) (LVGL 9.3.0 on T-Deck)
- [LVGL keypad and encoder docs](https://lvgl.io/docs/open/9.3/details/main-modules/indev)
- [LVGL forum: ESP32-S3 with LovyanGFX double buffer](https://forum.lvgl.io/t/esp32s3-with-lovyangfx-double-buffer-with-direct-render-mode/17929)
- [LVGL tileview docs](https://docs.lvgl.io/9.3/details/widgets/tileview.html)
- [lvgl_nav_kit](https://components.espressif.com/components/txp666/lvgl-nav-kit)
- [lv_port_pc_vscode](https://github.com/lvgl/lv_port_pc_vscode)
