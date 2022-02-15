#ifndef THREADS_H_
#define THREADS_H_

#include "stdio.h"
#include "pthread.h"

#include "uart_device.h"
#include "eth_device.h"


/* 
 * Simple emulator of FreeRTOS tasks.
 */


#define THREADS_N_TASKS 3

void* Dafult_Task(void* data)
{    
    printf("DO Something\n");
    sleep(1);
}


void* UART_Task(void*data)
{
    UART_Init();
    for(;;) {
        UART_Run();        
    }
}


void* ETH_Task(void*data)
{
    ETH_Init();
    for(;;) {
        ETH_Run();
    }
}



int ThreadsStart()
{    
    pthread_t thread_ptr[THREADS_N_TASKS];
    void* (*task_ptr[THREADS_N_TASKS])(void*);

    task_ptr[0] = Dafult_Task;
    task_ptr[1] = UART_Task;
    task_ptr[2] = ETH_Task;

    for (int i = 0; i < THREADS_N_TASKS; i++) {
        if (pthread_create(&thread_ptr[i], NULL, task_ptr[i], NULL)) {
            printf("Faild to create thread #%d\n", i);
            return -1; 
        }
    }
        
    int status;
    for (int i = 0; i < THREADS_N_TASKS; i++) {
        pthread_join(thread_ptr[i], (void**)&status);
    }
}

#endif //THREADS_H_