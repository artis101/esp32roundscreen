#define LGFX_USE_V1
#include <Arduino.h>
#include <lvgl.h>
#include <LovyanGFX.hpp>
#include <Ticker.h>
#include "CST816D.h"
#include "ui.h"

#define I2C_SDA 4
#define I2C_SCL 5
#define TP_INT 0
#define TP_RST 1

#define off_pin 35
#define buf_size 120
#define is_touch 1

// 25 rows in height and 28 rows in width max
// 
static const char *ASCII_ART[] = {
    "          ^...01",
    "       ^......02",
    "     ^........03",
    "    ^.........04",
    "   ^..........05",
    "  ^...........06",
    " ^............07",
    " ^............08",
    "^.............09",
    "^.............10",
    "..............11",
    "..............12",
    "..............13",
    "01############14###########29",
    "..............15",
    "..............16",
    "..............17",
    "^.............18",
    " .............19",
    " ^............20",
    "  ^...........21",
    "   ^..........22",
    "    ^.........23",
    "     ^........24",
    "       ^......25",
    "         ^....26",
}; 

class LGFX : public lgfx::LGFX_Device
{
    lgfx::Panel_GC9A01 _panel_instance;
    lgfx::Bus_SPI _bus_instance;

public:
    LGFX(void)
    {
        {
            auto cfg = _bus_instance.config();

            // SPI bus configuration
            cfg.spi_host = SPI2_HOST; // Select which SPI to use. ESP32-S2,C3: SPI2_HOST or SPI3_HOST / ESP32: VSPI_HOST or HSPI_HOST
            cfg.spi_mode = 0;         // Set SPI communication mode (0 ~ 3)
            cfg.freq_write = 80000000; // SPI clock during transmission (max 80MHz, rounded to nearest value divisible by 80MHz)
            cfg.freq_read = 20000000;  // SPI clock during reception
            cfg.spi_3wire = true;      // Set true if receiving on MOSI pin
            cfg.use_lock = true;       // Set true when using transaction lock
            cfg.dma_channel = SPI_DMA_CH_AUTO; // Set DMA channel (0=DMA disabled / 1=1ch / 2=ch / SPI_DMA_CH_AUTO=automatic setting)
            cfg.pin_sclk = 6;          // Set SPI SCLK pin number
            cfg.pin_mosi = 7;          // Set SPI MOSI pin number
            cfg.pin_miso = -1;         // Set SPI MISO pin number (-1 = disable)
            cfg.pin_dc = 2;            // Set SPI D/C pin number (-1 = disable)

            _bus_instance.config(cfg);              // Apply settings to the bus
            _panel_instance.setBus(&_bus_instance); // Set the bus to the panel
        }

        {                                      // Configure display panel control
            auto cfg = _panel_instance.config(); // Get configuration structure for display panel

            cfg.pin_cs = 10;   // Pin number connected to CS (-1 = disable)
            cfg.pin_rst = -1;  // Pin number connected to RST (-1 = disable)
            cfg.pin_busy = -1; // Pin number connected to BUSY (-1 = disable)

            cfg.memory_width = 240;  // Maximum width supported by driver IC
            cfg.memory_height = 240; // Maximum height supported by driver IC
            cfg.panel_width = 240;   // Actual displayable width
            cfg.panel_height = 240;  // Actual displayable height
            cfg.offset_x = 0;        // Panel X direction offset
            cfg.offset_y = 0;        // Panel Y direction offset
            cfg.offset_rotation = 0; // Value offset in rotation direction 0~7 (4~7 are inverted)
            cfg.dummy_read_pixel = 8; // Number of dummy bits to read before reading pixels
            cfg.dummy_read_bits = 1;  // Number of dummy read bits before reading non-pixel data
            cfg.readable = false;     // Set to true if data can be read
            cfg.invert = true;        // Set to true if panel light/dark is inverted
            cfg.rgb_order = false;    // Set to true if panel red and blue are swapped
            cfg.dlen_16bit = false;   // Set to true for panels that send data length in 16-bit units
            cfg.bus_shared = false;   // Set to true if bus is shared with SD card (for bus control when using drawJpgFile etc.)

            _panel_instance.config(cfg);
        }

        setPanel(&_panel_instance); // Set the panel to use
    }
};

// Create an instance of the prepared class
LGFX tft;
CST816D touch(I2C_SDA, I2C_SCL, TP_RST, TP_INT);

/* Change to your screen resolution */
static const uint32_t screenWidth = 240;
static const uint32_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[2][screenWidth * buf_size];

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(lv_log_level_t level, const char *file, uint32_t line, const char *fn_name, const char *dsc)
{
    Serial.printf("%s(%s)@%d->%s\r\n", file, fn_name, line, dsc);
    Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    if (tft.getStartCount() == 0)
    {
        tft.endWrite();
    }

    tft.pushImageDMA(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, (lgfx::swap565_t *)&color_p->full);

    lv_disp_flush_ready(disp); /* tell lvgl that flushing is done */
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{

    bool touched;
    uint8_t gesture;
    uint16_t touchX, touchY;

    touched = touch.getTouch(&touchX, &touchY, &gesture);

    if (!touched)
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = touchX;
        data->point.y = touchY;
    }
}

Ticker ticker;

void tcr1s()
{
    Serial.printf("SRAM free size: %d\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
}

// Convert the ASCII_ART array into a single string separated by \n
String getAsciArtString()
{
    String asciiArtString = "";
    for (int i = 0; i < sizeof(ASCII_ART) / sizeof(ASCII_ART[0]); i++)
    {
        asciiArtString += ASCII_ART[i];
        if (i < sizeof(ASCII_ART) / sizeof(ASCII_ART[0]) - 1)
        {
            asciiArtString += "\n"; // Add newline between lines
        }
    }
    return asciiArtString;
}

void setup()
{
    Serial.begin(115200); /* prepare for possible serial debug */
    Serial.println("Hi I'm Artis' Arduino");

    // ticker.attach(1, tcr1s);

    tft.init();
    tft.initDMA();
    tft.startWrite();
    tft.setColor(0, 0, 0);

    if (is_touch == 1)
    {
        touch.begin();
    }

    lv_init();

#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

    lv_disp_draw_buf_init(&draw_buf, buf[0], buf[1], screenWidth * buf_size);

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /*Initialize the (dummy) input device driver*/
    if (is_touch == 1)
    {
        static lv_indev_drv_t indev_drv;
        lv_indev_drv_init(&indev_drv);
        indev_drv.type = LV_INDEV_TYPE_POINTER;
        indev_drv.read_cb = my_touchpad_read;
        lv_indev_drv_register(&indev_drv);
    }

    init_ui();

    Serial.println("Setup done, enjoy the sakura");

    pinMode(3, OUTPUT);
    digitalWrite(3, HIGH);

    pinMode(0, INPUT);
}

void loop()
{
    lv_timer_handler(); /* let the GUI do its work */
    delay(5);
}

