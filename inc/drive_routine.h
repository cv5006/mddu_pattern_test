#ifndef DRIVE_ROUTINE_H_
#define DRIVE_ROUTINE_H_

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define DRIVE_ROUTINE_MAX_ENTITIES 32
#define DRIVE_ROUTINE_DEFAULT_ID -1

// Routine Entity
typedef int (*RoutineFncPtr) (void);

typedef struct RoutineEntityStruct {
    char* name;
    RoutineFncPtr func;
} RoutineEntityStruct;

RoutineEntityStruct CreateRoutineEntity(char* name, RoutineFncPtr func)
{
    RoutineEntityStruct res;
    res.name = (char*)malloc(strlen(name));
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

void ClearDriveRoutines(DriveRoutineStruct* drv)
{
    for (int i = 0; i < DRIVE_ROUTINE_MAX_ENTITIES; i++) {
        drv->id[i] = DRIVE_ROUTINE_DEFAULT_ID;
    }
    drv->n_id = 0;
}

int PushDrviceRoutine(DriveRoutineStruct* drv, int id)
{
    if (drv->n_id >= DRIVE_ROUTINE_MAX_ENTITIES) {
        return -1;
    }

    if (drv->entity[id].func == NULL) {
        return -2;
    }
    drv->id[drv->n_id++] = id;
}


#endif //DRIVE_ROUTINE_H_