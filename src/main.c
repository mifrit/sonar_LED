#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char* TAG = "LED";

#define LED_PIN 8

void app_main(void){
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    int ON = 0;
    ESP_LOGI(TAG,"Hello\n");
    while(true){
        ON = !ON;
        gpio_set_level(LED_PIN, ON);
        vTaskDelay(1000/ portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "+");
    }
}