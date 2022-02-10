#include "stdio.h"
#include "unistd.h"
#include "time.h"

#include "eth_device.h"

DeviceStruct eth_dv;

int main() {
    // State Machine Test
    int cmd;
    int arg;    

    ETH_Init();
    PushDrviceRoutine(&eth_dv.drive_routine, 0);
    PushDrviceRoutine(&eth_dv.drive_routine, 1);
    while(1){
        ETH_Run();
        usleep(100);
    }
}
