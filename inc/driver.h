#ifndef DRIVER_H_
#define DRIVER_H_

#include "stdio.h"

// Device Drvier
typedef int (*DriverFncPtr) (void);

typedef struct DriverEntityStruct {
    int id;
    char* name;
    DriverFncPtr func;
} DriverEntityStruct;

typedef struct DriverStruct {
    int* ids;
    size_t n_ids;
    DriverEntityStruct* entity;
} DriverStruct;


// Driver Interface

// TODO: Init interface
// TODO: func ptr check

void RunDeviceDriver(DriverStruct* drv)
{
    for(int i = 0; i < drv->n_ids; i++) {
        // TODO: Exception handling
        if ( drv->entity[drv->ids[i]].func()) {
            return;
        }
    }
}



#endif //DRIVER_H_