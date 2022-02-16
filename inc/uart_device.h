#ifndef UART_DEVICE_H_
#define UART_DEVICE_H_

#include "stdio.h"
#include "time.h"

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
// Off state
void UART_StateOff_Ent();

// Disable state
void UART_StateDisable_Ent();
void UART_StateDisable_Run();

// Enable state
void UART_StateEnable_Ent();
void UART_StateEnable_Run();
void UART_StateEnable_Ext();

// Error state
void UART_StateError_Run();

/*
  _   _  _   ___ _____   ___      _           ___          _   _             
 | | | |/_\ | _ \_   _| |   \ _ _(_)_ _____  | _ \___ _  _| |_(_)_ _  ___ ___
 | |_| / _ \|   / | |   | |) | '_| \ V / -_) |   / _ \ || |  _| | ' \/ -_|_-<
  \___/_/ \_\_|_\ |_|   |___/|_| |_|\_/\___| |_|_\___/\_,_|\__|_|_||_\___/__/
                                                                             
*/

int UART_Routine_Hello();
int UART_Routine_Bye();

/*
  _   _  _   ___ _____   ___          _          ___     _            __             
 | | | |/_\ | _ \_   _| |   \ _____ _(_)__ ___  |_ _|_ _| |_ ___ _ _ / _|__ _ __ ___ 
 | |_| / _ \|   / | |   | |) / -_) V / / _/ -_)  | || ' \  _/ -_) '_|  _/ _` / _/ -_)
  \___/_/ \_\_|_\ |_|   |___/\___|\_/|_\__\___| |___|_||_\__\___|_| |_| \__,_\__\___|
                                                                                     
*/ 
void UART_Init();
void UART_Run();

#endif //UART_DEVICE_H_

