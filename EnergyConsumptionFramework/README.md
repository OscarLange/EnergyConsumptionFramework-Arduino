# Energy Consumption Framework
This project contains everything needed on the esp32/client side.
It sets up a wifi connection and a tcp client to interact with the server side:

https://github.com/OscarLange/EnergyConsumptionFramework-RasberryPi

It contains a timed tcp call for the other device to start/stop collect the energy metrics.
Also it collects system information on the running tasks.
It also schedules the needed tasks for creating work.

## Folder contents

```
├── CMakeLists.txt
├── include
│   ├── structs 
│   │   └── structs.h           header file for regulating the work to be done
│   ├── tcp 
│   │   └── tcp.h               header file for tcp
│   └── wifi 
│       └── wifi.h              header file for wifi
├── src
│   ├── CMakeLists.txt
│   ├── main.cpp                main loop and setup
│   ├── main.h                  main config file
│   ├── work 
│   │   └── work.c              work file where tasks are created and stats collected
│   ├── tcp 
│   │   └── tcp.cpp             tcp client implementation
│   └── wifi 
│       └── wifi.cpp            wifi implementation
└── README.md                   This is the file you are currently reading
```
