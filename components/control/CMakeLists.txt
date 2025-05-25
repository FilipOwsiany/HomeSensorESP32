idf_component_register(
    SRCS "CControl.cpp"
    INCLUDE_DIRS "include"
    REQUIRES
    # ===== IDF =====

    # === CUSTOM ====
    common
    utility
    halesp32
)
