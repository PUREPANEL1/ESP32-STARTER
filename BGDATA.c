// PAGE2.c
#include "my_gui.h"   // 如果里面需要用到 page1
#include "global.h"
    // ⭐ 真正的定义，只在一个 .c 里出现一次

void update_time_cb(lv_timer_t * timer)
{
    lv_label_set_text_fmt(TIME, "%02d:%02d:%02d", hh, mm, ss);
}
