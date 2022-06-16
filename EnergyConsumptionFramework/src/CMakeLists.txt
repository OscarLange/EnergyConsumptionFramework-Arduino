set(SOURCES main.cpp
            wifi/wifi.cpp tcp/tcp.cpp work/work.c)

idf_component_register(SRCS ${SOURCES}
            INCLUDE_DIRS .  ../include/wifi ../include/tcp ../include/structs
            REQUIRES esp_wifi nvs_flash esp_event esp_netif lwip)