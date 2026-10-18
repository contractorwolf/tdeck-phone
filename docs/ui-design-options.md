# T-Deck phone UI: options and approach

How to build a lightweight, pleasant user interface for the T-Deck phone: conversation lists, contact lists, file lists, a thread view, a call screen. This is an options document: it lays out the choices, shows real examples, and ends with a recommendation and open decisions. It pairs with [at-library-plan.md](at-library-plan.md) (the UI consumes that library's events) and [next-steps.md](next-steps.md).

Evidence labels: **seen** means I looked at the image or page myself; **reported** means it comes from a search result or documentation I could not verify; **unverified** means my own assumption, to be checked.

## What the hardware gives the UI

- **Screen:** 320 x 240 pixels, landscape, colour, **no touch** (LilyGO wiki).
- **Input:** the built-in QWERTY keyboard (I2C) and a **trackball** with a click. Navigation is keyboard and trackball only, so every screen needs a clear focus model and keyboard shortcuts.
- **Chip:** ESP32-S3 with PSRAM. LilyGO's T-Deck is sold with 16 MB flash and 8 MB PSRAM (unverified for this exact unit; confirm from the board). SD card slot for storage. See [parts/t-deck.md](parts/t-deck.md).
- **Small screen, dense content:** about 10 text rows at a readable size. Lists and thread views are the main design problem.
- **Other things sharing the CPU:** the AT-command library polling the modem, and later audio. The UI must stay responsive while a text send is pending for minutes.

## Real examples (look at these first)

None of these images are copied into this repo; they are linked from their owners' sites, and their licences vary (Meshtastic's device-ui code is GPL-3.0).

### Meshtastic's on-device UI (the closest match to this project)

A touch/keyboard UI built on LVGL, running on the T-Deck. The screenshots below are the project's own docs, rendered in its desktop simulator at 480 x 480, **not** the T-Deck's 320 x 240, but the layout idea transfers directly. I downloaded and viewed these four (dark theme).

| Screen | Image | What it shows (seen) |
|---|---|---|
| Chat list | [mui_chat_list_dark.webp](https://meshtastic.org/img/software/meshtastic-ui/mui_chat_list_dark.webp) | Left icon rail (home, nodes, contacts, chats, map, settings), a coloured title bar with a count ("4 active chat(s)"), one rounded row per chat. The docs say an orange chat is unread |
| Chat thread | [mui_chat_message_dark.webp](https://meshtastic.org/img/software/meshtastic-ui/mui_chat_message_dark.webp) | Bubbles: received left (grey outline, sender and time), sent right (outline colour shows delivery status: green acknowledged, red failed). Text field and keyboard button at the bottom |
| Node list | [mui_node_list_dark.webp](https://meshtastic.org/img/software/meshtastic-ui/mui_node_list_dark.webp) | Two-line rows with a coloured icon, short name, long name, and right-aligned details (signal, battery, "3 min" age). A good model for a contact list |
| Settings | [mui_settings_dark.webp](https://meshtastic.org/img/software/meshtastic-ui/mui_settings_dark.webp) | A plain vertical list of full-width buttons with "Label: value" text, and Settings / Tools tabs at the bottom |

More: the [MUI documentation](https://meshtastic.org/docs/software/meshtastic-ui/) (also covers map and tools screens), and a [photo of the UI running on a real T-Deck](https://raw.githubusercontent.com/meshtastic/device-ui/master/docs/T-Deck.jpg) in the device-ui repo. In that photo (seen) the T-Deck shows a **horizontal icon tab bar across the top** (rather than the left rail) over a node list, which is how it fits the small landscape screen.

Lessons: a persistent tab or rail for the main areas, one row per item with a two-line layout, colour for state (unread, delivery, signal), and very little chrome.

### Other projects worth browsing

| Project | Why look | Notes |
|---|---|---|
| [meshtastic/device-ui](https://github.com/meshtastic/device-ui) | A complete LVGL UI library with a desktop (Linux X11) simulator, MVC-style split, designed in EEZ Studio. Good reference for structure | GPL-3.0; reported to list keyboard navigation as a feature and trackball as not mentioned. Memory is a stated constraint on small boards |
| [Tactility](https://github.com/ByteWelder/Tactility) | An "operating system" for ESP32 with an LVGL launcher and apps, T-Deck and T-Deck Plus are recommended targets, includes a PC simulator | Reported (CNX Software, CircuitDigest, Hackster); I found no screenshots in search, so open the repo to see them |
| [Bruce firmware](https://github.com/BruceDevices/firmware) | Menu-driven launcher style on keyboard devices, supports the T-Deck | I found no menu screenshots in search; check the repo. It is a different domain (security tools) so only the menu pattern is relevant |
| MeshCore "Ripple" firmware, [user guide PDF](https://files.liamcottle.net/MeshCore/Documentation/Ripple_User_Guide.pdf) | Another T-Deck messaging UI | Reported to navigate by tapping four screen quadrants and show a first-boot help wizard; I did not open the PDF |
| [M5Apps](https://gitee.com/yihua-wang/M5Apps) (Cardputer) | A keyboard-driven launcher on a smaller screen with no touch | Reported; the closest to "list + keyboard only" on a tiny display |
| [LilyGO T-Deck repo](https://github.com/Xinyuan-LilyGO/T-Deck) and [quick start](https://wiki.lilygo.cc/products/t-deck-series/t-deck/quick-start) | Official `lvgl_example` and `Keyboard_T_Deck_Master` examples, with the pins and drivers | The wiki says LVGL v8.4.0 with the `LilyGo_LovyanGFX` display flush; reported |

## Graphics stack options

| Option | What it is | Pros | Cons |
|---|---|---|---|
| **A. LVGL** (over LovyanGFX or the ESP LCD API) | The standard embedded UI toolkit: widgets (lists, labels, buttons, text areas), styles, animations, input groups | Rich widgets; keyboard/trackball focus handling built in (`lv_group`); styling and animation; huge community; Meshtastic, Tactility and LilyGO's own example use it; a desktop simulator lets you iterate without flashing | Learning curve and API differences between v8 and v9; RAM use; stock list/table widgets do not virtualise long lists (see below); needs tuning for smoothness |
| **B. LovyanGFX only, own widgets** | Draw text, rects and bitmaps directly, write a small list/thread/keyboard framework yourself | Smallest footprint; total control; very fast; LovyanGFX is described as TFT_eSPI's faster successor with good ESP32-S3 support (reported, author's claim) | You build and maintain scrolling, focus, text input, animations and layout. A lot of work for a delightful UI |
| **C. TFT_eSPI only** | Older popular display library | Familiar, many examples | Reported to be limited on ESP32-S3 (DMA completion handling); LovyanGFX is generally recommended instead; same "build your own widgets" cost as B |
| **D. Adopt a platform** (Tactility, or fork Meshtastic's device-ui) | Reuse an existing launcher/app framework or UI | Fast start for the shell | Taking on someone else's architecture and, for device-ui, GPL-3.0 obligations; both are built around other products |
| **E. Native text UI on a serial-style console** | Terminal-like screen | Trivial | Does not meet the "delightful" goal |

I found no independent benchmark comparing these on the ESP32-S3; the performance claims above come from project docs and forum comments, so any decision about speed should be settled by a quick test on the actual T-Deck.

## Recommendation (to confirm)

**Use LVGL, drawn through LovyanGFX as in LilyGO's example, behind a thin UI layer of our own** (a screen manager, a windowed list component and a theme). Reasons: it gives keyboard/trackball focus handling and styling for free, it is what the closest examples use, and the PC simulator makes designing screens fast. Build custom pieces only where LVGL falls short, mainly long lists. Start on LVGL v8.4 to match LilyGO's working example; revisit v9 later.

Prototype early: a screen with a scrolling list of 200 fake items on the real T-Deck, to measure frame rate and RAM before committing.

## Lists: the core pattern

Conversations, contacts and files are all "a long list of rows, pick one, open it". Build **one reusable list component** and three data sources.

- **Windowed (virtual) rendering:** only create row objects for the rows on screen (plus a couple of spare), and reuse them as the user scrolls, by rebinding data. This keeps RAM flat whether there are 20 or 2000 items. LVGL's stock list and table widgets create every row, and one forum report measured a 600-row `lv_table` drawing at only 6 to 8 fps on an STM32H7, and another poster wanted to keep about 30 of 200 rows alive because of memory (both reported, older LVGL versions). Treat this as the main performance risk.
- **Data source interface:** `count()` and `get(i)` returning a small row struct (title, subtitle, right-aligned detail, icon or colour, unread flag). The list never knows whether rows come from RAM, flash, or the SD card.
- **Keyboard model:** trackball up/down moves focus a row; click opens; a character key jumps to the first item starting with that letter (great for contacts and files); `Esc`/back key pops a screen; a key (say Del) opens an actions menu. Hold-to-accelerate scrolling for long lists.
- **Delight without cost:** short eased scroll (about 100 to 150 ms), a clear focus highlight, unread badge and bold title, relative times ("3 min", "Tue"), coloured initial-circle avatars for contacts, a smooth fade between screens. Avoid heavy blur or large images.

### Conversations (message list)

One row per contact or thread: name, last-message preview (one line, truncated), time, unread count. Sort by last activity. Delivery state on the last message (queued, sending, sent, failed) comes straight from the library's `onSmsSent` events; with a long send timeout, show "sending" optimistically and never block the screen.

### Thread view

A scrolling chat: received left, sent right (as in Meshtastic's screens), time and delivery state per message, a one-line text field at the bottom that grows to two or three lines. Load the most recent 20 to 30 messages and fetch older ones when scrolling up. The T-Deck keyboard types straight into the field; Enter sends, with a visible character count past 160 characters (the library's v1 limit).

### Contacts

Two-line rows (name, number), a letter-jump key, a favourites section on top, and a detail screen with "Call" and "Text" actions. Store on SD or flash as a simple file (see below).

### Files

For browsing the SD card: a breadcrumb path in the header, folders first, then files sorted by name, with an icon by type, size and date on the right, a text preview for small text files, and a delete/rename actions menu. Read directories in pages so a folder with thousands of files does not stall the UI.

## Screens (draft wireframes at 320 x 240)

```
Home / main tabs (top bar, like the T-Deck Meshtastic photo)
+--------------------------------------------------+
| [Msgs 2] [Calls] [Contacts] [Files] [Settings]   | tab bar (18-20 px)
+--------------------------------------------------+
| (current tab content)                            |
|                                                  |
+--------------------------------------------------+
| LTE  ▂▄▆  Ting            12:41         bat 82%  | status bar (16 px)
+--------------------------------------------------+

Conversation list                         Thread
+------------------------------+          +------------------------------+
| Messages                  (2)|          | < Alex            [Call]     |
+------------------------------+          +------------------------------+
| (A) Alex            12:40  2 |          |        hey, running late     |
|     running late, see...     |          |  ok no problem  12:38        |
| (M) Mom              Tue     |          |        thanks  12:40 sent    |
|     Call me when you...      |          |                              |
| (J) Jess             Mon     |          +------------------------------+
+------------------------------+          | Type a message...        [>] |
                                          +------------------------------+

Incoming call                              Active call
+------------------------------+          +------------------------------+
|        Incoming call         |          |  Alex           00:42        |
|           (A) Alex           |          |  [Mute]  [Vol -/+]  [End]    |
|   [Answer]          [Decline]|          |  keypad: 1 2 3 ...           |
+------------------------------+          +------------------------------+
```

Status bar content (signal bars, operator, time, battery) comes from the library's `NetworkState`.

## Architecture

```
 input (keyboard, trackball) -> LVGL input group -> focused widget
 screens (Messages, Thread, Contacts, Files, Call, Settings)
      |  screen manager: a stack (push/pop), one active screen
      |  list component with data sources
      |  theme: colours, fonts, spacing in one place
 view-model / event bus  <---- events from the AT library (SMS, call, network)
 storage: messages and contacts (SD card or flash), settings
```

- **Single-threaded UI loop** (`lv_timer_handler()` plus `Phone::loop()` in `loop()`), at first. The AT library is non-blocking, so this stays smooth. Move to a FreeRTOS task for the UI only if needed.
- **UI never touches AT:** it calls `Phone` methods and reacts to events, nothing more.
- **Storage:** messages as an append-only log per conversation plus a small index; contacts as one file; settings as a small key-value file. Keep it simple and recoverable.
- **Theme in one file:** fonts (pick a clean, small font with the characters needed; custom LVGL bitmap fonts cost flash), a dark theme first (also saves OLED-like glare indoors and reads well), colour tokens for unread, success, error.
- **Memory:** put large buffers (images, message history caches) in PSRAM; keep LVGL's draw buffers in internal RAM so rendering is fast (draw buffer in internal RAM, with partial updates, is the usual setup; tune by measurement).

## Making it feel good (checklist)

- Respond to every key press within one frame; never block on I/O in a handler.
- Partial redraws only; no full-screen refresh on scroll.
- Clear focus indicator; consistent back behaviour.
- Fast first paint at boot, then fill in data.
- Empty states and error states with a plain sentence ("No messages yet", "No signal").
- Short animations (under about 150 ms), none on the critical path of typing.
- Screen timeout and a backlight dimming step to save battery.
- A single visual language: same row height, same header, same icon set.

## Workflow and tooling

- **Simulator first:** LVGL has a PC simulator (SDL); Meshtastic's device-ui and Tactility both ship one. Design and test screens on the Mac at 320 x 240, then flash. Fake the AT library with a mock that emits scripted events (matches the testing plan in [at-library-plan.md](at-library-plan.md)).
- **Screen design tools:** EEZ Studio (used by device-ui) or SquareLine Studio can lay out screens visually; a hand-written code-first approach is also fine for this small set of screens. Decide after the first prototype.
- **Icons:** a small icon set as an LVGL font or compact bitmaps (device-ui credits SVG Repo icons); check each icon's licence.

## Milestones

1. **Hardware spike:** LilyGO's `lvgl_example` running on the T-Deck; confirm keyboard and trackball input and frame rate.
2. **List component:** windowed list with a fake 200-item data source; measure fps and RAM. Decide LVGL stock list vs custom.
3. **Shell:** theme, status bar, tab bar, screen manager, back behaviour.
4. **Conversations and thread:** with the mock library, then the real one.
5. **Contacts:** list, detail, call/text actions, storage.
6. **Call screens:** incoming, active, and dial pad.
7. **Files:** SD browsing with paged directories and previews.
8. **Polish:** animations, empty/error states, power behaviour.
9. **Simulator build** kept working throughout.

## Open decisions

- LVGL v8.4 (matches LilyGO's example) or v9 (newer, API changes).
- Tab bar on top (as in the real T-Deck photo) or a left rail (as in the 480 x 480 simulator shots). The top bar leaves more width on the 320-pixel screen.
- Stock LVGL list for the first prototype, or go straight to a custom windowed list.
- EEZ Studio / SquareLine vs code-only layout.
- Whether to take any code or structure from Meshtastic's device-ui (GPL-3.0 means the combined work must be GPL-3.0; decide the project's licence first). Using it only as a visual and structural reference has no such constraint.
- Font choice and which languages must render.
- Where messages and contacts are stored (SD card vs flash) and the backup story.

## Sources

- [Meshtastic UI docs](https://meshtastic.org/docs/software/meshtastic-ui/); [meshtastic/device-ui](https://github.com/meshtastic/device-ui); [Meshtastic T-Deck page](https://www.meshtastic.org/docs/hardware/devices/tdeck/)
- [LilyGO T-Deck quick start](https://wiki.lilygo.cc/products/t-deck-series/t-deck/quick-start) and [repo](https://github.com/Xinyuan-LilyGO/T-Deck); [espp T-Deck component example](https://components.espressif.com/components/espp/t-deck/versions/1.1.4/examples/example) (ESP-IDF)
- [Tactility on CNX Software](https://cnx-software.com/2025/01/10/tactility-operating-system-for-the-esp32-microcontroller-family-supports-built-in-and-external-applications), [CircuitDigest](https://circuitdigest.com/news/tactility-brings-app-based-functionality-to-esp32-microcontrollers), [Hackster](https://hackster.io/news/ken-van-hoeylandt-s-tactility-is-an-os-for-your-espressif-esp32-complete-with-graphical-interface-6b2fa806a2e3)
- [LovyanGFX](https://github.com/lovyan03/LovyanGFX) and [docs](https://lovyangfx.readthedocs.io/en/latest/01_intro.html)
- LVGL forum: [lists using a lot of memory](https://forum.lvgl.io/t/how-to-avoid-crashes-because-lists-use-a-lot-of-memory/4117), [600-row table performance](https://forum.lvgl.io/t/lvgl-9-3-large-table-virtual-row-performance-optimization-help-600-rows/23113)
- [Bruce firmware](https://github.com/BruceDevices/firmware); [M5Apps launcher](https://gitee.com/yihua-wang/M5Apps); [MeshCore Ripple guide](https://files.liamcottle.net/MeshCore/Documentation/Ripple_User_Guide.pdf); [Jeff Geerling on the T-Deck](https://www.jeffgeerling.com/blog/2024/realizing-meshtastics-promise-t-deck/)
