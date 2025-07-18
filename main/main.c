#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_partition.h"
#include "wifi_setup.h"
#include "http_upload.h"
#include "ws2812_control.h"
//#include "rtc_wdt.h"

#define delay_ms(x)     vTaskDelay(x/portTICK_PERIOD_MS);

static const char* TAG = "LED";

#define LED_PIN 8
#define BUTTON_PIN 10

#define LED_SPEED_START 100
#define LED_SPEED_STEP  10000
#define LED_SPEED_STOP  40000

void app_main(void) {
    int64_t t0=0, t1=0, t_old=0, t_now;

    struct led_state led_buf;
    for(int i=0; i<65; i++)
        led_buf.leds[i] = 0;

    int led_speed = LED_SPEED_START;
    uint8_t led_s_num = 0;
    uint8_t led_s_cnt = 0;
    uint8_t led_mtx[32];
    uint32_t led_color[16];

    
    // access partition
    esp_partition_t *part = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_TYPE_ANY, "storage");

    // setup accesspoint
     WIFI_init();
    // start http server
    http_server_init();
    // init led_driver
    ws2812_control_init();

    // ini LED and Button
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);

    gpio_set_level(LED_PIN, 1);

    int old_button_level = 1;
    bool am_I_online = true;

    //read LED seq-num and color table
    esp_partition_read(part, 0, &led_s_num, 1);
    esp_partition_read(part, 1, led_color, 64);

    // main loop
    while(1) {
        // falling trigger
        if ((gpio_get_level(BUTTON_PIN) == 0) && (old_button_level == 1)) {
            t0 = esp_timer_get_time();
            old_button_level = 0;
            ESP_LOGI(TAG, "down");
        }
        // down for longer than 2 Seconds
        if ((gpio_get_level(BUTTON_PIN) == 0) && (old_button_level == 0) && ((esp_timer_get_time()-t0) > 2000000)) {
            gpio_set_level(LED_PIN, 0);
        }
        if ((gpio_get_level(BUTTON_PIN) == 0) && (old_button_level == 0) && ((esp_timer_get_time()-t0) > 2100000)) {
            gpio_set_level(LED_PIN, 1);
        }
        // rising trigger
        if ((gpio_get_level(BUTTON_PIN) == 1) && (old_button_level == 0)) {
            t1 = esp_timer_get_time();
            old_button_level = 1;
            gpio_set_level(LED_PIN, 1);
            ESP_LOGI(TAG, "up");
            if ((t1-t0) > 10000) {
                if((t1-t0) < 2000000) {
                    if(am_I_online) {
                        //WIFI_stop();
                        ESP_LOGI(TAG, "WiFi stoped");
                        am_I_online = false;
                        led_speed = LED_SPEED_START;
                        //read LED seq-num and color table
                        esp_partition_read(part, 0, &led_s_num, 1);
                        esp_partition_read(part, 1, led_color, 64);
                        led_s_cnt = 0;
                    }
                    else {
                        led_speed += LED_SPEED_STEP;
                        if(led_speed > LED_SPEED_STOP) {
                            led_speed = LED_SPEED_START;
                        }
                    }
                    ESP_LOGI(TAG, "LED speed: %i", led_speed);
                } else {
                    ESP_LOGI(TAG, "long press: %i", (int)(t1-t0));
                    for(int i=0; i<65; i++)
                        led_buf.leds[i] = 0x000000;
                    ws2812_write_leds(led_buf);
                    //WIFI_start();
                    delay_ms(100);
                    am_I_online = true;
                }
            }
        }
        t_now = esp_timer_get_time();
        if(((t_now - t_old) > led_speed) && !am_I_online) {
            //change picture
            //read pixel data from flash
            esp_partition_read(part, (65 + led_s_cnt * 32), led_mtx, 32);
            int j = 1;
            for(int i=0; i<32; i++){
                led_buf.leds[j++] = led_color[led_mtx[i] >> 4];
                led_buf.leds[j++] = led_color[led_mtx[i] & 0x0f];
                ws2812_write_leds(led_buf);
            }
            //prepare for the next round
            led_s_cnt ++;
            if(led_s_cnt > led_s_num)
                led_s_cnt = 0;
            t_old = t_now;
        }
    }
    
}