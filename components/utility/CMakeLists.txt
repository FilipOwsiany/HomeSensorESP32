idf_component_register( 
    SRCS            "baseQueue/CBaseQueue.cpp"
                    "baseSemaphore/CBaseSemaphore.cpp"
                    "baseQueue/CMessageQueue.cpp"
                    "baseTask/CBaseTask.cpp"
                    "eventManager/IEventPublisherManager.cpp"
                    "logger/CLogger.cpp"
     
    INCLUDE_DIRS    "baseQueue/include"
                    "baseSemaphore/include"
                    "baseTask/include"
                    "eventManager/include"
                    "logger/include"
    REQUIRES
    # ===== IDF =====

    # === CUSTOM ====
    config
    PRIV_REQUIRES
    # ===== IDF =====
    log
    # === CUSTOM ====
    common
)
