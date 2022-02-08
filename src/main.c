#include "stdio.h"
#include "unistd.h"

#include "uart_device.h"

DeviceStruct uart_dv;

int main() {
    // State Machine Test
    int cmd;
    int arg;    

    UART_Init();
    while(1){
        printf("Commnad? :");
        scanf("%d %d", &cmd, &arg);
        switch (cmd) {
        case 1:
            StateTrainsition(&uart_dv.state_machine, (StateEnum)arg);
            break;
        case 2:
            switch (PushDrviceRoutine(&uart_dv.drive_routine, arg)) {
            case -1:
                ClearDriveRoutines(&uart_dv.drive_routine);
                printf("UART Device: Routine list cleared\n");
                break;
            case -2:
                printf("UART Device: Invalid routine ID\n");
                break;
            default:
                printf("UART Device: Set routine id%d:%s\n", arg, uart_dv.drive_routine.entity[arg].name);
                break;
            }
        default:
            break;
        }

        for (int i=0; i<5; i++){
            UART_Run();
        }
    }

}
