#include "actions.h"

void action_led_color_change(lv_obj_t *led, uint32_t color) {
    lv_obj_set_style_bg_color(led, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT);
}