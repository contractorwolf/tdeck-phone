// touchcal.ino - touch calibration helper for the T-Deck (landscape, 320x240).
// Shows a target at each corner and the centre, one at a time. Tap each target; the raw touch
// numbers are printed to serial next to the screen position, so we can work out how the touch
// chip's portrait coordinates map onto the landscape screen.
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <Wire.h>

constexpr int BOARD_POWERON = 10;
constexpr int TOUCH_I2C_ADDR = 0x5D;
constexpr int I2C_SDA = 18;
constexpr int I2C_SCL = 8;

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

struct Target { int x, y; const char *name; };
const Target TARGETS[] = {{20, 20, "top-left"}, {300, 20, "top-right"}, {300, 220, "bottom-right"},
                          {20, 220, "bottom-left"}, {160, 120, "centre"}};
constexpr int TARGET_COUNT = sizeof(TARGETS) / sizeof(TARGETS[0]);

// Read the first touch point. Returns true if a finger is down. Raw values are in the chip's
// own portrait shape (x 0-239, y 0-319).
static bool read_touch(int &raw_x, int &raw_y) {
  Wire.beginTransmission(TOUCH_I2C_ADDR);
  Wire.write(0x81); Wire.write(0x4E);
  if (Wire.endTransmission() != 0 || Wire.requestFrom(TOUCH_I2C_ADDR, 1) != 1) return false;
  const uint8_t status = Wire.read();
  bool down = false;
  if ((status & 0x80) && (status & 0x0F) > 0) {
    Wire.beginTransmission(TOUCH_I2C_ADDR);
    Wire.write(0x81); Wire.write(0x50);
    Wire.endTransmission();
    if (Wire.requestFrom(TOUCH_I2C_ADDR, 4) == 4) {
      uint8_t p[4];
      for (int i = 0; i < 4; i++) p[i] = Wire.read();
      raw_x = p[0] | (p[1] << 8);
      raw_y = p[2] | (p[3] << 8);
      down = true;
    }
  }
  if (status & 0x80) {  // clear the "new report" flag so the chip sends the next one
    Wire.beginTransmission(TOUCH_I2C_ADDR);
    Wire.write(0x81); Wire.write(0x4E); Wire.write(0x00);
    Wire.endTransmission();
  }
  return down;
}

static void draw_target(int i) {
  gfx.fillScreen(TFT_BLACK);
  gfx.setTextColor(TFT_WHITE);
  gfx.setTextSize(2);
  gfx.setCursor(60, 100);
  gfx.printf("Tap the %s", TARGETS[i].name);
  gfx.setCursor(60, 125);
  gfx.printf("target (%d/%d)", i + 1, TARGET_COUNT);
  const int x = TARGETS[i].x, y = TARGETS[i].y;
  gfx.drawLine(x - 12, y, x + 12, y, TFT_RED);
  gfx.drawLine(x, y - 12, x, y + 12, TFT_RED);
  gfx.drawCircle(x, y, 8, TFT_RED);
}

void setup() {
  Serial.begin(115200);
  pinMode(BOARD_POWERON, OUTPUT);
  digitalWrite(BOARD_POWERON, HIGH);
  delay(500);
  Wire.begin(I2C_SDA, I2C_SCL);
  gfx.init();
  gfx.setRotation(1);  // landscape, same as phone.ino
  gfx.setBrightness(200);
}

void loop() {
  Serial.println("CAL START");
  for (int i = 0; i < TARGET_COUNT; i++) {
    draw_target(i);
    int rx = 0, ry = 0;
    while (!read_touch(rx, ry)) delay(10);  // wait for a tap
    Serial.printf("CAL %s screen=(%d,%d) raw=(%d,%d)\n", TARGETS[i].name, TARGETS[i].x, TARGETS[i].y, rx, ry);
    gfx.fillScreen(TFT_GREEN);
    // wait for the finger to lift (no touch for 300 ms)
    uint32_t last_down = millis();
    while (millis() - last_down < 300) {
      int a, b;
      if (read_touch(a, b)) last_down = millis();
      delay(10);
    }
  }
  Serial.println("CAL DONE");
  gfx.fillScreen(TFT_BLUE);
  delay(3000);
}
