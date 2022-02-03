#ifndef DEVICES_H_
#define DEVICES_H_

#include "stdint.h"

#include "state_machine.h"
#include "driver.h"


// Device Struct
typedef struct DeviceStruct {
    int id;
    char* name;
    StateMachineStruct state_machine;
    DriverStruct driver;
} DeviceStruct;




#endif //DEVICES_H_