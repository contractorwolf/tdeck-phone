// phone.ino - a mock phone interface for the LilyGO T-Deck, drawn with LVGL.
//
// Everything on screen is FAKE data. Nothing talks to a modem yet. The goal is to
// get the screens, navigation and keyboard input working first.
//
// Input: the touch screen (tap buttons and rows; tap the title bar to go home, swipe right to go back) and the
// T-Deck keyboard. Keyboard keys:
//   w      move to the previous item
//   s      move to the next item
//   Enter  open / select the highlighted item
//   q      go back to the home screen
//   0-9 * # +  type into the dial pad (Calls screen)
//   Backspace  delete a digit (Calls screen)
//
// How LVGL works, in one paragraph: you create "objects" (screens, buttons, labels...),
// give them styles, and call lv_timer_handler() over and over from loop(). LVGL draws
// into a small memory buffer and calls OUR flush_cb() to copy the pixels to the real
// display. Keyboard keys reach buttons through a "group": the group knows which object
// is focused (highlighted), and an input device feeds keys into that group.

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <Wire.h>
#include <lvgl.h>

// ---------------------------------------------------------------------------
// Board constants (pins from docs/reference/LilyGO_T-Deck_utilities.h)
// ---------------------------------------------------------------------------
constexpr int BOARD_POWERON = 10;       // must be HIGH or the peripherals stay off on battery
constexpr int KEYBOARD_I2C_ADDR = 0x55;  // the keyboard is its own chip, we ask it for keys
constexpr int TOUCH_I2C_ADDR = 0x5D;     // GT911 touch chip, on the same I2C bus as the keyboard
constexpr uint32_t RELEASE_DELAY_MS = 120;  // how long with no finger counts as 'lifted'
constexpr int I2C_SDA = 18;
constexpr int I2C_SCL = 8;

constexpr int SCREEN_W = 320;
constexpr int SCREEN_H = 240;
constexpr int STATUS_BAR_H = 24;  // top strip: title, clock, signal, battery

// ---------------------------------------------------------------------------
// Display driver (LovyanGFX). This is the same setup that worked in hello.ino.
// ---------------------------------------------------------------------------
class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ST7789 panel_;
  lgfx::Bus_SPI bus_;
  lgfx::Light_PWM light_;

 public:
  LGFX() {
    auto b = bus_.config();
    b.spi_host = SPI2_HOST;
    b.spi_mode = 0;
    b.freq_write = 40000000;
    b.pin_sclk = 40;
    b.pin_mosi = 41;
    b.pin_miso = 38;
    b.pin_dc = 11;
    bus_.config(b);
    panel_.setBus(&bus_);

    auto p = panel_.config();
    p.pin_cs = 12;
    p.pin_rst = -1;
    p.panel_width = 240;
    p.panel_height = 320;
    p.invert = true;
    p.readable = false;
    panel_.config(p);

    auto l = light_.config();
    l.pin_bl = 42;
    light_.config(l);
    panel_.setLight(&light_);

    setPanel(&panel_);
  }
};

LGFX gfx;

// LVGL calls this when a rectangle of pixels is ready. We copy it to the display.
static void flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px) {
  const int w = area->x2 - area->x1 + 1;
  const int h = area->y2 - area->y1 + 1;
  gfx.startWrite();
  gfx.setAddrWindow(area->x1, area->y1, w, h);
  gfx.writePixels(reinterpret_cast<lgfx::rgb565_t *>(px), w * h);
  gfx.endWrite();
  lv_display_flush_ready(disp);  // tell LVGL we are done with the buffer
}

// ---------------------------------------------------------------------------
// Fake data. Later this will come from the modem; for now it is just text.
// ---------------------------------------------------------------------------
struct Contact {
  const char *name;
  const char *number;
};
const Contact CONTACTS[] = {
    {"Alice Johnson", "555-0101"}, {"Bob Smith", "555-0102"}, {"Carol Diaz", "555-0103"},
    {"Dave Lee", "555-0104"},      {"Emma Wilson", "555-0105"},
};
constexpr int CONTACT_COUNT = sizeof(CONTACTS) / sizeof(CONTACTS[0]);

struct Thread {
  const char *name;
  const char *last_message;  // the last thing they said to us
};
const Thread THREADS[] = {
    {"Alice", "Are we still on for lunch?"},
    {"Bob", "Got the files, thanks!"},
    {"Carol", "Call me when you can"},
    {"Dave", "See you tomorrow"},
};
constexpr int THREAD_COUNT = sizeof(THREADS) / sizeof(THREADS[0]);

const char *const SETTINGS_ROWS[] = {"Wi-Fi", "Sound", "Vibrate", "Airplane mode"};
constexpr int SETTINGS_COUNT = sizeof(SETTINGS_ROWS) / sizeof(SETTINGS_ROWS[0]);

#define APP_VERSION "v0.0.1"
#define HOME_TITLE "Wolf OpenPhone " APP_VERSION  // shown in the home screen status bar
#define FAKE_TIME "12:34"  // a macro so it can be joined to other strings at compile time

// ---------------------------------------------------------------------------
// Navigation. We always show exactly one screen, and rebuild it when we switch.
// ---------------------------------------------------------------------------
static lv_indev_t *keyboard_indev = nullptr;  // the keyboard as an LVGL input device
static lv_group_t *group = nullptr;           // which objects the keyboard can focus
static lv_obj_t *current_screen = nullptr;    // the screen being built / shown

// Which thread / contact the user picked (read by the detail screens).
static int selected_index = 0;

// A "screen builder" is a function that builds one screen: void name().
// (We write the pointer type out, 'void (*)()', instead of using a typedef, because
// the Arduino build inserts function prototypes above any typedef we write.)

// go_to() does NOT build the screen right away. It just remembers which one we want.
// loop() builds it a moment later. Why? Because go_to() is often called from inside a
// button's click handler, and deleting a screen while one of its buttons is still running
// code is a classic way to crash. Waiting until loop() is the simple, safe way.
static void (*pending_screen)() = nullptr;
static void go_to(void (*builder)()) { pending_screen = builder; }

// Forward declarations so screens can open each other.
static void show_home();
static void show_messages();
static void show_thread();
static void show_calls();
static void show_contacts();
static void show_contact();
static void show_settings();

// ---------------------------------------------------------------------------
// Keyboard input
// ---------------------------------------------------------------------------

// Ask the keyboard chip for one key. Returns 0 if nothing is pressed.
static uint8_t read_keyboard_byte() {
  if (Wire.requestFrom(KEYBOARD_I2C_ADDR, 1) == 1) {
    return Wire.read();
  }
  return 0;
}

// LVGL calls this regularly to ask "is a key pressed right now?".
// The keyboard only tells us about a press (never a release), so we report
// "pressed" once and then "released" on the next call.
static void keyboard_read_cb(lv_indev_t *indev, lv_indev_data_t *data) {
  static uint32_t last_key = 0;
  static bool release_next = false;

  data->key = last_key;
  data->state = LV_INDEV_STATE_RELEASED;

  if (release_next) {  // second half of a key press: send the release
    release_next = false;
    return;
  }

  const uint8_t c = read_keyboard_byte();
  if (c == 0) return;  // no key
  Serial.printf("key 0x%02X\n", c);  // handy for checking what the keyboard sends

  uint32_t key;
  switch (c) {
    case 'q':  // our own "back" key: go home and do not pass it to LVGL
      go_to(show_home);
      return;
    case 'w': key = LV_KEY_PREV; break;
    case 's': key = LV_KEY_NEXT; break;
    case '\r':
    case '\n': key = LV_KEY_ENTER; break;
    default: key = c; break;  // digits, Backspace (0x08) etc. pass straight through
  }

  last_key = key;
  data->key = key;
  data->state = LV_INDEV_STATE_PRESSED;
  release_next = true;
}

// ---------------------------------------------------------------------------
// Touch input (GT911 chip). We read it with the same Wire bus as the keyboard.
// ---------------------------------------------------------------------------

// The GT911 stores its data in registers with 16-bit addresses. This reads 'len' bytes
// starting at register 'reg'. Returns true if the chip answered.
static bool touch_read_regs(uint16_t reg, uint8_t *buf, size_t len) {
  Wire.beginTransmission(TOUCH_I2C_ADDR);
  Wire.write(reg >> 8);    // register address, high byte first
  Wire.write(reg & 0xFF);
  if (Wire.endTransmission() != 0) return false;
  if (Wire.requestFrom(TOUCH_I2C_ADDR, (int)len) != (int)len) return false;
  for (size_t i = 0; i < len; i++) buf[i] = Wire.read();
  return true;
}

// After reading a touch report we must write 0 to the status register, otherwise the
// chip never sends the next one.
static void touch_clear_status() {
  Wire.beginTransmission(TOUCH_I2C_ADDR);
  Wire.write(0x81);
  Wire.write(0x4E);
  Wire.write(0x00);
  Wire.endTransmission();
}

// LVGL calls this regularly to ask "where is the finger?".
static void touch_read_cb(lv_indev_t *indev, lv_indev_data_t *data) {
  // Remember the last answer: the chip only reports when something CHANGED.
  static int32_t last_x = 0, last_y = 0;
  static uint32_t last_down_ms = 0;  // when we last saw a finger
  bool pressed = false;

  uint8_t status;
  if (touch_read_regs(0x814E, &status, 1) && (status & 0x80)) {  // bit 7 = new report ready
    const uint8_t points = status & 0x0F;                        // low bits = finger count
    uint8_t p[4];
    if (points > 0 && touch_read_regs(0x8150, p, 4)) {
      // Register 0x8150 holds x (2 bytes, low byte first) then y (2 bytes).
      // The chip reports in the panel's own PORTRAIT shape: x is 0-239, y is 0-319.
      const int raw_x = p[0] | (p[1] << 8);
      const int raw_y = p[2] | (p[3] << 8);
      // Our screen is landscape (320 wide, 240 tall) but the chip is portrait, so swap the
      // axes. This mapping was measured by tapping targets (see firmware/touchcal).
      last_x = constrain(raw_y, 0, SCREEN_W - 1);
      last_y = constrain(SCREEN_H - 1 - raw_x, 0, SCREEN_H - 1);
      if (millis() - last_down_ms > RELEASE_DELAY_MS) {  // log only the start of each touch
        Serial.printf("touch down raw=(%d,%d) screen=(%d,%d)\n", raw_x, raw_y, (int)last_x, (int)last_y);
      }
      last_down_ms = millis();
    }
    touch_clear_status();
  }

  // The chip sometimes skips a frame while a finger is down, so only call it a release
  // after RELEASE_DELAY_MS with no finger. Otherwise one tap could count as several.
  pressed = last_down_ms != 0 && (millis() - last_down_ms) < RELEASE_DELAY_MS;

  data->point.x = last_x;
  data->point.y = last_y;
  data->state = pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

// ---------------------------------------------------------------------------
// Screen-building helpers shared by every screen
// ---------------------------------------------------------------------------

// Tapping the status bar goes back to the home screen (the touch version of 'q').
static void on_bar_clicked(lv_event_t *e) { go_to(show_home); }

// Each screen says which screen is "behind" it (nullptr for home, which has nothing behind it).
static void (*current_back)() = nullptr;

// A swipe to the right anywhere on a screen goes back one screen.
// LVGL turns finger movement into a gesture event and asks the input device which way it went.
static void on_screen_gesture(lv_event_t *e) {
  if (current_back && lv_indev_get_gesture_dir(lv_indev_active()) == LV_DIR_RIGHT) go_to(current_back);
}

// Start a new screen ('back' = the screen a right swipe returns to): makes the screen object, a fresh focus group, and the status bar.
// Returns the empty "content" area where the caller adds its own widgets.
static lv_obj_t *new_screen(const char *title, void (*back)()) {
  current_back = back;
  current_screen = lv_obj_create(nullptr);  // nullptr parent = a new top-level screen
  lv_obj_remove_flag(current_screen, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_event_cb(current_screen, on_screen_gesture, LV_EVENT_GESTURE, nullptr);

  // Each screen gets its own focus group, so focus always starts on its first widget.
  if (group) lv_group_delete(group);
  group = lv_group_create();
  lv_indev_set_group(keyboard_indev, group);

  // Status bar: title on the left, clock + signal + battery on the right.
  lv_obj_t *bar = lv_obj_create(current_screen);
  lv_obj_set_size(bar, SCREEN_W, STATUS_BAR_H);
  lv_obj_align(bar, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_set_style_bg_color(bar, lv_palette_darken(LV_PALETTE_BLUE_GREY, 3), 0);
  lv_obj_set_style_radius(bar, 0, 0);
  lv_obj_set_style_border_width(bar, 0, 0);
  lv_obj_set_style_pad_all(bar, 4, 0);
  lv_obj_remove_flag(bar, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_event_cb(bar, on_bar_clicked, LV_EVENT_CLICKED, nullptr);  // tap the bar = home

  lv_obj_t *title_label = lv_label_create(bar);
  lv_label_set_text(title_label, title);
  lv_obj_align(title_label, LV_ALIGN_LEFT_MID, 0, 0);

  lv_obj_t *status_label = lv_label_create(bar);
  lv_label_set_text(status_label, FAKE_TIME "  " LV_SYMBOL_WIFI "  " LV_SYMBOL_BATTERY_3);
  lv_obj_align(status_label, LV_ALIGN_RIGHT_MID, 0, 0);

  // The content area fills everything below the status bar.
  lv_obj_t *content = lv_obj_create(current_screen);
  lv_obj_set_size(content, SCREEN_W, SCREEN_H - STATUS_BAR_H);
  lv_obj_align(content, LV_ALIGN_TOP_MID, 0, STATUS_BAR_H);
  lv_obj_set_style_bg_opa(content, LV_OPA_TRANSP, 0);  // see-through
  lv_obj_set_style_border_width(content, 0, 0);
  lv_obj_set_style_pad_all(content, 4, 0);
  return content;
}

// Finish a screen: show it on the display. true = delete the old screen to free memory.
static void load_screen() {
  lv_screen_load_anim(current_screen, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
}

// Make a scrolling list that fills the content area. Its buttons are added to the
// focus group by add_list_item() below.
static lv_obj_t *new_list(lv_obj_t *content) {
  lv_obj_t *list = lv_list_create(content);
  lv_obj_set_size(list, lv_pct(100), lv_pct(100));
  return list;
}

// Add one row to a list and make it keyboard-focusable. Returns the row button so the
// caller can attach a click handler.
static lv_obj_t *add_list_item(lv_obj_t *list, const char *icon, const char *text) {
  lv_obj_t *btn = lv_list_add_button(list, icon, text);
  lv_group_add_obj(group, btn);
  return btn;
}

// ---------------------------------------------------------------------------
// Home screen: four big buttons in a 2x2 grid
// ---------------------------------------------------------------------------
struct HomeIcon {
  const char *symbol;  // built-in LVGL icon (a special string)
  const char *label;
  void (*target)();  // screen builder opened by this button
};
const HomeIcon HOME_ICONS[] = {
    {LV_SYMBOL_ENVELOPE, "Messages", show_messages},
    {LV_SYMBOL_CALL, "Calls", show_calls},
    {LV_SYMBOL_LIST, "Contacts", show_contacts},
    {LV_SYMBOL_SETTINGS, "Settings", show_settings},
};

// Click handler for a home button. user_data holds the button's index in HOME_ICONS.
static void on_home_icon_clicked(lv_event_t *e) {
  const int i = (int)(intptr_t)lv_event_get_user_data(e);
  go_to(HOME_ICONS[i].target);
}

static void show_home() {
  lv_obj_t *content = new_screen(HOME_TITLE, nullptr);

  // A "flex" container lays children out in a row and wraps to the next row.
  lv_obj_set_flex_flow(content, LV_FLEX_FLOW_ROW_WRAP);
  lv_obj_set_flex_align(content, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_SPACE_EVENLY,
                        LV_FLEX_ALIGN_SPACE_EVENLY);

  for (int i = 0; i < 4; i++) {
    lv_obj_t *btn = lv_button_create(content);
    lv_obj_set_size(btn, 140, 80);
    lv_obj_set_flex_flow(btn, LV_FLEX_FLOW_COLUMN);  // icon above the text
    lv_obj_set_flex_align(btn, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *icon = lv_label_create(btn);
    lv_label_set_text(icon, HOME_ICONS[i].symbol);
    lv_obj_set_style_text_font(icon, &lv_font_montserrat_20, 0);

    lv_obj_t *text = lv_label_create(btn);
    lv_label_set_text(text, HOME_ICONS[i].label);

    lv_group_add_obj(group, btn);  // lets the keyboard focus this button
    lv_obj_add_event_cb(btn, on_home_icon_clicked, LV_EVENT_CLICKED, (void *)(intptr_t)i);
  }
  load_screen();
}

// ---------------------------------------------------------------------------
// Messages: a list of conversations, then a simple thread view
// ---------------------------------------------------------------------------
static void on_thread_clicked(lv_event_t *e) {
  selected_index = (int)(intptr_t)lv_event_get_user_data(e);
  go_to(show_thread);
}

static void show_messages() {
  lv_obj_t *content = new_screen("Messages", show_home);
  lv_obj_t *list = new_list(content);

  for (int i = 0; i < THREAD_COUNT; i++) {
    // Build "Name: last message" as one line of text.
    char line[80];
    snprintf(line, sizeof(line), "%s: %s", THREADS[i].name, THREADS[i].last_message);
    lv_obj_t *row = add_list_item(list, LV_SYMBOL_ENVELOPE, line);
    lv_obj_add_event_cb(row, on_thread_clicked, LV_EVENT_CLICKED, (void *)(intptr_t)i);
  }
  load_screen();
}

// One chat bubble. mine = true puts it on the right in blue, otherwise left in grey.
static void add_bubble(lv_obj_t *parent, const char *text, bool mine) {
  lv_obj_t *bubble = lv_label_create(parent);
  lv_label_set_text(bubble, text);
  lv_obj_set_width(bubble, 220);
  lv_label_set_long_mode(bubble, LV_LABEL_LONG_WRAP);
  lv_obj_set_style_bg_opa(bubble, LV_OPA_COVER, 0);
  lv_obj_set_style_bg_color(bubble, mine ? lv_palette_main(LV_PALETTE_BLUE) : lv_palette_darken(LV_PALETTE_GREY, 2), 0);
  lv_obj_set_style_radius(bubble, 8, 0);
  lv_obj_set_style_pad_all(bubble, 6, 0);
  lv_obj_set_align(bubble, mine ? LV_ALIGN_RIGHT_MID : LV_ALIGN_LEFT_MID);
}

static void show_thread() {
  const Thread &t = THREADS[selected_index];
  lv_obj_t *content = new_screen(t.name, show_messages);

  // Stack the bubbles top to bottom.
  lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_pad_row(content, 8, 0);

  add_bubble(content, t.last_message, false);
  add_bubble(content, "OK, sounds good!", true);
  add_bubble(content, "Great, talk soon.", false);
  load_screen();
}

// ---------------------------------------------------------------------------
// Calls: a number box. Type digits, press Enter to "call" (fake).
// ---------------------------------------------------------------------------

// Runs when the user presses Enter inside the text box.
static void on_dial_ready(lv_event_t *e) {
  lv_obj_t *box = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t *status = (lv_obj_t *)lv_event_get_user_data(e);
  const char *number = lv_textarea_get_text(box);
  if (number[0] == '\0') {
    lv_label_set_text(status, "Type a number first");
  } else {
    lv_label_set_text_fmt(status, LV_SYMBOL_CALL " Calling %s ...\n(no modem yet)", number);
  }
}

static void show_calls() {
  lv_obj_t *content = new_screen("Calls", show_home);
  lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_pad_row(content, 12, 0);

  lv_obj_t *box = lv_textarea_create(content);
  lv_textarea_set_one_line(box, true);
  lv_textarea_set_placeholder_text(box, "Type a number");
  lv_textarea_set_accepted_chars(box, "0123456789*#+");  // ignore letters
  lv_textarea_set_max_length(box, 20);
  lv_obj_set_width(box, lv_pct(100));
  lv_obj_set_style_text_font(box, &lv_font_montserrat_20, 0);
  lv_group_add_obj(group, box);

  lv_obj_t *status = lv_label_create(content);
  lv_label_set_text(status, "");

  lv_obj_add_event_cb(box, on_dial_ready, LV_EVENT_READY, status);  // READY = Enter pressed
  load_screen();
}

// ---------------------------------------------------------------------------
// Contacts: a list of names, then a detail screen
// ---------------------------------------------------------------------------
static void on_contact_clicked(lv_event_t *e) {
  selected_index = (int)(intptr_t)lv_event_get_user_data(e);
  go_to(show_contact);
}

static void show_contacts() {
  lv_obj_t *content = new_screen("Contacts", show_home);
  lv_obj_t *list = new_list(content);

  for (int i = 0; i < CONTACT_COUNT; i++) {
    lv_obj_t *row = add_list_item(list, LV_SYMBOL_LIST, CONTACTS[i].name);
    lv_obj_add_event_cb(row, on_contact_clicked, LV_EVENT_CLICKED, (void *)(intptr_t)i);
  }
  load_screen();
}

static void show_contact() {
  const Contact &c = CONTACTS[selected_index];
  lv_obj_t *content = new_screen("Contact", show_contacts);
  lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(content, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_row(content, 10, 0);

  lv_obj_t *name = lv_label_create(content);
  lv_label_set_text(name, c.name);
  lv_obj_set_style_text_font(name, &lv_font_montserrat_20, 0);

  lv_obj_t *number = lv_label_create(content);
  lv_label_set_text_fmt(number, LV_SYMBOL_CALL " %s", c.number);
  load_screen();
}

// ---------------------------------------------------------------------------
// Settings: rows with on/off switches. The switches are fake, they only toggle visually.
// ---------------------------------------------------------------------------
static void on_setting_clicked(lv_event_t *e) {
  lv_obj_t *sw = (lv_obj_t *)lv_event_get_user_data(e);  // the switch inside this row
  if (lv_obj_has_state(sw, LV_STATE_CHECKED)) {
    lv_obj_remove_state(sw, LV_STATE_CHECKED);
  } else {
    lv_obj_add_state(sw, LV_STATE_CHECKED);
  }
}

static void show_settings() {
  lv_obj_t *content = new_screen("Settings", show_home);
  lv_obj_t *list = new_list(content);

  for (int i = 0; i < SETTINGS_COUNT; i++) {
    lv_obj_t *row = add_list_item(list, LV_SYMBOL_SETTINGS, SETTINGS_ROWS[i]);

    lv_obj_t *sw = lv_switch_create(row);  // put a switch inside the row
    lv_obj_set_size(sw, 40, 22);
    lv_obj_remove_flag(sw, LV_OBJ_FLAG_CLICKABLE);  // the ROW is what gets clicked
    if (i < 2) lv_obj_add_state(sw, LV_STATE_CHECKED);  // Wi-Fi and Sound start on

    lv_obj_add_event_cb(row, on_setting_clicked, LV_EVENT_CLICKED, sw);
  }
  load_screen();
}

// ---------------------------------------------------------------------------
// Arduino entry points
// ---------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  pinMode(BOARD_POWERON, OUTPUT);
  digitalWrite(BOARD_POWERON, HIGH);  // turn on power to display, keyboard, etc.
  delay(100);

  Wire.begin(I2C_SDA, I2C_SCL);  // I2C bus shared with the keyboard chip

  gfx.init();
  gfx.setRotation(1);  // landscape, 320x240
  gfx.setBrightness(200);

  lv_init();
  lv_tick_set_cb([]() -> uint32_t { return millis(); });  // LVGL needs a clock in ms

  // Two drawing buffers, each 1/10 of the screen, in the big PSRAM chip.
  // LVGL draws the screen in strips and calls flush_cb for each strip.
  constexpr size_t buf_bytes = SCREEN_W * SCREEN_H / 10 * sizeof(uint16_t);
  void *buf1 = heap_caps_malloc(buf_bytes, MALLOC_CAP_SPIRAM);
  void *buf2 = heap_caps_malloc(buf_bytes, MALLOC_CAP_SPIRAM);
  lv_display_t *disp = lv_display_create(SCREEN_W, SCREEN_H);
  lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565);
  lv_display_set_flush_cb(disp, flush_cb);
  lv_display_set_buffers(disp, buf1, buf2, buf_bytes, LV_DISPLAY_RENDER_MODE_PARTIAL);

  // Dark theme: dark backgrounds, light text, blue highlights.
  lv_theme_t *theme = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE),
                                            lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
  lv_display_set_theme(disp, theme);

  // Register the keyboard as an LVGL input device of type "keypad".
  keyboard_indev = lv_indev_create();
  lv_indev_set_type(keyboard_indev, LV_INDEV_TYPE_KEYPAD);
  lv_indev_set_read_cb(keyboard_indev, keyboard_read_cb);

  // Register the touch screen as a second input device of type "pointer".
  lv_indev_t *touch_indev = lv_indev_create();
  lv_indev_set_type(touch_indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(touch_indev, touch_read_cb);

  show_home();
}

void loop() {
  // If a button asked for a different screen, build it now (see go_to() above).
  if (pending_screen) {
    void (*builder)() = pending_screen;
    pending_screen = nullptr;
    builder();
  }
  lv_timer_handler();  // let LVGL read the keyboard, run animations and redraw
  delay(5);
}
