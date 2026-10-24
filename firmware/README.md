# Firmware

Built with `arduino-cli` (no PlatformIO). LVGL 9.3.0 and LovyanGFX 1.2.25 live in `libraries/` so the shared `~/Documents/Arduino/libraries` is left alone. They are not committed; fetch them once:

```
mkdir -p libraries && cd libraries
curl -sL https://github.com/lvgl/lvgl/archive/refs/tags/v9.3.0.zip -o lvgl.zip && unzip -q lvgl.zip && mv lvgl-9.3.0 lvgl
curl -sL https://github.com/lovyan03/LovyanGFX/archive/refs/tags/1.2.25.zip -o lgfx.zip && unzip -q lgfx.zip && mv LovyanGFX-1.2.25 LovyanGFX
rm *.zip && cd ..
```

`libraries/lv_conf.h` is committed (RGB565, clib malloc, Montserrat 14/20).

Build (esp32 core 3.3.0):

```
arduino-cli compile -b esp32:esp32:esp32s3 \
  --board-options "USBMode=hwcdc,CDCOnBoot=cdc,FlashSize=16M,PSRAM=opi,PartitionScheme=app3M_fat9M_16MB" \
  --libraries libraries hello
```

Upload adds `-u -p <port>`. If upload fails, hold the trackball click, plug in USB, upload, then press RST.
