set(SOURCES main.cpp
            wifi/wifi.cpp tcp/tcp.cpp work/work.c bluetooth/gatt_server.c)

idf_component_register(SRCS ${SOURCES}
            INCLUDE_DIRS .  ../include/wifi ../include/tcp ../include/structs
            REQUIRES bt esp_wifi nvs_flash esp_event esp_netif lwip)