// PAGE2.c
#include "PAGE2.h"
#include "my_gui.h"   // 如果里面需要用到 page1
#include "global.h"
    // ⭐ 真正的定义，只在一个 .c 里出现一次

static void button2_event_cb(lv_event_t * e);


void page2_init(void)
{

        lv_obj_t *page2 = lv_obj_create(NULL);
        lv_obj_set_layout(page2, LV_LAYOUT_NONE);

        // 标题
        lv_obj_t *label = lv_label_create(page2);
        lv_label_set_text(label, "Rat Portal");
        lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

        // 左边按钮
        lv_obj_t *button1 = lv_button_create(page2);
        lv_obj_align(button1, LV_ALIGN_TOP_LEFT, 0, 30);
        lv_obj_t *label1 = lv_label_create(button1);
        lv_label_set_text(label1, "Rat Air quality Monitor");
        lv_obj_center(label1);
        // lv_obj_add_event_cb(button1, button1_event_cb, LV_EVENT_PRESSED, NULL);

        // 返回按钮
        lv_obj_t *button2 = lv_button_create(page2);
        lv_obj_align(button2, LV_ALIGN_BOTTOM_LEFT, 0, 0);
        lv_obj_t *label2 = lv_label_create(button2);
        lv_label_set_text(label2, "BACK");
        lv_obj_center(label2);
        lv_obj_add_event_cb(button2, button2_event_cb, LV_EVENT_PRESSED, NULL);

    lv_screen_load(page2);
}

static void button2_event_cb(lv_event_t * e)
{
        page1th();
}
