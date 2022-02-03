#include "stdio.h"
#include "unistd.h"

#include "uart_device.h"

UART_Device uart_dev;

int main() {
    // State Machine Test
    int cmd;
    int state;    

    UART_Device_Init();
    while(1){
        printf("Commnad? :");
        scanf("%d %d", &cmd, &state);
        if (cmd == 1) {
            StateTrainsition(&uart_dev.device_handle.state_machine, (StateEnum)state);
        }

        for (int i=0; i<5; i++){
            UART_Device_Run();
        }
    }

}
