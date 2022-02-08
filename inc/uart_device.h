#ifndef UART_DEVICE_H_
#define UART_DEVICE_H_

#include "stdio.h"

#include "devices.h"


extern DeviceStruct uart_dv;

typedef struct UART_Data
{    
    int8_t buff[1024];
} UART_Data;

/*
  _   _  _   ___ _____   ___ _        _         ___             _   _             
 | | | |/_\ | _ \_   _| / __| |_ __ _| |_ ___  | __|  _ _ _  __| |_(_)___ _ _  ___
 | |_| / _ \|   / | |   \__ \  _/ _` |  _/ -_) | _| || | ' \/ _|  _| / _ \ ' \(_-<
  \___/_/ \_\_|_\ |_|   |___/\__\__,_|\__\___| |_| \_,_|_||_\__|\__|_\___/_||_/__/
                                                                                  
*/
void UART_StateOff_Ent()
{
    printf("UART Device: Turnning Off\n");
}

void UART_StateDisable_Ent()
{
    printf("UART Device: Turnning On\n");
}

void UART_StateDisable_Run()
{
    printf("UART Device: Self Enabling\n");
    StateTrainsition(&uart_dv.state_machine, State_Enable);
}

void UART_StateEnable_Ent()
{
    printf("UART Device: Device is now Running!\n");
}

void UART_StateEnable_Run()
{
    // if (uart_dv.drive_routine.n_id < 1) {
        // printf("UART Device: Nothing to execute\n");
        // return;
    // }
    RunDriveRoutines(&uart_dv.drive_routine);
}

void UART_StateEnable_Ext()
{
    printf("UART Device: Stop\n");
}

void UART_StateError_Run()
{
    printf("UART Device: Error!\n");
}


/*
  _   _  _   ___ _____   ___      _           ___          _   _             
 | | | |/_\ | _ \_   _| |   \ _ _(_)_ _____  | _ \___ _  _| |_(_)_ _  ___ ___
 | |_| / _ \|   / | |   | |) | '_| \ V / -_) |   / _ \ || |  _| | ' \/ -_|_-<
  \___/_/ \_\_|_\ |_|   |___/|_| |_|\_/\___| |_|_\___/\_,_|\__|_|_||_\___/__/
                                                                             
*/

int UART_Routine_Hello()
{
    printf("UART Device: Hello UART!\n");
}

int UART_Routine_Bye()
{
    printf("UART Device: Bye UART!\n");
}

/*
  _   _  _   ___ _____   ___          _          ___     _            __             
 | | | |/_\ | _ \_   _| |   \ _____ _(_)__ ___  |_ _|_ _| |_ ___ _ _ / _|__ _ __ ___ 
 | |_| / _ \|   / | |   | |) / -_) V / / _/ -_)  | || ' \  _/ -_) '_|  _/ _` / _/ -_)
  \___/_/ \_\_|_\ |_|   |___/\___|\_/|_\__\___| |___|_||_\__\___|_| |_| \__,_\__\___|
                                                                                     
*/ 
void UART_Init()
{
    InitDevice(&uart_dv);

    // State Machine
    StateEntityStruct uart_off     = CreateStateEntity(UART_StateOff_Ent,     NULL,                  NULL);
    StateEntityStruct uart_enable  = CreateStateEntity(UART_StateEnable_Ent,  UART_StateEnable_Run,  UART_StateEnable_Ext);
    StateEntityStruct uart_disable = CreateStateEntity(UART_StateDisable_Ent, UART_StateDisable_Run, NULL);
    StateEntityStruct uart_error   = CreateStateEntity(NULL,                  UART_StateError_Run,   NULL);

    DeviceSetStateEntity(&uart_dv, State_Off,     uart_off,     true);
    DeviceSetStateEntity(&uart_dv, State_Disable, uart_disable, false);
    DeviceSetStateEntity(&uart_dv, State_Enable,  uart_enable,  false);
    DeviceSetStateEntity(&uart_dv, State_Error,   uart_error,   false);

    // Drive Routine
    RoutineEntityStruct uart_hello = CreateRoutineEntity("uart_hello", UART_Routine_Hello);
    RoutineEntityStruct uart_bye = CreateRoutineEntity("uart_bye", UART_Routine_Bye);

    DeviceSetRoutineEntity(&uart_dv, 2, uart_hello);
    DeviceSetRoutineEntity(&uart_dv, 5, uart_bye);
}

void UART_Run()
{
    RunDevice(&uart_dv);
}

#endif //UART_DEVICE_H_