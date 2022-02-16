#include "drive_routine.h"

// Routine Entity
RoutineEntityStruct CreateRoutineEntity(char* name, RoutineFncPtr func)
{
    RoutineEntityStruct res;
    res.name = (char*)malloc(strlen(name));
    strcpy(res.name, name);
    res.func = func;
    return res;
}


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
        int res = drv->entity[drv->id[i]].func();
        if (res < 0) {
            return res;
        }
    }
    return 0;
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
    return 0;
}
