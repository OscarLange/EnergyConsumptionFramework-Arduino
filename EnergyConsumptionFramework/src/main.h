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
        /*
        * Main loop that runs continously
        */
        void run(void);
        /*
        * One time execution that sets up imortant functionalities such as the WiFi
        */
        void wifi_setup(void);
        /*
        * One time execution that sets up imortant functionalities such as the Bluetooth
        */
        void ble_setup(void);
        /*
        * Main loop that runs continously with ble
        */
        void run_ble(void);
        /*
        * Sending a msg over tcp
        */
        bool send_msg(char *payload, bool get_info, bool get_work);
        //amount of iterations for data collection
        static const int iteration_amount = 10000;
        
        //wifi and tcp class
        EnergyConsumptionFramework::Wifi::state_e wifiState { EnergyConsumptionFramework::Wifi::state_e::NOT_INITIALIZED };
        EnergyConsumptionFramework::Wifi Wifi;
        EnergyConsumptionFramework::TCPClient TCPClient;
};