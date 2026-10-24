// Hello LVGL on the LilyGO T-Deck: one label on the screen, nothing else.
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lvgl.h>

constexpr int BOARD_POWERON = 10;
constexpr int SCREEN_W = 320;
constexpr int SCREEN_H = 240;

// ST7789 on the shared SPI bus; pins from docs/reference/LilyGO_T-Deck_utilities.h
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

static void flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px) {
  const int w = area->x2 - area->x1 + 1;
  const int h = area->y2 - area->y1 + 1;
  gfx.startWrite();
  gfx.setAddrWindow(area->x1, area->y1, w, h);
  gfx.writePixels(reinterpret_cast<lgfx::rgb565_t *>(px), w * h);
  gfx.endWrite();
  lv_display_flush_ready(disp);
}

void setup() {
  Serial.begin(115200);
  pinMode(BOARD_POWERON, OUTPUT);
  digitalWrite(BOARD_POWERON, HIGH);  // peripherals are off on battery without this
  delay(100);

  gfx.init();
  gfx.setRotation(1);  // landscape, 320x240
  gfx.setBrightness(200);
  gfx.fillScreen(TFT_BLUE);  // raw check before LVGL
  Serial.println("gfx init done, filled blue");
  delay(1500);

  lv_init();
  lv_tick_set_cb([]() -> uint32_t { return millis(); });

  // partial render, two buffers of 1/10 screen in PSRAM
  constexpr size_t buf_bytes = SCREEN_W * SCREEN_H / 10 * sizeof(uint16_t);
  void *buf1 = heap_caps_malloc(buf_bytes, MALLOC_CAP_SPIRAM);
  void *buf2 = heap_caps_malloc(buf_bytes, MALLOC_CAP_SPIRAM);
  lv_display_t *disp = lv_display_create(SCREEN_W, SCREEN_H);
  lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565);
  lv_display_set_flush_cb(disp, flush_cb);
  lv_display_set_buffers(disp, buf1, buf2, buf_bytes, LV_DISPLAY_RENDER_MODE_PARTIAL);

  lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x0000FF), 0);
  lv_obj_set_style_bg_opa(lv_screen_active(), LV_OPA_COVER, 0);

  lv_obj_t *label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "Hello T-Deck");
  lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
  lv_obj_set_style_text_color(label, lv_color_white(), 0);
  lv_obj_center(label);

  Serial.printf("free PSRAM: %u buf1=%p buf2=%p\n", (unsigned)ESP.getFreePsram(), buf1, buf2);
}

void loop() {
  static uint32_t last = 0;
  if (millis() - last > 2000) { last = millis(); Serial.println("alive"); }
  lv_timer_handler();
  delay(5);
}
