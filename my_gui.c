// my_gui.c
#include "my_gui.h"
#include "lvgl.h"
#include <stdio.h>
#include "global.h"
#include "PAGE2.h"
#include "BGDATA.h"


//变量定以全局GLOBAL.H AND .C

// 函数声明
void page1th(void);
static void slider_event_cb(lv_event_t * e);
static void buttonair_event_cb(lv_event_t * e);

/* GUI 入口 */
void my_gui(void)
{
    page1th();             // 创建主页

}

/* page1 页面 */
void page1th(void)
{
    lv_obj_t *page1 = lv_obj_create(NULL);
    lv_obj_set_layout(page1, LV_LAYOUT_NONE);



   TIME = lv_label_create(page1);
    lv_obj_align(TIME,LV_ALIGN_TOP_MID,0,30);

       // 🔥 直接对对象设置字体和颜色（不通过 Style 结构体，优先级最高）
    lv_obj_set_style_text_color(TIME, lv_color_hex(0xf800),
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(TIME, &lv_font_montserrat_30,
                               LV_PART_MAIN | LV_STATE_DEFAULT);


    lv_label_set_text_fmt(TIME, "%02d:%02d:%02d", hh, mm, ss);


    slider = lv_slider_create(page1);
    lv_obj_set_size(slider, 200, 20);
    lv_obj_center(slider);

    lv_style_init(&style_knob);
    lv_style_set_bg_opa(&style_knob, LV_OPA_TRANSP);
    lv_style_set_border_width(&style_knob, 0);
    lv_style_set_bg_image_src(&style_knob, &ratboss);
    lv_style_set_width(&style_knob, ratboss.header.w);
    lv_style_set_height(&style_knob, ratboss.header.h);
    lv_obj_add_style(slider, &style_knob, LV_PART_KNOB);

    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

     lv_screen_load(page1);

      // ★ 创建一个 1 秒刷新的定时器
       lv_timer_create(update_time_cb, 1000, NULL);
}

/* 滑块事件：滑到最大就切 page2 */
static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    int16_t value = lv_slider_get_value(slider);
    int16_t max   = lv_slider_get_max_value(slider);

    if (value == max) {
        page2_init();      // ⭐ 创建和准备 page2
    }
}

static void buttonair_event_cb(lv_event_t * e)
{
    // 比如你在别处创建了 pageair，这里切回 page2 或 page1
    page2_init();
}


