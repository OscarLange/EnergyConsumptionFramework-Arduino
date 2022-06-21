#include <string>
#include <iostream>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "wifi.h"
#include "tcp.h"
#include <structs.h>
#include <sstream>
#include "esp_pm.h"

class Main final
{
    private:
    public:
        void run(void);
        void setup(void);
        bool send_msg(char *payload);
        //static void task_wrapper_collecting(void* _this);
        //amount of iterations for data collection
        static const int iteration_amount = 10000;
        
        EnergyConsumptionFramework::Wifi::state_e wifiState { EnergyConsumptionFramework::Wifi::state_e::NOT_INITIALIZED };
        EnergyConsumptionFramework::Wifi Wifi;
        EnergyConsumptionFramework::TCPClient TCPClient;
};