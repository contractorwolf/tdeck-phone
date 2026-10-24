// i2cscan.ino - prints the address of every device that answers on the T-Deck I2C bus.
// Used to find out which chips are on the bus (keyboard should be 0x55; touch is probably 0x5D or 0x14).
#include <Wire.h>

constexpr int BOARD_POWERON = 10;  // peripherals are off without this
constexpr int I2C_SDA = 18;
constexpr int I2C_SCL = 8;

void setup() {
  Serial.begin(115200);
  pinMode(BOARD_POWERON, OUTPUT);
  digitalWrite(BOARD_POWERON, HIGH);
  delay(500);  // let the chips power up
  Wire.begin(I2C_SDA, I2C_SCL);
}

void loop() {
  Serial.println("--- I2C scan ---");
  int found = 0;
  for (int addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {  // 0 = a device acknowledged this address
      Serial.printf("found 0x%02X\n", addr);
      found++;
    }
  }
  Serial.printf("%d device(s)\n", found);
  delay(3000);
}
