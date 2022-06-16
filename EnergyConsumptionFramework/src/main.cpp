#include "main.h"

Main App;

//send msg over tcp
bool Main::send_msg(char *payload)
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
            Wifi.Begin();
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
            TCPClient.tcp_client_task(payload);
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

extern "C" void collect_stats(struct TABLE_ENTRY entries[]);

//main work function for collecting stats
void Main::run(void)
{
    //msgs
    char start_msg[] = "Start collecting";
    char end_msg[] = "Stop collecting";

    //send msg to rasberry to start collecting
    std::cout << start_msg << "\n";
    while(App.send_msg(start_msg) == false){}
    
    //collect stats
    struct TABLE_ENTRY entries[11] = {};
    collect_stats(entries);

    //send msg to rasberry to stop collecting
    std::cout << end_msg << "\n";
    while(App.send_msg(end_msg) == false){}

    //building string
    std::stringstream entries_stream{};
    for(auto &entry: entries)
    {
        std::string s_entry = std::string(entry.task_name);
        std::cout << "name: " << s_entry << "\n";
        if(s_entry.substr(0, sizeof(NONE_VALUE)) != std::string(NONE_VALUE))
        {
            entries_stream << entry.task_name << "," << entry.elapsed_time << "," << entry.percentage_time << ";";
        }
    }
    auto final = entries_stream.str();
 
    std::cout << "final string: " << final << "\n";

    //send values
    while(App.send_msg(final.data()) == false){}
}

void Main::setup(void)
{
    esp_event_loop_create_default();
    nvs_flash_init();
    Wifi.SetCredentials("PYUR 6C93E", "BjJZ44b2B7hb");
    Wifi.Init();
}

extern "C" void app_main(void)
{
    //Allow other core to finish initialization
    vTaskDelay(pdMS_TO_TICKS(100));

    //connect to wifi
    App.setup();

    int i = 0;

    //main loop
    while(i < Main::iteration_amount)
    {
        //do the main work
        App.run();
        i++;
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}