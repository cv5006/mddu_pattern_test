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
    StateMachineStruct state_machine;
    DriveRoutineStruct drive_routine;
} DeviceStruct;


// Device Interface
void InitDevice(DeviceStruct* dv)
{
    InitStateMachine(&dv->state_machine);
    InitDriveRoutine(&dv->drive_routine);
}

void RunDevice(DeviceStruct* dv)
{
    RunStateMachine(&dv->state_machine);
}

void DeviceSetStateEntity(DeviceStruct* dv, StateEnum state, StateEntityStruct entity, bool is_default)
{
    dv->state_machine.entity[state] = entity;

    if (is_default) {
        dv->state_machine.curr_state = state;
        dv->state_machine.prev_state = state;
        dv->state_machine.active_func = entity.on_enter;
    }
}

//TODO: id, order range/duplication check
void DeviceSetRoutineEntity(DeviceStruct* dv, int id, RoutineEntityStruct entity)
{
    dv->drive_routine.entity[id] = entity;
}

// void DeviceSetDriver


#endif //DEVICES_H_