#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

void action_led_color_change(lv_obj_t *led, uint32_t color);

#ifdef __cplusplus
}
#endif

#endif /* EEZ_LVGL_UI_EVENTS_H */