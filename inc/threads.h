#ifndef THREADS_H_
#define THREADS_H_

#include "devices.h"

/* 
 * Simple model of rtos threads. 
 * Note this is not actual threads! 
 */

void ETH_Task()
{
    printf("Hello, ETH!\n");
}

void UART_Task()
{
    printf("Hello, UART!\n");
}

void JointsTask()
{
    printf("Hello, Joints!\n");
}


#endif //THREADS_H_