#ifndef ETH_DEVICE_H_
#define ETH_DEVICE_H_

#include "stdio.h"
#include "string.h"
#include "errno.h"

#include "unistd.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "arpa/inet.h"

#include "devices.h"


#define ETH_BUFF_SIZE 1024

extern DeviceStruct eth_dv;

typedef struct ETH_Data
{    
    int srvsock;
    int clisock;    

    char rx_buff[ETH_BUFF_SIZE];
    char tx_buff[ETH_BUFF_SIZE];
} ETH_Data;

ETH_Data eth_data;

/*
  ___ _____ _  _   ___ _        _         ___             _   _             
 | __|_   _| || | / __| |_ __ _| |_ ___  | __|  _ _ _  __| |_(_)___ _ _  ___
 | _|  | | | __ | \__ \  _/ _` |  _/ -_) | _| || | ' \/ _|  _| / _ \ ' \(_-<
 |___| |_| |_||_| |___/\__\__,_|\__\___| |_| \_,_|_||_\__|\__|_\___/_||_/__/
                                                                                                                                                              
*/
void ETH_StateOff_Ent()
{
    printf("ETH Device: Turned off. Go to Disable state\n");
    StateTrainsition(&eth_dv.state_machine, State_Disable);
}

void ETH_StateDisable_Ent()
{
    printf("ETH Device: Disabled. ");

    if (eth_data.srvsock) {
        close(eth_data.srvsock);
        printf("Socket closed.\n");
    }else{
        printf("\n");
    }
}

void ETH_StateEnable_Ent()
{
    printf("ETH Device: Enabeld. Socket creation begin\n");

    // Create
    eth_data.srvsock = socket(AF_INET, SOCK_STREAM, 0);
    if (eth_data.srvsock < 0) {
        printf("ETH Device: [Socket Error] %s\n", strerror(errno));
		StateTrainsition(&eth_dv.state_machine, State_Disable);
	}
    int on = 1;
    if(setsockopt(eth_data.srvsock, SOL_SOCKET, SO_REUSEADDR, (const char*) &on, sizeof(on)) < 0){
        printf("ETH Device: [Socket Error] %s\n", strerror(errno));
        StateTrainsition(&eth_dv.state_machine, State_Disable);
    }
    printf("ETH Device: [Socket Created]\n");

    // Bind
    struct sockaddr_in srvaddr;
    srvaddr.sin_addr.s_addr = INADDR_ANY;
    srvaddr.sin_port = htons(8765);
    srvaddr.sin_family = AF_INET;

    if(bind(eth_data.srvsock, (struct sockaddr *) &srvaddr, sizeof(srvaddr)) < 0) {
        printf("ETH Device: [Bind Error] %s\n", strerror(errno));
        StateTrainsition(&eth_dv.state_machine, State_Disable);
    }
    printf("ETH Device: [Bind Successed]\n");

    // Listen
    printf("ETH Device: Waiting for client...\n");
    if(listen(eth_data.srvsock, 1) < 0) {
        printf("ETH Device: [Listen Error] %s\n", strerror(errno));
        StateTrainsition(&eth_dv.state_machine, State_Disable);
    }
}

void ETH_StateEnable_Run()
{
    RunDriveRoutines(&eth_dv.drive_routine);
}

void ETH_StateEnable_Ext()
{
    close(eth_data.srvsock);
    close(eth_data.clisock);
}

void ETH_StateError_Run()
{
    printf("ETH Device: Error!\n");
}


/*
  ___ _____ _  _   ___      _           ___          _   _             
 | __|_   _| || | |   \ _ _(_)_ _____  | _ \___ _  _| |_(_)_ _  ___ ___
 | _|  | | | __ | | |) | '_| \ V / -_) |   / _ \ || |  _| | ' \/ -_|_-<
 |___| |_| |_||_| |___/|_| |_|\_/\___| |_|_\___/\_,_|\__|_|_||_\___/__/
                                                                       
*/

int ETH_Routine_Accept()
{
    if (!eth_data.clisock) {
        struct sockaddr_in cliaddr;
        int cliaddr_size = sizeof(cliaddr);

        eth_data.clisock = accept(eth_data.srvsock, (struct sockaddr *) &cliaddr, (socklen_t *) &cliaddr_size);
        if(eth_data.clisock < 0) {
            printf("ETH Device: [Accept Error] %s\n", strerror(errno));
            StateTrainsition(&eth_dv.state_machine, State_Disable);
            return -1;
        }

        printf("ETH Device: [Accepted] %s:%d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
        return 1;
    }
    return 0;
}

int ETH_Routine_Recv()
{
    int recv_len = recv(eth_data.clisock, eth_data.rx_buff, ETH_BUFF_SIZE, 0);
    if (recv_len < 0) {
        printf("ETH Device: [Recv Error] %s\n", strerror(errno));
        return -1;
    } 
    
    if (recv_len == 0) {
        printf("ETH Device: Client closed socket\n");
        close(eth_data.clisock);
        eth_data.clisock = 0;
        return 0;
    }

    char* recv_data;
    printf("ETH Device: [Recv: %d] %.*s\n", recv_len, recv_len, eth_data.rx_buff);
    return recv_len;
}

int ETH_Routine_Send()
{

}

/*
  ___ _____ _  _   ___          _          ___     _            __             
 | __|_   _| || | |   \ _____ _(_)__ ___  |_ _|_ _| |_ ___ _ _ / _|__ _ __ ___ 
 | _|  | | | __ | | |) / -_) V / / _/ -_)  | || ' \  _/ -_) '_|  _/ _` / _/ -_)
 |___| |_| |_||_| |___/\___|\_/|_\__\___| |___|_||_\__\___|_| |_| \__,_\__\___|
                                                                               
*/ 
void ETH_Init()
{
    InitDevice(&eth_dv);

    // State Machine
    StateEntityStruct eth_off     = CreateStateEntity(ETH_StateOff_Ent,     NULL,                NULL               );
    StateEntityStruct eth_disable = CreateStateEntity(ETH_StateDisable_Ent, NULL,                NULL               );
    StateEntityStruct eth_enable  = CreateStateEntity(ETH_StateEnable_Ent,  ETH_StateEnable_Run, ETH_StateEnable_Ext);
    StateEntityStruct eth_error   = CreateStateEntity(NULL,                 ETH_StateError_Run,  NULL               );

    DeviceSetStateEntity(&eth_dv, State_Off,     eth_off,     false);
    DeviceSetStateEntity(&eth_dv, State_Disable, eth_disable, false);
    DeviceSetStateEntity(&eth_dv, State_Enable,  eth_enable,  true);
    DeviceSetStateEntity(&eth_dv, State_Error,   eth_error,   false);

    // Drive Routine
    RoutineEntityStruct eth_accept = CreateRoutineEntity("eth_accept", ETH_Routine_Accept);
    RoutineEntityStruct eth_recv   = CreateRoutineEntity("eth_recv",   ETH_Routine_Recv);
    RoutineEntityStruct eth_send   = CreateRoutineEntity("eth_send",   ETH_Routine_Send);

    DeviceSetRoutineEntity(&eth_dv, 0, eth_accept);
    DeviceSetRoutineEntity(&eth_dv, 1, eth_recv);
    DeviceSetRoutineEntity(&eth_dv, 2, eth_send);
}

void ETH_Run()
{
    RunDevice(&eth_dv);
}

#endif //ETH_DEVICE_H_