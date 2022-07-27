# Energy Consumption Framework
This project contains everything needed on the esp32/client side.
It sets up a wifi connection and a tcp client to interact with the server side:

https://github.com/OscarLange/EnergyConsumptionFramework-RasberryPi

It contains a timed tcp call for the other device to start/stop collect the energy metrics.
Also it collects system information on the running tasks.
It also schedules the needed tasks for creating work.

## Necessary Configurations
Some configurations have to be enabled through idf.py menuconfig:
1. Bluetooth:
    - CONFIG_BT_ENABLED
    - CONFIG_BTDM_CTRL_MODE_BLE_ONLY
    - CONFIG_BT_BLUEDROID_ENABLED
    - CONFIG_BT_BLE_ENABLED
    - CONFIG_BT_GATTS_ENABLE
2. Disable watchdog:
    - Open menuconfig: make menuconfig
    - Enter component config
    - Enter ESP32-specific
    - Remove star from interrupt watchdog


## Folder contents

```
├── CMakeLists.txt
├── include
│   ├── structs 
│   │   └── structs.h           header file for regulating the work to be done
│   ├── tcp 
│   │   └── tcp.h               header file for tcp
│   ├── tcp 
│   │   └── tcp.h               header file for tcp
│   └── wifi 
│       └── wifi.h              header file for wifi
├── src
│   ├── CMakeLists.txt
│   ├── main.cpp                main loop and setup
│   ├── main.h                  main config file
│   ├── bluetooth 
│   │   └── gatt_server.c       gatt ble implementation
│   ├── work 
│   │   └── work.c              work file where tasks are created and stats collected
│   ├── tcp 
│   │   └── tcp.cpp             tcp client implementation
│   └── wifi 
│       └── wifi.cpp            wifi implementation
└── README.md                   This is the file you are currently reading
```
