idf_component_register(
    SRCS "CHardware.cpp"
    INCLUDE_DIRS "include"
    REQUIRES
    # ===== IDF =====

    # === CUSTOM ====
    common
    utility
    halesp32
)
