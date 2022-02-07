#include "stdio.h"
#include "unistd.h"

#include "uart_device.h"

DeviceStruct uart_dv;

int main() {
    // State Machine Test
    int cmd;
    int state;    

    UART_Init();
    while(1){
        printf("Commnad? :");
        scanf("%d %d", &cmd, &state);
        if (cmd == 1) {
            StateTrainsition(&uart_dv.state_machine, (StateEnum)state);
        }

        for (int i=0; i<5; i++){
            UART_Run();
        }
    }

}
