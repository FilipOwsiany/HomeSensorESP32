idf_component_register(
    SRCS "communication/CCommunication.cpp"
    INCLUDE_DIRS "communication/include"
    REQUIRES
    # ===== IDF =====

    # === CUSTOM ====
    common
    utility
)
