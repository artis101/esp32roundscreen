# ESP32C3 GC9A01 dev board with a 1.28" 240x240 IPS LCD

This is a work in progress to showcase this module. A write up will follow soon.

## How it looks like?

![Picture of ESP32C3 module with a round 1.28 inch IPS display](https://github.com/artis101/esp32roundscreen/blob/main/screenshot.png?raw=true "Picture of ESP32C3 module with a round 1.28 inch IPS display")

### Links

- [AliExpress listing](https://www.aliexpress.com/item/1005006140751675.html) note that I do not endorse this seller, I just bought mine from here.
- [Official documentation](https://wiki.icbbuy.com/doku.php?id=developmentboard:esp32-c3-1.28-lcd)

### Dependencies

- `LovyanGFX` 1.1.6
- `lvgl` 8.3.0

### `lv_conf.h`` adjustments

```c
#define LV_USE_FS_FATFS # 1
```

Needs to be changed to

```c
#define LV_USE_FS_FATFS 0
```

This board doesn't have an SD card reader or internal flash that can be accessed. Instead SPIFFS is used to store assets.
