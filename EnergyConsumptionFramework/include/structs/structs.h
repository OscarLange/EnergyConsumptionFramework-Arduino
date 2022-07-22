#include <stdint.h>
#include "freertos/FreeRTOS.h"


#define NONE_VALUE "NONE_VALUE"  

#define RUN_TIME 15000

struct TABLE_ENTRY {
    char task_name[configMAX_TASK_NAME_LEN];
    uint32_t elapsed_time;
    uint32_t percentage_time;
};