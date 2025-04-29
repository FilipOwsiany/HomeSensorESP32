

idf_component_register(
    SRCS "ESP32_HomeSensor.cpp"
    INCLUDE_DIRS "."
    PRIV_REQUIRES
    # ===== IDF =====
        nvs_flash
        esp_netif
        esp_event
    # ===== AMC =====
        lvgl
        lvgl_esp32_drivers
        driver
        esp_timer
)
