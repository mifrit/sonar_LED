#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_partition.h"
#include "wifi_setup.h"

#define delay_ms(x)     vTaskDelay(x/portTICK_PERIOD_MS);

static const char* TAG = "LED";

#define LED_PIN 8
#define BUTTON_PIN 9

void app_main(void) {
    int64_t t0=0, t1=0;
    // access storage partition
    const esp_partition_t *part = esp_partition_find_first(ESP_PARTITION_TYPE_DATA , ESP_PARTITION_SUBTYPE_ANY, "storage");
    // setup accesspoint
    WIFI_init();
    OTA_init();
    static char save_data[] = "Das ist ein Test";
    static char read_data[sizeof(save_data)];

    esp_partition_write(part, 0, save_data, sizeof(save_data));
    //esp_partition_read(part, 1, read_data, sizeof(save_data)-1);
    ESP_LOGI(TAG, "Read data: %s", save_data);

    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);

    gpio_set_level(LED_PIN, 1);

    int old_button_level = 1;

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
            if ((t1-t0) > 5000) {
                if((t1-t0) < 2000000) {
                    ESP_LOGI(TAG, "short press: %i", (int)(t1-t0));
                } else {
                    ESP_LOGI(TAG, "long press: %i", (int)(t1-t0));
                }
            }
        }
        vTaskDelay(1);
    }
    
}