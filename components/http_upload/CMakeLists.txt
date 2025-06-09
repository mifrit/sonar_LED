cmake_minimum_required(VERSION 3.16)

idf_component_register(
    SRCS            "http_upload.c" 
    INCLUDE_DIRS    "."
    EMBED_TXTFILES  "web/index.html" 
    REQUIRES        "esp_http_server"
                    "esp_partition"
)