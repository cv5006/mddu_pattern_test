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

void* Dafult_Task(void* data);
void* UART_Task(void*data);
void* ETH_Task(void*data);

int ThreadsStart();

#endif //THREADS_H_