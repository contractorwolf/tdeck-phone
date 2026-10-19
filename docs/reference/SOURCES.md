# Reference documents

Downloaded 2026-10-17. Third-party mirrors are used where SIMCom's own download page hides its links behind JavaScript.

| File | What | Source |
|---|---|---|
| `SIM7600G-H_SIM7600NA-H_Hardware_Design_V1.10.pdf` | SIMCom hardware design for the G-H module (82 pp). Primary pinout source | DFRobot mirror https://ws.dfrobot.com.cn/lqScRumjnLVipwa7W7KIq8ZLu86A |
| `SIM7600G_G-H_Hardware_Design_V1.01.pdf` | Earlier hardware design, same pad numbers (50 pp) | Elecrow https://www.elecrow.com/download//product/SEN18550G/SIM7600G%20G-H_Hardware%20design.pdf |
| `SIM7500_SIM7600_Series_AT_Command_Manual_V3.00.pdf` | AT command manual, 512 pp (SMS, voice, audio) | DFRobot mirror https://ws.dfrobot.com.cn/Ftbi-gQP6fuk5I1SigJuHrOtnrxq |
| `SIM7500_SIM7600_SIM7800_TCPIP_App_Note_V2.00.pdf` | TCP/IP app note, not needed for SMS/voice but handy | https://files.waveshare.com/upload/f/ff/SIM7500_SIM7600_SIM7800_Series_TCPIP_Application_Note_V2.00.pdf |
| `SIM7X00_Series_UART_Application_Note_V1.00.pdf` | SIMCom UART application note (2016; written for SIM7600C/CE, SIM7500A/JE, so older than our A-H): autobaud, flow control, DTR/RI behaviour, CMUX | https://files.waveshare.com/upload/7/79/SIM7X00_Series_UART_Application_Note_V1.00.pdf |
| `SIM7500_SIM7600_Series_GNSS_Application_Note_V2.00.pdf` | SIMCom GNSS application note V2.00 (2020), 19 pp | https://files.waveshare.com/upload/e/e1/SIM7500_SIM7600_Series_GNSS_Application_Note_V2.00.pdf |
| `Waveshare_SIM7600G-H_4G_HAT_schematic_V1.pdf` | Schematic of the older HAT revision (CP2102 + NAU8810) | https://files.waveshare.com/upload/6/63/SIM7600G-H-4G-HAT-schematic.pdf |
| `Waveshare_SIM7600X_4G_HAT_B_V2_schematic.pdf` | **Schematic of this board** (SIM7600X 4G HAT (B) V2): hub, USB1/2/3, UART stubs, PWRKEY R26 | https://files.waveshare.com/upload/f/fe/SIM7600X_4G_HAT_(B)_V2.pdf (linked from https://www.waveshare.com/wiki/SIM7600G-H_4G_HAT_(B)) |
| `Waveshare_SIM7600X_4G_HAT_B_V1_schematic.pdf` | Earlier (B) revision | https://files.waveshare.com/upload/f/fe/SIM7600X_4G_HAT_%28B%29.pdf |
| `LilyGO_T-Deck_schematic.pdf` | T-Deck schematic | https://github.com/Xinyuan-LilyGO/T-Deck/blob/master/schematic/schematic.pdf |
| `LilyGO_T-Deck_utilities.h` | LilyGO pin definitions (MIT) | https://github.com/Xinyuan-LilyGO/T-Deck/blob/master/examples/UnitTest/utilities.h |
| `ESP32-S3_datasheet.pdf` | Espressif datasheet (VIH 0.75 x VDD) | https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf |

| `SIM7500_SIM7600_SIM7800_Series_SMS_Application_Note_V2.00.pdf` | SIMCom SMS application note V2.00 (2020, 16 pp): storage, CMGL/CMGR/CMGD/CMGW/CMGS/CMSS flow, CNMI | https://files.waveshare.com/upload/f/f0/SIM7500_SIM7600_SIM7800_Series_SMS_Application_Note_V2.00.pdf |
| `SIM7X00_Series_SMS_Application_Note_V1.00.pdf` | Older SMS note (2016), same flow | https://files.waveshare.com/upload/0/06/SIM7X00_Series_SMS_Application_Note_V1.00.pdf |
| `SIM7X00_Series_SAT_Application_Note_V1.00.pdf` | SIM Application Toolkit (`+STIN`, `+STGI`, `+STGR`, `+STKFMT`, `+STSM`, `+STENV`) | https://files.waveshare.com/upload/e/e6/SIM7X00_Series_SAT_Application_Note_V1.00.pdf |
| `SIM7500_SIM7600_Series_FTPS_Application_Note_V2.00.pdf` | FTP(S) note V2.00 (the `+CFTPS...` family) | https://files.waveshare.com/upload/6/66/SIM7500_SIM7600_Series_FTP%28S%29_Application_Note_V2.00.pdf |
| `SIM7500_SIM7600_SIM7800_Series_MQTT_Application_Note_V2.00.pdf` | MQTT(S) note V2.00 (`+CMQTT...`) | https://files.waveshare.com/upload/5/50/SIM7500_SIM7600_SIM7800_Series_MQTT_Application_Note_V2.00.pdf |
| `SIM7500_SIM7600_Series_MQTT_ATC_V1.01.pdf` | MQTT AT command detail (40 pp) | https://files.waveshare.com/upload/6/65/SIM7500_SIM7600_Series_MQTT_ATC_V1.01.pdf |
| `SIM7500_SIM7600_SIM7800_Series_SSL_Application_Note_V2.00.pdf` | SSL/TLS note V2.00 (`+CSSLCFG`, `+CCERTDOWN`) | https://files.waveshare.com/upload/4/41/SIM7500_SIM7600_SIM7800_Series_SSL_Application_Note_V2.00.pdf |
| `SIM7500_SIM7600_Series_Compatibility_With_SIM800_HTTP_ATC_V1.00.pdf` | The SIM800-style `+HTTP...` commands (`+HTTPINIT`, `+HTTPPARA`, `+HTTPACTION`) | https://files.waveshare.com/upload/9/9a/SIM7500_SIM7600_Series_Compatibility_With_SIM800_HTTP_ATC_V1.00.pdf |
| `SIM7100_SIM7500_SIM7600_Series_USB_AUDIO_Application_Note_V1.03.pdf` | USB audio (`+CPCMREG`, `+CPCMFRM`) | https://files.waveshare.com/upload/8/8e/SIM7100_SIM7500_SIM7600_Series_USB_AUDIO_Application_Note_V1.03.pdf |

Downloaded 2026-10-18 (second research pass). The Waveshare `.net` mirror of the SMS V2.00 note returned an HTML page; the `files.waveshare.com` URL above is the working one.

## Not obtained

- SIMCom pages list newer notes (SMS V3.00, HTTP(S) V3.00, FTP(S) V3.00, MQTT(S) V3.00, TTS V3.00, USB audio V3.00, GNSS V3.00, UART V2.00, TCP/IP V3.00) but their download buttons are JavaScript placeholders, so no direct links were found (checked 2026-10-18 on https://cn.simcom.com/product/SIM7600X.html). Download from https://www.simcom.com/download.html in a browser.
- A7600 audio application note (Waveshare mirror URL returned an HTML error page). It is for a sibling module anyway.
- A newer AT manual than V3.00: searches found none (V3.00 is listed as current).

- Still missing: the HTTP(S) V3.00 and TTS V3.00 notes and a newer USB audio note. No Waveshare copy was found. Fetch from https://www.simcom.com/product/SIM7600X-H.html in a browser if needed.
- SIMCom copies on promelec.ru were blocked (DDoS-Guard).
