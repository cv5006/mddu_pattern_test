#include "eth_device.h"

DeviceStruct eth_dv;
DeviceStruct uart_dv;

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
    sleep(1);
    
    printf("ETH Device: Restart ETH device.\n");
    StateTrainsition(&eth_dv.state_machine, State_Enable);
}

void ETH_StateEnable_Ent()
{
    printf("ETH Device: Enabeld. Socket creation begin\n");
    if (SocketBegin() < 0) {
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
    DeviceDelay(&eth_dv);
}


/*
  ___ _____ _  _   ___      _           ___          _   _             
 | __|_   _| || | |   \ _ _(_)_ _____  | _ \___ _  _| |_(_)_ _  ___ ___
 | _|  | | | __ | | |) | '_| \ V / -_) |   / _ \ || |  _| | ' \/ -_|_-<
 |___| |_| |_||_| |___/|_| |_|\_/\___| |_|_\___/\_,_|\__|_|_||_\___/__/
                                                                       
*/
int ETH_Routine_Echo()
{
    if (!eth_data.clisock) {
        if (SocketAccept() < 0) {
            return -2;
        }
    }
    if (SocketRecv() < 0) {
        return -1;
    }
    Protocol(eth_data.rx_buff);
    SocketSend();
    return 0;
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
    eth_dv.period = 1;

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
    RoutineEntityStruct eth_echo   = CreateRoutineEntity("eth_echo",   ETH_Routine_Echo);

    DeviceSetRoutineEntity(&eth_dv, 1, eth_echo);

    // MEMO: Default routines?
    PushDrviceRoutine(&eth_dv.drive_routine, 1);
}

void ETH_Run()
{
    RunDevice(&eth_dv);
}


/*
  ___ _____ _  _   ___          _          ___     _          _         ___             _   _             
 | __|_   _| || | |   \ _____ _(_)__ ___  | _ \_ _(_)_ ____ _| |_ ___  | __|  _ _ _  __| |_(_)___ _ _  ___
 | _|  | | | __ | | |) / -_) V / / _/ -_) |  _/ '_| \ V / _` |  _/ -_) | _| || | ' \/ _|  _| / _ \ ' \(_-<
 |___| |_| |_||_| |___/\___|\_/|_\__\___| |_| |_| |_|\_/\__,_|\__\___| |_| \_,_|_||_\__|\__|_\___/_||_/__/
                                                                                                          
*/
static int SocketBegin()
{
    // Create
    eth_data.srvsock = socket(AF_INET, SOCK_STREAM, 0);
    if (eth_data.srvsock < 0) {
        printf("ETH Device: [Socket Error] %s\n", strerror(errno));
		return -1;
	}
    int on = 1;
    if(setsockopt(eth_data.srvsock, SOL_SOCKET, SO_REUSEADDR, (const char*) &on, sizeof(on)) < 0){
        printf("ETH Device: [Socket Error] %s\n", strerror(errno));
        return -1;
    }
    printf("ETH Device: [Socket Created]\n");

    // Bind
    struct sockaddr_in srvaddr;
    srvaddr.sin_addr.s_addr = INADDR_ANY;
    srvaddr.sin_port = htons(8765);
    srvaddr.sin_family = AF_INET;

    if(bind(eth_data.srvsock, (struct sockaddr *) &srvaddr, sizeof(srvaddr)) < 0) {
        printf("ETH Device: [Bind Error] %s\n", strerror(errno));
        return -2;
    }
    printf("ETH Device: [Bind Successed]\n");

    // Listen
    printf("ETH Device: Waiting for client...\n");
    if(listen(eth_data.srvsock, 1) < 0) {
        printf("ETH Device: [Listen Error] %s\n", strerror(errno));
        return -3;
    }
    return 0;
}

static int SocketAccept()
{
    struct sockaddr_in cliaddr;
    int cliaddr_size = sizeof(cliaddr);

    eth_data.clisock = accept(eth_data.srvsock, (struct sockaddr *) &cliaddr, (socklen_t *) &cliaddr_size);
    if(eth_data.clisock < 0) {
        printf("ETH Device: [Accept Error] %s\n", strerror(errno));
        StateTrainsition(&eth_dv.state_machine, State_Disable);
        return -1;
    }

    printf("ETH Device: [Accepted] %s:%d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
    return 0;
}


static int SocketRecv()
{
    int recv_len;
    int packet_len;
    recv_len = recv(eth_data.clisock, &packet_len, 1, 0);
    if (recv_len < 0) {
        printf("ETH Device: [Recv Error] %s\n", strerror(errno));
        return -1;
    }
    
    if (recv_len == 0) {
        printf("ETH Device: Client closed socket\n");
        close(eth_data.clisock);
        eth_data.clisock = 0;
        return -1;
    }

    recv_len = recv(eth_data.clisock, eth_data.rx_buff, packet_len, 0);
    printf("ETH Device: [Recv: %d] ", recv_len);
    for (int i = 0; i < recv_len; i++) {
        printf("%d ", eth_data.rx_buff[i]);
    }
    printf("\n");
    return recv_len;
}


static int SocketSend()
{
    int send_len = eth_data.tx_buff[0];
    printf("ETH Device: [Send: %d] ", send_len);
    for (int i = 0; i < send_len; i++) {
        printf("%d ", eth_data.tx_buff[i+1]);
    }
    printf("\n");
    if (send(eth_data.clisock, eth_data.tx_buff, eth_data.tx_buff[0]+1, 0) < 0) {
        printf("ETH Device: [Send Error] %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

static void Protocol(int8_t* msg)
{
    /*
    * |-------------------------- ETH Protocol --------------------------|
    * |------------|------------|------------|------------|--------------|
    * |   Header   |   byte 0   |   byte 1   |   byte 2   |   byte 3 ~   |
    * |------------|------------|------------|------------|--------------|
    * | Packet Len |  Req /Res  |  Device ID |  Data ID   |  Data array  |
    * |------------|------------|------------|------------|--------------|
    */

    // Get/Set
    bool is_set;
    switch (msg[ETH_PACKET_REQS]) {
        case 0: is_set = false; break;
        case 1: is_set = true;  break;
        default: 
            printf("ETH Device: Invalid command %d\n", msg[ETH_PACKET_REQS]); 
            return;
    }

    // Device ID
    int8_t dev_id = msg[ETH_PACKET_DVID];
    DeviceStruct* dev;
    switch (dev_id) {
    case 0: dev = &eth_dv; break;
    case 1: dev = &uart_dv; break;
    default: 
        printf("ETH Device: Invalid devicd ID %d\n", msg[ETH_PACKET_DVID]); 
        return;
    }
    
    // Data ID
    int8_t data_id = msg[ETH_PACKET_DAID];
    int res = 0;

    // Request Processing
    int data_len = 0;

    if (is_set) {
        res = SetActions(dev, data_id, &msg[ETH_PACKET_ARGS]);
    } else {
        data_len = GetActions(dev, data_id, &eth_data.tx_buff[4]);
        if (data_len < 0) {
            res = -1;
        }
    }

    eth_data.tx_buff[0] = 3 + data_len; // Packet Len
    eth_data.tx_buff[1] = res;          // Response
    eth_data.tx_buff[2] = dev_id;       // Device ID
    eth_data.tx_buff[3] = data_id;      // Data ID
}

static int SetActions(DeviceStruct* dev, int data_id, int8_t* data)
{
    switch (data_id) {
    case 0:  // State
        switch (data[0]) {
        case 0: StateTrainsition(&dev->state_machine, State_Off);     break;
        case 1: StateTrainsition(&dev->state_machine, State_Disable); break;
        case 2: StateTrainsition(&dev->state_machine, State_Enable);  break;
        case 3: StateTrainsition(&dev->state_machine, State_Error);   break;
        default: return -1;
        }
        break;
    case 1: // Drive routine
        if (data[0] < 0) {
            ClearDriveRoutines(&dev->drive_routine);
            return 0;
            break;
        }
        if (data[0] < DRIVE_ROUTINE_MAX_ENTITIES) {
            return PushDrviceRoutine(&dev->drive_routine, data[0]);
            break;
        }
        return -1;
        break;
    case 2: // Period
        memcpy(&dev->period, &data[0], 2);
        return 0;
        break;
    default:
        printf("ETH Device: Invalid data ID\n");
        return -1;
        break;
    }
}

static int GetActions(DeviceStruct* dev, int data_id, int8_t* data)
{
    int data_len = 0;
    switch (data_id) {
    case 0: // State
        data_len = 1;
        memcpy(data, (int8_t*)&dev->state_machine.curr_state, 1);
        break;
    case 1: // Drive routine
        data_len = dev->drive_routine.n_id;
        for (int i = 0 ; i < data_len; i++) {
            data[i] = (int8_t)dev->drive_routine.id[i];
        }
        break;
    case 2: // Period
        data_len = 2;
        memcpy(data, &dev->period, 2);
        break;
    default:
        printf("ETH Device: Invalid data ID\n"); 
        return -1;
        break;
    }
    return data_len;
}