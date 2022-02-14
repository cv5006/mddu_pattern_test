#ifndef THREADS_H_
#define THREADS_H_

#include "stdio.h"
#include "time.h"
#include "pthread.h"

/* 
 * Simple emulator of FreeRTOS tasks.
 */

void* Dafult_Task()
{    
    printf("DO SomeThing\n");
    sleep(1);
}


int ThreadsStart()
{
    pthread_t default_task_ptr;
    if (pthread_create(&default_task_ptr, NULL, Dafult_Task, NULL)) {return -1; }
    
    
    int status;
    pthread_join(default_task_ptr, (void**)&status);
}

#endif //THREADS_H_