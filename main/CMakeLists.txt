

idf_component_register(
    SRCS "ESP32_HomeSensor.cpp"
    INCLUDE_DIRS "."
    PRIV_REQUIRES
    # ===== IDF =====
        nvs_flash
        esp_netif
        esp_event
        esp_timer
        esp_pm
        driver
    # === CUSTOM ====
        common
        lvgl
        lvgl_esp32_drivers
        control
        communication
        hardware
        utility
)
