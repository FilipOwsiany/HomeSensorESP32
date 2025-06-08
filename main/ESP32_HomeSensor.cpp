// ========== IDF ==========
#include "nvs_flash.h"
#include "esp_timer.h"
#include "esp_pm.h"
#include "esp_sleep.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
// ======== CUSTOM =========
#include "commonStd.h"
#include "commonOs.h"
#include "commonLvgl.h"
#include "CLogger.h"

#include "CControl.h"
#include "CCommunication.h"
#include "CHardware.h"

#define LV_TICK_PERIOD_MS   1
#define DRAW_BUF_SIZE       (240 * 240 / 10 * (LV_COLOR_DEPTH / 8))

static void lv_tick_task(void *arg);

uint32_t draw_buf[DRAW_BUF_SIZE / 4];
SemaphoreHandle_t xGuiSemaphore;

static void guiTask(void *pvParameter) {

    (void) pvParameter;
    xGuiSemaphore = xSemaphoreCreateMutex();

    lv_init();

    lvgl_driver_init();

    lv_display_t * disp;
    disp = lv_display_create(240, 240);
    lv_display_set_flush_cb(disp, disp_driver_flush);
    lv_display_set_buffers(disp, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

#if defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_IL3820         \
    || defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_JD79653A    \
    || defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_UC8151D     \
    || defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_SSD1306

    /* Actual size in pixels, not bytes. */
    size_in_px *= 8;
#endif

    printf("LV_HOR_RES_MAX %d\n", LV_HOR_RES_MAX);
    printf("LV_VER_RES_MAX %d\n", LV_VER_RES_MAX);

#ifdef CONFIG_LV_TFT_DISPLAY_MONOCHROME
    disp_drv.rounder_cb = disp_driver_rounder;
    disp_drv.set_px_cb = disp_driver_set_px;
#endif

#if CONFIG_LV_TOUCH_CONTROLLER != TOUCH_CONTROLLER_NONE
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.read_cb = touch_driver_read;
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    lv_indev_drv_register(&indev_drv);
#endif

    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .arg = NULL,
        .name = "periodic_gui"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

    static lv_style_t style;
    lv_style_init(&style);

    lv_style_set_radius(&style, LV_RADIUS_CIRCLE);
    lv_style_set_bg_opa(&style, LV_OPA_COVER);
    lv_style_set_bg_color(&style, lv_palette_lighten(LV_PALETTE_GREY, 1));

    lv_style_set_border_color(&style, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_border_width(&style, 5);
    lv_style_set_border_opa(&style, LV_OPA_50);
    lv_style_set_border_side(&style, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT);

    lv_obj_t* obj = lv_obj_create(lv_screen_active());
    lv_obj_add_style(obj, &style, 0);
    lv_obj_center(obj);
    lv_obj_set_size(obj, 240, 240);
    lv_obj_t * bar1 = lv_bar_create(lv_screen_active());
    lv_obj_set_size(bar1, 200, 20);
    lv_obj_center(bar1);
    lv_bar_set_value(bar1, 70, LV_ANIM_OFF);

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10));

        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
            lv_task_handler();
            xSemaphoreGive(xGuiSemaphore);
       }
    }

    vTaskDelete(NULL);
}

static void lv_tick_task(void *arg) {
    (void) arg;

    lv_tick_inc(LV_TICK_PERIOD_MS);
}

extern "C" void app_main(void)
{
    esp_pm_config_t pm_config = {
    .max_freq_mhz = 160,
    .min_freq_mhz = 40,
    #if CONFIG_FREERTOS_USE_TICKLESS_IDLE
    .light_sleep_enable = true
    #endif
    };
    ESP_ERROR_CHECK(esp_pm_configure(&pm_config));


    CLogger::log(CLoggerModule::Main, CLoggerLevel::Success, "Application started");

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);


    gpio_set_direction(GPIO_NUM_32, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_NUM_33, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);

    // gpio_set_level(GPIO_NUM_4, 1);

    // gpio_set_level(GPIO_NUM_32, 1);
    // gpio_set_level(GPIO_NUM_33, 0);

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    gpio_set_level(GPIO_NUM_32, 1);
    gpio_set_level(GPIO_NUM_33, 1);

    // CControl *control = new CControl();
    // CCommunication *communication = new CCommunication();
    // CHardware *hardware = new CHardware();

    // control->subscribe(communication);

    // hardware->subscribe(control);
    // hardware->subscribe(communication);

    //xTaskCreatePinnedToCore(guiTask, "gui", 4096 * 2, NULL, 0, NULL, 1);
        esp_sleep_enable_timer_wakeup(1000000);
        CLogger::log(CLoggerModule::Main, CLoggerLevel::Success, "Application go sleep");
        esp_light_sleep_start();
    while (true)
    {
        // gpio_set_level(GPIO_NUM_32, 1);
        // gpio_set_level(GPIO_NUM_33, 0);

        vTaskDelay(1000 / portTICK_PERIOD_MS);

        // gpio_set_level(GPIO_NUM_32, 0);
        // gpio_set_level(GPIO_NUM_33, 1);

        vTaskDelay(1000 / portTICK_PERIOD_MS);

        esp_sleep_enable_timer_wakeup(1000000);
        CLogger::log(CLoggerModule::Main, CLoggerLevel::Success, "Application go sleep");
        esp_light_sleep_start();
    }
    
}


