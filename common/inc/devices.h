#ifndef DEVICES_H_
#define DEVICES_H_

#include "stdint.h"
#include "stdbool.h"

#include "state_machine.h"
#include "drive_routine.h"



// Device Struct
typedef struct DeviceStruct {
    int id;
    char* name;
    int period;
    StateMachineStruct state_machine;
    DriveRoutineStruct drive_routine;
} DeviceStruct;


// Device Interface
void InitDevice(DeviceStruct* dv);
void RunDevice(DeviceStruct* dv);

void DeviceSetStateEntity(DeviceStruct* dv, StateEnum state, StateEntityStruct entity, bool is_default);
void DeviceSetRoutineEntity(DeviceStruct* dv, int id, RoutineEntityStruct entity);
void DeviceDelay(DeviceStruct* dv);

#endif //DEVICES_H_