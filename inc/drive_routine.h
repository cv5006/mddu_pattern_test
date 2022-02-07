#ifndef DRIVE_ROUTINE_H_
#define DRIVE_ROUTINE_H_

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define DRIVE_ROUTINE_MAX_ENTITIES 32

// Routine Entity
typedef int (*RoutineFncPtr) (void);

typedef struct RoutineEntityStruct {
    char* name;
    RoutineFncPtr func;
} RoutineEntityStruct;

RoutineEntityStruct CreateRoutineEntity(char* name, RoutineFncPtr func)
{
    RoutineEntityStruct res;
    res.name = malloc(strlen(name));
    strcpy(res.name, name);
    res.func = func;
    return res;
}


// Drive Routine Struct
typedef struct DriveRoutineStruct {
    int id[DRIVE_ROUTINE_MAX_ENTITIES];
    size_t n_id;
    RoutineEntityStruct entity[DRIVE_ROUTINE_MAX_ENTITIES];
} DriveRoutineStruct;


// DriveRoutine Interface
void InitDriveRoutine(DriveRoutineStruct* drv)
{
    for (int i = 0; i < DRIVE_ROUTINE_MAX_ENTITIES; i++) {
        drv->id[i] = 0;
        drv->entity[i].func = NULL;
    }
    drv->n_id = 0;
}

int RunDriveRoutines(DriveRoutineStruct* drv)
{
    for (int i = 0; i < drv->n_id; i++){
        //TODO: routine func exception handling
        drv->entity[drv->id[i]].func();
    }
}

//TODO: Set, clear routine id functions

#endif //DRIVE_ROUTINE_H_