#include <lvgl.h>  //去lvgl.conf调试
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include "my_gui.h"
#include <WiFi.h>
#include <time.h>
#include "global.h"
#include <Arduino.h> 
// Touchscreen pins
#define XPT2046_IRQ 36   // T_IRQ
#define XPT2046_MOSI 32  // T_DIN
#define XPT2046_MISO 39  // T_OUT
#define XPT2046_CLK 25   // T_CLK
#define XPT2046_CS 33    // T_CS
#define TFT_RST -1

//TFT LED 3.3V
//TFT RST 3.3V


SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);
TFT_eSPI tft = TFT_eSPI();//TFT_eSP 去文件里user_setup调试

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240


// LVGL 显存缓冲区（部分刷新） 模板
static uint8_t lvgl_buf[SCREEN_WIDTH * SCREEN_HEIGHT / 10 * 2];//默认

//static uint8_t lvgl_buf[SCREEN_WIDTH * 20 * 2];

// 屏幕对象 模板
static lv_display_t *display;


// 屏幕刷新回调  模板
static void my_flush_cb(lv_display_t * disp,
                        const lv_area_t * area,
                        uint8_t * px_map)
{
  uint16_t x = area->x1;
  uint16_t y = area->y1;
  uint16_t w = area->x2 - area->x1 + 1;
  uint16_t h = area->y2 - area->y1 + 1;

  tft.startWrite();
  tft.setAddrWindow(x, y, w, h);
  tft.pushPixels((uint16_t *)px_map, w * h);
  tft.endWrite();

  lv_display_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read( lv_indev_t * indev, lv_indev_data_t * data )
{
      // 如果没按，就告诉 LVGL：松开
    if (!(touchscreen.tirqTouched() && touchscreen.touched())) {
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }

    // 读取触摸点
    TS_Point p = touchscreen.getPoint();

    // 把原始触摸坐标映射到屏幕像素
    int16_t x = map(p.x, 3700, 200, 0, SCREEN_WIDTH  - 1); //方向
    int16_t y = map(p.y, 3800, 240, 0, SCREEN_HEIGHT - 1); //

    data->state    = LV_INDEV_STATE_PRESSED;
    data->point.x  = x;
    data->point.y  = y;
  }
     
//以上照抄


// =====================
// 初始化 WiFi + NTP
// =====================
void init_wifi_ntp()
{
    Serial.println("WiFi Connecting...");
    WiFi.begin("corvette best", "manual51");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");

    // 设置 NTP
    configTime(8 * 3600, 0, "pool.ntp.org", "time.nist.gov");

    Serial.println("Waiting for NTP time...");
    struct tm timeinfo;
    while (!getLocalTime(&timeinfo)) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nNTP Time Synced!");
}

// =====================
// 获取时间
// =====================

void getTime(void *pvParameters) {
  while (1) {
    struct tm timeinfo;

    if (getLocalTime(&timeinfo)) {
      hh = timeinfo.tm_hour;
      mm = timeinfo.tm_min;
      ss = timeinfo.tm_sec;

      Serial.printf("Time: %02d:%02d:%02d\n", hh, mm, ss);
    } else {
      Serial.println("Failed to obtain time");
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}




void setup() {
  Serial.begin(115200);


  // 初始化 TFT
  tft.init();
  tft.setRotation(1);

  //初始化触摸屏
    // Start the SPI for the touchscreen and init the touchscreen
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);

  // 初始化 LVGL
  lv_init();

  // 创建显示驱动
  display = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);

  // 设置显示 buffer（单缓冲就行） //照抄
  lv_display_set_buffers(display,
                         lvgl_buf,
                         NULL,
                         sizeof(lvgl_buf),
                         LV_DISPLAY_RENDER_MODE_PARTIAL);

  // 设置刷屏回调
  lv_display_set_flush_cb(display, my_flush_cb);


     // 注册触摸输入设备
  lv_indev_t * indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, my_touchpad_read);
  lv_indev_set_display(indev, display);   // ★ 绑定到当前 display
  // ====================================

  // ---------------------------
  //   创建 LVGL (网上都是调试好的环境)
  // ---------------------------
  my_gui();


 // 初始化 WIFI
  init_wifi_ntp();

  configTime(0, 0, "pool.ntp.org");

  xTaskCreate(getTime, "Task1", 4096, NULL, 1,NULL ); 


}

void loop() {
  

  lv_timer_handler();
  lv_tick_inc(5); 
  delay(5);
}
