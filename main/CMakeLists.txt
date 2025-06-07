idf_component_register(
    SRCS            "main.c"
    PRIV_REQUIRES   spi_flash
    REQUIRES        esp_driver_gpio
    REQUIRES        esp_partition
    REQUIRES        esp_timer
    REQUIRES        wifi
    REQUIRES        http_upload
    INCLUDE_DIRS    "."
)
