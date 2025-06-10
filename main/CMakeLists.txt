idf_component_register(
    INCLUDE_DIRS    "."
    SRCS            "main.c"
    REQUIRES        esp_driver_gpio
    REQUIRES        esp_partition
    REQUIRES        esp_timer
    REQUIRES        wifi
    REQUIRES        http_upload
    REQUIRES        led_driver 
)
