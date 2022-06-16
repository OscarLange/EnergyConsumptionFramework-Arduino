/*
* https://github.com/espressif/esp-idf/blob/master/examples/system/freertos/real_time_stats/main/real_time_stats_example_main.c
*/
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include <stdint.h>
#include <string.h>
#include <structs.h>

//Actual CPU cycles used will depend on compiler optimization
#define SPIN_ITER           50000000  
#define SPIN_TASK_PRIO      1
#define STATS_TASK_PRIO     2
#define STATS_TICKS         pdMS_TO_TICKS(100000)
//Increase this if print_real_time_stats returns ESP_ERR_INVALID_SIZE
#define ARRAY_SIZE_OFFSET   5 

static char task_name[configMAX_TASK_NAME_LEN] = "spin_task";
static SemaphoreHandle_t sync_spin_task;
static SemaphoreHandle_t sync_spin_done;
static SemaphoreHandle_t sync_done;

uint32_t get_real_time_stats(TaskStatus_t *array, UBaseType_t array_size)
{
    uint32_t run_time;

    //Allocate array to store current task states
    if (array == NULL) {
        printf("Malloc failed\n");
        return UINT32_MAX;
    }
    //Get current task states
    array_size = uxTaskGetSystemState(array, array_size, &run_time);
    if (array_size == 0) {
        printf("No tasks to get\n");
        return UINT32_MAX;
    }
    return run_time;
}

void calculate_real_time_stats(uint32_t start_run_time, TaskStatus_t *start_array, UBaseType_t start_array_size,
    uint32_t end_run_time, TaskStatus_t *end_array, UBaseType_t end_array_size, 
    struct TABLE_ENTRY entries[])
{
    printf("Start array size: %d, end array size %d", start_array_size, end_array_size);
    //Calculate total_elapsed_time in units of run time stats clock period.
    uint32_t total_elapsed_time = (end_run_time - start_run_time);
    printf("Start run time: %d , end run time: %d", start_run_time, end_run_time);
    if (total_elapsed_time == 0) {
        printf("Total elapsed time does not make sense\n");
        return;
    }

    printf("| Task | Run Time | Percentage\n");
    //Match each task in start_array to those in the end_array
    for (int i = 0; i < start_array_size; i++) {
        int k = -1;
        for (int j = 0; j < end_array_size; j++) {
            if (end_array[j].xHandle != NULL && start_array[i].xHandle == end_array[j].xHandle) {
                k = j;
                //Mark that task have been matched by overwriting their handles
                start_array[i].xHandle = NULL;
                end_array[j].xHandle = NULL;
                break;
            }
        }
        //Check if matching task found
        if (k >= 0) {
            uint32_t task_elapsed_time = end_array[k].ulRunTimeCounter - start_array[i].ulRunTimeCounter;
            uint32_t percentage_time = (task_elapsed_time * 100UL) / (total_elapsed_time * portNUM_PROCESSORS);
            printf("| %s | %d | %d%%\n", start_array[i].pcTaskName, task_elapsed_time, percentage_time);

            //create entry in struct
            memcpy(entries[i].task_name, start_array[i].pcTaskName, configMAX_TASK_NAME_LEN * sizeof(char));
            entries[i].elapsed_time = task_elapsed_time;
            entries[i].percentage_time = percentage_time;
        } else {
            memcpy(entries[i].task_name, NONE_VALUE, 10 * sizeof(char));
        }
    }

    return;
}

static void spin_task(void *arg)
{
    //wait for sync start
    xSemaphoreTake(sync_spin_task, portMAX_DELAY);
    //cycle until given semaphore
    while(true)
    {
        if(xSemaphoreTake(sync_spin_done, pdMS_TO_TICKS(100)) == pdTRUE){
            break;
        }
        //Consume CPU cycles
        for (int i = 0; i < SPIN_ITER; i++) {
            __asm__ __volatile__("NOP");
        }
    }
    //terminate task
    vTaskDelay(pdMS_TO_TICKS(100));
    xSemaphoreGive(sync_done);
    vTaskDelete(NULL);
}

void collect_stats(struct TABLE_ENTRY entries[])
{
    //Allow other core to finish initialization
    vTaskDelay(pdMS_TO_TICKS(100));

    //Create semaphores to synchronize
    sync_spin_task = xSemaphoreCreateBinary();
    sync_spin_done = xSemaphoreCreateBinary();
    sync_done = xSemaphoreCreateBinary();

    printf("Task created %s", task_name);
    xTaskCreatePinnedToCore(spin_task, task_name, 1024, NULL, SPIN_TASK_PRIO, NULL, tskNO_AFFINITY);

    //allocate stats values
    TaskStatus_t *start_array = NULL, *end_array = NULL;
    UBaseType_t start_array_size, end_array_size;
    uint32_t start_run_time = UINT32_MAX;
    uint32_t end_run_time = UINT32_MAX;

    //Allocate array to store current task states
    start_array_size = uxTaskGetNumberOfTasks() + ARRAY_SIZE_OFFSET;
    start_array = malloc(sizeof(TaskStatus_t) * start_array_size);

    //Start the spin task
    xSemaphoreGive(sync_spin_task);

    //Try until succesfully gotten real time stats
    start_run_time = get_real_time_stats(start_array, start_array_size);
    while (start_run_time == UINT32_MAX) {
        start_run_time = get_real_time_stats(start_array, start_array_size);
        printf("Error getting real time stats\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    vTaskDelay(pdMS_TO_TICKS(10000));

    //Allocate array to store current task states
    end_array_size = uxTaskGetNumberOfTasks() + ARRAY_SIZE_OFFSET;
    end_array = malloc(sizeof(TaskStatus_t) * end_array_size);

    ///Try until succesfully gotten real time stats
    end_run_time = get_real_time_stats(end_array, end_array_size);
    while (end_run_time == UINT32_MAX) {
        end_run_time = get_real_time_stats(end_array, end_array_size);
        printf("Error getting real time stats\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    //calculate the real_time_stats
    calculate_real_time_stats(start_run_time, start_array, start_array_size, end_run_time, end_array, end_array_size, entries);

    printf("Entry 0: %s", entries[0].task_name);

    //wait for all spin tasks to be done
    xSemaphoreGive(sync_spin_done);
    //wait for ack
    xSemaphoreTake(sync_done, portMAX_DELAY);
}