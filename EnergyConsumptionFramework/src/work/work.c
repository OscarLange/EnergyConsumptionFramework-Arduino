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

static char task_name[configMAX_TASK_NAME_LEN] = "work_task";
static SemaphoreHandle_t sync_spin_task;

/*
* Wrapper function for uxTaskGetSystemState
* Internal esp function to get info from task scheduler
*/
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

/*
* reorder and calculate run time statistics for tasks
*/
void calculate_real_time_stats(uint32_t start_run_time, TaskStatus_t *start_array, UBaseType_t start_array_size,
    uint32_t end_run_time, TaskStatus_t *end_array, UBaseType_t end_array_size, 
    struct TABLE_ENTRY entries[])
{
    //Calculate total_elapsed_time in units of run time stats clock period.
    uint32_t total_elapsed_time = (end_run_time - start_run_time);
    if (total_elapsed_time == 0) {
        printf("Total elapsed time does not make sense\n");
        return;
    }

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
            uint32_t percentage_time = (task_elapsed_time * 100UL) / (total_elapsed_time);

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
    volatile int a,b = 30000;
    volatile float f_a, f_b = 30000.300;
    volatile int c = 0;
    volatile float f_c;
    //wait for sync start
    xSemaphoreTake(sync_spin_task, portMAX_DELAY);
    while(true)
    {
        //Consume CPU cycles
        for (int i = 0; i < SPIN_ITER; i++) {
            __asm__ __volatile__("NOP");
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

static void add_task(void *arg)
{
    volatile int a,b = 30000;
    volatile int c = 0;
    //wait for sync start
    xSemaphoreTake(sync_spin_task, portMAX_DELAY);
    while(true)
    {
        for (int i = 0; i < SPIN_ITER; i++) {
            c = a + b;
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

static void sub_task(void *arg)
{
    volatile int a,b = 30000;
    volatile int c = 0;
    //wait for sync start
    xSemaphoreTake(sync_spin_task, portMAX_DELAY);
    while(true)
    {
        for (int i = 0; i < SPIN_ITER; i++) {
            c = a - b;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

static void mul_task(void *arg)
{
    volatile int a,b = 30000;
    volatile int c = 0;
    //wait for sync start
    xSemaphoreTake(sync_spin_task, portMAX_DELAY);
    while(true)
    {
        for (int i = 0; i < SPIN_ITER; i++) {
            c = a * b;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

static void div_task(void *arg)
{
    volatile int a,b = 30000;
    volatile int c = 0;
    //wait for sync start
    xSemaphoreTake(sync_spin_task, portMAX_DELAY);
    while(true)
    {
        for (int i = 0; i < SPIN_ITER; i++) {
            c = a / b;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

static void addf_task(void *arg)
{
    volatile float f_a, f_b = 30000.300;
    volatile float f_c;
    //wait for sync start
    xSemaphoreTake(sync_spin_task, portMAX_DELAY);
    while(true)
    {
        for (int i = 0; i < SPIN_ITER; i++) {
            f_c = f_a + f_b;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

static void subf_task(void *arg)
{
    volatile float f_a, f_b = 30000.300;
    volatile float f_c;
    //wait for sync start
    xSemaphoreTake(sync_spin_task, portMAX_DELAY);
    while(true)
    {
        for (int i = 0; i < SPIN_ITER; i++) {
            f_c = f_a - f_b;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

static void mulf_task(void *arg)
{
    volatile float f_a, f_b = 30000.300;
    volatile float f_c;
    //wait for sync start
    xSemaphoreTake(sync_spin_task, portMAX_DELAY);
    while(true)
    {
        for (int i = 0; i < SPIN_ITER; i++) {
            f_c = f_a * f_b;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

static void divf_task(void *arg)
{
    volatile float f_a, f_b = 30000.300;
    volatile float f_c;
    //wait for sync start
    xSemaphoreTake(sync_spin_task, portMAX_DELAY);
    while(true)
    {
        for (int i = 0; i < SPIN_ITER; i++) {
            f_c = f_a / f_b;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void collect_stats(struct TABLE_ENTRY entries[], int work_mode, int cpu_util)
{
    //cpu util 0-100
    int work_time = (RUN_TIME / 100) * cpu_util;
    int suspended_time = RUN_TIME - work_time;

    //Allow other core to finish initialization
    vTaskDelay(pdMS_TO_TICKS(100));
    xTaskHandle work_handle;

    //Create semaphores to synchronize
    sync_spin_task = xSemaphoreCreateBinary();

    if(work_mode == 0)
    {
        xTaskCreatePinnedToCore(spin_task, task_name, 1024, NULL, SPIN_TASK_PRIO, &work_handle, 1);
    } else if(work_mode == 1)
    {
        xTaskCreatePinnedToCore(add_task, task_name, 1024, NULL, SPIN_TASK_PRIO, &work_handle, 1);
    } else if(work_mode == 2)
    {
        xTaskCreatePinnedToCore(sub_task, task_name, 1024, NULL, SPIN_TASK_PRIO, &work_handle, 1);
    } else if(work_mode == 3)
    {
        xTaskCreatePinnedToCore(mul_task, task_name, 1024, NULL, SPIN_TASK_PRIO, &work_handle, 1);
    } else if(work_mode == 4)
    {
        xTaskCreatePinnedToCore(div_task, task_name, 1024, NULL, SPIN_TASK_PRIO, &work_handle, 1);
    }else if(work_mode == 5)
    {
        xTaskCreatePinnedToCore(addf_task, task_name, 1024, NULL, SPIN_TASK_PRIO, &work_handle, 1);
    } else if(work_mode == 6)
    {
        xTaskCreatePinnedToCore(subf_task, task_name, 1024, NULL, SPIN_TASK_PRIO, &work_handle, 1);
    } else if(work_mode == 7)
    {
        xTaskCreatePinnedToCore(mulf_task, task_name, 1024, NULL, SPIN_TASK_PRIO, &work_handle, 1);
    } else
    {
        xTaskCreatePinnedToCore(divf_task, task_name, 1024, NULL, SPIN_TASK_PRIO, &work_handle, 1);
    }

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

    //the following part is to achieve different cpu utilizations
    //actuall work time for other processor/task
    vTaskDelay(pdMS_TO_TICKS(work_time/2));
    vTaskSuspend( work_handle );

    //suspended time
    vTaskDelay(pdMS_TO_TICKS(suspended_time));

    //second work time
    vTaskResume( work_handle );
    vTaskDelay(pdMS_TO_TICKS(work_time/2));

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

    //stop the working task to be done
    vTaskDelete(work_handle);
}