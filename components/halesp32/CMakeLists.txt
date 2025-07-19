idf_component_register( 
    SRCS                "adc/CAdc.cpp"
                        "storage/CStorage.cpp"
                        "wifi/CWifi.cpp"
                        "bme280/bmx280.c"
                        "bme280/bme280.c"
                        "bme280/bme280_support.c"
                        "bme280/CBme280.cpp"
                        "halEspressif32Factory/CHalEspressifFactory.cpp"
                        "https/httpsClient/CHttpsClient.cpp"
                        "lowPower/CLowPower.cpp"
     
    INCLUDE_DIRS        "adc/include"    
                        "bme280/include"
                        "storage/include"
                        "wifi/include"
                        "halEspressif32Factory/include"
                        "https/httpsClient/include"
                        "lowPower/include"
    REQUIRES
    # ===== IDF =====
    esp_pm
    esp_adc
    esp_wifi
    esp_http_client
    wpa_supplicant
    nvs_flash
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
