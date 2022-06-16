#pragma once

#include <cstring>
#include <mutex>

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include <iostream>

namespace EnergyConsumptionFramework
{
    #define host_ip "192.168.0.5"
    #define host_port 8090


    class TCPClient
    {
        public:
            TCPClient(void);
            void tcp_client_task(char *payload);

        private:
    };
}