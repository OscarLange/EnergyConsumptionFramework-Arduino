#include "main.h"

Main App;

//persistent variable
static RTC_NOINIT_ATTR  int max_freq;

char config_msg[] = "Request config";
char work_msg[] = "Get work";

//send msg over tcp
bool Main::send_msg(char *payload, bool get_info, bool get_work)
{
    vTaskDelay(pdMS_TO_TICKS(100));
    wifiState = Wifi.GetState();

    switch (wifiState)
    {
        case EnergyConsumptionFramework::Wifi::state_e::READY_TO_CONNECT:
            std::cout << "Wifi Status: READY_TO_CONNECT\n";
            Wifi.Begin();
            return 0;
        case EnergyConsumptionFramework::Wifi::state_e::DISCONNECTED:
            std::cout << "Wifi Status: DISCONNECTED\n";
            esp_restart();
            return 0;
        case EnergyConsumptionFramework::Wifi::state_e::CONNECTING:
            std::cout << "Wifi Status: CONNECTING\n";
            return 0;
        case EnergyConsumptionFramework::Wifi::state_e::WAITING_FOR_IP:
            std::cout << "Wifi Status: WAITING_FOR_IP\n";
            return 0;
        case EnergyConsumptionFramework::Wifi::state_e::ERROR:
            std::cout << "Wifi Status: ERROR\n";
            return 0;
        case EnergyConsumptionFramework::Wifi::state_e::CONNECTED:
            std::cout << "Wifi Status: CONNECTED\n";
            TCPClient.tcp_client_task(payload, get_info, get_work);
            return 1;
        case EnergyConsumptionFramework::Wifi::state_e::NOT_INITIALIZED:
            std::cout << "Wifi Status: NOT_INITIALIZED\n";
            return 0;
        case EnergyConsumptionFramework::Wifi::state_e::INITIALIZED:
            std::cout << "Wifi Status: INITIALIZED\n";
            return 0;
    }
    return 0;
}

//function in C to collect statistics
extern "C" void collect_stats(struct TABLE_ENTRY entries[], int work_mode, int cpu_util);

//main work function for collecting stats
void Main::run(void)
{    
    //get frequency
    esp_pm_config_esp32_t pm_config;
    esp_pm_get_configuration(&pm_config);

    //msgs
    char end_msg[] = "Stop collecting";
    std::stringstream start_stream{};
    //send frequency back to make sure the server and client are in sync
    start_stream << "Start collecting:" << pm_config.min_freq_mhz << "," << TCPClient.cpu_util << "," << TCPClient.work_mode;
    auto start = start_stream.str();

    //send msg to rasberry to start collecting
    while(App.send_msg(start.data(), false, false) == false){}

    //stop wifi for stat collection
    esp_wifi_stop();
    
    //collect stats
    struct TABLE_ENTRY entries[11] = {};
    collect_stats(entries, TCPClient.work_mode, TCPClient.cpu_util);

    //start wifi again
    esp_wifi_start();

    //send msg to rasberry to stop collecting
    std::cout << end_msg << "\n";
    while(App.send_msg(end_msg, false, false) == false){}

    //building string
    //encode the collected metrics to be sent to the server
    std::stringstream entries_stream{};
    for(auto &entry: entries)
    {
        std::string s_entry = std::string(entry.task_name);
        if(s_entry.substr(0, sizeof(NONE_VALUE)) != std::string(NONE_VALUE))
        {
            entries_stream << entry.task_name << "," << entry.elapsed_time << "," << entry.percentage_time <<";";
        }
        entries_stream << "MIN_FREQ," << pm_config.min_freq_mhz << ";MAX_FREQ," <<pm_config.max_freq_mhz << ";";
    }
    auto final = entries_stream.str();
 

    //send values
    while(App.send_msg(final.data(), false, false) == false){}

    if(TCPClient.mode == 1)
    {
        //retrieve new work (type of operation, work utilization)
        //Seting work
        while(App.send_msg(work_msg, false, true) == false){}

    } else if(TCPClient.mode == 2)
    {
        //retrieve new frequency
        //send msg to rasberry to get config values
        while(App.send_msg(config_msg, true, false) == false){}
        
        //stop the wifi 
        esp_wifi_stop();
        max_freq = TCPClient.freq;
        vTaskDelay(pdMS_TO_TICKS(500));
        esp_restart();
    }
}

void Main::setup(void)
{
    //get the reason for the reset of esp
    esp_reset_reason_t reason = esp_reset_reason();

    //if its a software reset then setup frequency and wifi
    if(reason == ESP_RST_SW)
    {
        //setup cpu frequency
        esp_pm_config_esp32_t pm_config = {
            .max_freq_mhz = max_freq,
            .min_freq_mhz = max_freq,
            .light_sleep_enable = false
        };
        ESP_ERROR_CHECK( esp_pm_configure(&pm_config) );

        //setup wifi
        esp_event_loop_create_default();
        nvs_flash_init();
        Wifi.SetCredentials("", "");
        Wifi.Init();

        //Seting work
        std::cout << work_msg << "\n";
        while(App.send_msg(work_msg, false, true) == false){}
    } else {
        //setup wifi
        esp_event_loop_create_default();
        nvs_flash_init();
        Wifi.SetCredentials("", "");
        Wifi.Init();

        //send msg to rasberry to get config values
        while(App.send_msg(config_msg, true, false) == false){}
        
        //Stop wifi to reset frequency
        esp_wifi_stop();
        //store frequency for reset
        max_freq = TCPClient.freq;
        vTaskDelay(pdMS_TO_TICKS(500));
        esp_restart();
    }

}

extern "C" void app_main(void)
{
    //Allow other core to finish initialization
    vTaskDelay(pdMS_TO_TICKS(100));

    //connect to wifi
    App.setup();

    int i = 0;

    //the first few values don't make too much sense so wait a bit
    vTaskDelay(pdMS_TO_TICKS(1000));

    //main loop
    while(i < Main::iteration_amount)
    {
        //do the main work
        App.run();
        i++;
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}