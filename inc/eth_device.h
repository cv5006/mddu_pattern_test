#ifndef ETH_DEVICE_H_
#define ETH_DEVICE_H_

#include "stdio.h"
#include "string.h"
#include "errno.h"
#include "time.h"

#include "unistd.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "arpa/inet.h"

#include "devices.h"


#define ETH_BUFF_SIZE 1024
#define ETH_PACKET_REQS 0
#define ETH_PACKET_DVID 1
#define ETH_PACKET_DAID 2
#define ETH_PACKET_ARGS 3


extern DeviceStruct eth_dv;

typedef struct ETH_Data
{    
    int srvsock;
    int clisock;    

    int8_t rx_buff[ETH_BUFF_SIZE];
    int8_t tx_buff[ETH_BUFF_SIZE];
} ETH_Data;

/*
  ___ _____ _  _   ___ _        _         ___             _   _             
 | __|_   _| || | / __| |_ __ _| |_ ___  | __|  _ _ _  __| |_(_)___ _ _  ___
 | _|  | | | __ | \__ \  _/ _` |  _/ -_) | _| || | ' \/ _|  _| / _ \ ' \(_-<
 |___| |_| |_||_| |___/\__\__,_|\__\___| |_| \_,_|_||_\__|\__|_\___/_||_/__/
                                                                                                                                                              
*/
// Off state
void ETH_StateOff_Ent();

// Disable state
void ETH_StateDisable_Ent();

// Enable state
void ETH_StateEnable_Ent();
void ETH_StateEnable_Run();
void ETH_StateEnable_Ext();

// Error state
void ETH_StateError_Run();


/*
  ___ _____ _  _   ___      _           ___          _   _             
 | __|_   _| || | |   \ _ _(_)_ _____  | _ \___ _  _| |_(_)_ _  ___ ___
 | _|  | | | __ | | |) | '_| \ V / -_) |   / _ \ || |  _| | ' \/ -_|_-<
 |___| |_| |_||_| |___/|_| |_|\_/\___| |_|_\___/\_,_|\__|_|_||_\___/__/
                                                                       
*/

int ETH_Routine_Accept();
int ETH_Routine_Recv();
int ETH_Routine_Send();


/*

  ___ _____ _  _   ___          _          ___     _            __             
 | __|_   _| || | |   \ _____ _(_)__ ___  |_ _|_ _| |_ ___ _ _ / _|__ _ __ ___ 
 | _|  | | | __ | | |) / -_) V / / _/ -_)  | || ' \  _/ -_) '_|  _/ _` / _/ -_)
 |___| |_| |_||_| |___/\___|\_/|_\__\___| |___|_||_\__\___|_| |_| \__,_\__\___|
                                                                               
*/ 
void ETH_Init();
void ETH_Run();

/*
  ___ _____ _  _   ___          _          ___     _          _         ___             _   _             
 | __|_   _| || | |   \ _____ _(_)__ ___  | _ \_ _(_)_ ____ _| |_ ___  | __|  _ _ _  __| |_(_)___ _ _  ___
 | _|  | | | __ | | |) / -_) V / / _/ -_) |  _/ '_| \ V / _` |  _/ -_) | _| || | ' \/ _|  _| / _ \ ' \(_-<
 |___| |_| |_||_| |___/\___|\_/|_\__\___| |_| |_| |_|\_/\__,_|\__\___| |_| \_,_|_||_\__|\__|_\___/_||_/__/
                                                                                                          
*/
static void Protocol(int8_t* msg);

#endif //ETH_DEVICE_H_