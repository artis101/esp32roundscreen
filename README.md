# ESP32C3 GC9A01 dev board with a 1.28" 240x240 LCD

This is a fun project I made to play with the ESP32 C3 microcontroller and a round 1.28 inch TFT IPS display.

## Description

The SKU of this unit is `ESP32-2424S012C-I`. It uses a 240 by 240 pixel display with a GC9A01 driver. The board has a 3.3V regulator and a USB-C connector for power and programming.

The core is an ESP32-C3-MINI-1U module, a 32-bit RISC-V single-core processor, up to 160MHz. The board has 4MB of flash memory and 384KB of ROM. 400Kb of SRAM is available for the user. 8Kb of RTC memory is also available. The board has a 2.4GHz WiFi and Bluetooth 5.0.

## How it looks like?

![Picture of ESP32C3 module with a round 1.28 inch IPS display](https://github.com/artis101/esp32roundscreen/blob/main/screenshot.png?raw=true "Picture of ESP32C3 module with a round 1.28 inch IPS display")

### Links

- [AliExpress listing](https://www.aliexpress.com/item/1005006140751675.html) note that I do not endorse this seller, I just bought mine from here.
- [Official documentation](https://wiki.icbbuy.com/doku.php?id=developmentboard:esp32-c3-1.28-lcd)

### Dependencies

- `LovyanGFX` 1.1.6
- `lvgl` 8.3.0

### `lv_conf.h` adjustments

```c
#define LV_USE_FS_FATFS 1
```

Needs to be changed to

```c
#define LV_USE_FS_FATFS 0
```

This board doesn't have an SD card reader or internal flash that can be accessed.

The ESP32 is partitioned as a Huge APP preset with 3Mb of application memory and 1Mb of SPIFFS for storage.
It is also a good idea to change

```c
#define LV_BUILD_EXAMPLES 1
```

to

```c
#define LV_BUILD_EXAMPLES 0
```

That will avoid building all `lvgl` examples and save a lot of time compiling.

Unless you want to compile the examples just as the factory demo does.
