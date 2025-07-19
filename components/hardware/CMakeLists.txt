idf_component_register(
    SRCS         "hardware/CHardware.cpp"
                 "measQueue/CMeasQueue.cpp"
    INCLUDE_DIRS "hardware/include"
                 "measQueue/include"
    REQUIRES
    # ===== IDF =====

    # === CUSTOM ====
    common
    utility
    halesp32
)
