idf_component_register(
    SRCS "communication/CCommunication.cpp"
         "jsonBuilder/CJsonBuilder.cpp"
    INCLUDE_DIRS "communication/include"
                 "jsonBuilder/include"
    REQUIRES
    # ===== IDF =====
    json
    # === CUSTOM ====
    common
    utility
    halesp32
)

