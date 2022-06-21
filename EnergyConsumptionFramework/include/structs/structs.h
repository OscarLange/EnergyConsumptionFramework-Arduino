#include <stdint.h>
#include "freertos/FreeRTOS.h"


#define NONE_VALUE "NONE_VALUE"  

//function takes the following frequencies as valid values:
//  240, 160, 80    <<< For all XTAL types
//  40, 20, 10      <<< For 40MHz XTAL
//  26, 13          <<< For 26MHz XTAL
//  24, 12          <<< For 24MHz XTAL
#define CPU_FREQUENCY 80

//VALUES
//SPIN_WORK ADD_WORK
#define ADD_WORK

struct TABLE_ENTRY {
    char task_name[configMAX_TASK_NAME_LEN];
    uint32_t elapsed_time;
    uint32_t percentage_time;
};