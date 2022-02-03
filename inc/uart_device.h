#ifndef UART_DEVICE_H_
#define UART_DEVICE_H_

#include "stdio.h"

#include "devices.h"

typedef struct UART_Device
{
    DeviceStruct device_handle;

    uint8_t buff[1024];
} UART_Device;


UART_Device uart_dev;


// UART State Functions
void UART_Device_StateOff_Init()
{
    printf("UART Device: Turnning Off\n");
}

void UART_Device_StateDisable_Init()
{
    printf("UART Device: Turnning On\n");
}

void UART_Device_StateDisable_Run()
{
    printf("UART Device: Self Enabling\n");
    StateTrainsition(&uart_dev.device_handle.state_machine, State_Enable);
}

void UART_Device_StateEnable_Init()
{
    printf("UART Device: Device is now Running!\n");
}

void UART_Device_StateEnable_Run()
{
    printf("UART Device: Running...\n");
    // RunDeviceDriver(&uart_dev.device_handle.driver);
}

void UART_Device_StateEnable_Exit()
{
    printf("UART Device: Stop\n");
}

// UART Device Interface
void UART_Device_Init()
{
    // Init State Machine
    uart_dev.device_handle.state_machine.state[State_Off] = CreateStateEntity(
        UART_Device_StateOff_Init,
        NULL,
        NULL
    );

    uart_dev.device_handle.state_machine.state[State_Disable] = CreateStateEntity(
        UART_Device_StateDisable_Init,
        UART_Device_StateDisable_Run,
        NULL
    );

    uart_dev.device_handle.state_machine.state[State_Enable] = CreateStateEntity(
        UART_Device_StateEnable_Init,
        UART_Device_StateEnable_Run,
        UART_Device_StateEnable_Exit
    );

    uart_dev.device_handle.state_machine.state[State_Error] = CreateStateEntity(
        NULL,
        NULL,
        NULL
    );

    InitStateMachine(&uart_dev.device_handle.state_machine);
}


void UART_Device_Run()
{
    RunStateMachine(&uart_dev.device_handle.state_machine);
}

#endif //UART_DEVICE_H_