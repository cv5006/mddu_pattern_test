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

RoutineEntityStruct CreateRoutineEntity(char* name, RoutineFncPtr func);


// Drive Routine Struct
typedef struct DriveRoutineStruct {
    int id[DRIVE_ROUTINE_MAX_ENTITIES];
    size_t n_id;
    RoutineEntityStruct entity[DRIVE_ROUTINE_MAX_ENTITIES];
} DriveRoutineStruct;


// DriveRoutine Interface
void InitDriveRoutine(DriveRoutineStruct* drv);

int RunDriveRoutines(DriveRoutineStruct* drv);

void ClearDriveRoutines(DriveRoutineStruct* drv);

int PushDrviceRoutine(DriveRoutineStruct* drv, int id);


#endif //DRIVE_ROUTINE_H_