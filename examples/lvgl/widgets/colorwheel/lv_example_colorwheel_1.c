#include <lvgl.h>
#if LV_USE_COLORWHEEL

void lv_example_colorwheel_1(void)
{
    lv_obj_t *cw;

    cw = lv_colorwheel_create(lv_scr_act(), true);
    lv_obj_set_size(cw, 200, 200);
    lv_obj_center(cw);
}

#endif
