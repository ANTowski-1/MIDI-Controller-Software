#include <lvgl.h>
// #include <display_config.cpp>
#include <TFT_eSPI.h>

#define BUF_SIZE (320 * 40 * 2)

static uint8_t draw_buf[BUF_SIZE];
static uint32_t my_tick(void)
{
    return millis();
}

TFT_eSPI tft = TFT_eSPI();

void lvgl_init(void) {
    tft.init();
    tft.setRotation(1);
    lv_init();
    lv_tick_set_cb(my_tick);

    lv_display_t * disp = lv_tft_espi_create(320, 240, draw_buf, sizeof(draw_buf));
}
