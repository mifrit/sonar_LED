#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_partition.h"

#define delay_ms(x)     vTaskDelay(x/portTICK_PERIOD_MS);

static const char* TAG = "LED";

#define LED_PIN 8

void app_main(void) {
    // access storage partition
    const esp_partition_t *part = esp_partition_find_first(ESP_PARTITION_TYPE_DATA , ESP_PARTITION_SUBTYPE_ANY, "storage");
    // setup accesspoint
    //WIFI_init();
    static char save_data[] = "Das ist ein Test";
    static char read_data[sizeof(save_data)];

    esp_partition_write(part, 0, save_data, sizeof(save_data));
    esp_partition_read(part, 1, read_data, sizeof(save_data)-1);
    ESP_LOGI(TAG, "Read data: %s", read_data);

    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    int ON = 0;
    while(true){
        ON = !ON;
        gpio_set_level(LED_PIN, ON);
        delay_ms(300);
    }
}