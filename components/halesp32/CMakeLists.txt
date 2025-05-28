idf_component_register( 
    SRCS                "adc/CAdc.cpp"
                        "wifi/CWifi.cpp"
                        "bme280/bmx280.c"
                        "bme280/bme280.c"
                        "bme280/bme280_support.c"
                        "bme280/CBme280.cpp"
                        "https/httpsClient/CHttpsClient.cpp"
     
    INCLUDE_DIRS        "adc/include"    
                        "bme280/include"
                        "wifi/include"
                        "https/httpsClient/include"
    REQUIRES
    # ===== IDF =====
    esp_adc
    esp_wifi
    esp_http_client
    wpa_supplicant
    driver
    # === CUSTOM ====
    config
    PRIV_REQUIRES
    # ===== IDF =====
    log
    # === CUSTOM ====
    common
    utility
)
