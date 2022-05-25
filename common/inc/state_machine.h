#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include "unistd.h"

// State & Transition Map
#define STATE_MACHINE_N_STATES 4

typedef enum {
    State_Off = 0,
    State_Standby = 1,
    State_Enable = 2,
    State_Error = 3,
} StateEnum;


// State Functions
typedef void (*StateFuncPtr) (void);

typedef struct StateEntityStruct
{
    StateFuncPtr on_enter;
    StateFuncPtr on_run;
    StateFuncPtr on_exit;
} StateEntityStruct;


StateEntityStruct CreateStateEntity(StateFuncPtr ent, StateFuncPtr run, StateFuncPtr ext);


// State Machine
typedef enum {
    StateLifeCycle_Ent,
    StateLifeCycle_Run,
    StateLifeCycle_Ext,
} StateLifeCycleEnum;

typedef struct StateMachineStruct
{
    StateEnum curr_state;
    StateEnum prev_state;
    StateLifeCycleEnum slc;

    StateEntityStruct entity[STATE_MACHINE_N_STATES];
} StateMachineStruct;


// State Machine Interfaces
void InitStateMachine(StateMachineStruct* sm);
void RunStateMachine(StateMachineStruct* sm);

void StateTrainsition(StateMachineStruct* sm, StateEnum state_cmd);


#endif //STATE_MACHINE_H_
