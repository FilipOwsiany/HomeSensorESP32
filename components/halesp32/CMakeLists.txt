idf_component_register( 
    SRCS            "wifi/CWifi.cpp"
                    "https/httpsClient/CHttpsClient.cpp"
     
    INCLUDE_DIRS    "wifi/include"
                    "https/httpsClient/include"
    REQUIRES
    # ===== IDF =====
    esp_wifi
    esp_http_client
    wpa_supplicant
    # === CUSTOM ====
    config
    PRIV_REQUIRES
    # ===== IDF =====
    log
    # === CUSTOM ====
    common
    utility
)
