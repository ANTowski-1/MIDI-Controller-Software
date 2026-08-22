#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/

extern void action_led_color_change(lv_obj_t *led, uint32_t color) {
    lv_obj_set_style_bg_color(led, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT); 
};