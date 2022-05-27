#include "state_machine.h"

// State & Transition Map
static StateEnum TransitionMap(StateEnum curr, StateEnum cmd)
{
    if (curr == cmd) {
        return curr;
    }

    switch (curr) {
    /*  |- From State -|  |---------------------------------- To State ----------------------------------| |- If valid -| |else| */
    case State_Off:     if (0              || cmd==State_Standby || 0                 || 0               ) {return cmd; } break;
    case State_Standby: if (cmd==State_Off || 0                  || cmd==State_Enable || 0               ) {return cmd; } break;
    case State_Enable:  if (cmd==State_Off || cmd==State_Standby || 0                 || cmd==State_Error) {return cmd; } break;
    case State_Error:   if (cmd==State_Off || cmd==State_Standby || 0                 || 0               ) {return cmd; } break;
    default: break;
    }

    return curr;
}


// State Functions
StateEntityStruct CreateStateEntity(StateFuncPtr ent, StateFuncPtr run, StateFuncPtr ext)
{
    StateEntityStruct res;
    res.on_enter = ent;
    res.on_run =   run;
    res.on_exit =  ext;
    return res;
}


// State Machine Interfaces
void InitStateMachine(StateMachineStruct* sm)
{
    for (int i = 0; i < STATE_MACHINE_N_STATES; i++) {
        sm->entity[i].on_enter = NULL;
        sm->entity[i].on_run   = NULL;
        sm->entity[i].on_exit  = NULL;
    }
    sm->slc = StateLifeCycle_Ent;
}

void RunStateMachine(StateMachineStruct* sm)
{
    switch (sm->slc) {
    case StateLifeCycle_Ent:
        if (sm->entity[sm->curr_state].on_enter) {
            sm->entity[sm->curr_state].on_enter();
        }
        if (sm->slc == StateLifeCycle_Ent){
            sm->slc = StateLifeCycle_Run;
        }
        break;

    case StateLifeCycle_Run:
        if (sm->entity[sm->curr_state].on_run) {
            sm->entity[sm->curr_state].on_run();
        }
        break;

    case StateLifeCycle_Ext:
        if (sm->entity[sm->prev_state].on_exit) {
            sm->entity[sm->prev_state].on_exit();
        }
        sm->slc = StateLifeCycle_Ent;
        break;

    default: // Invalid Lifecycle
        break;
    }
}

void StateTrainsition(StateMachineStruct* sm, StateEnum state_cmd)
{
    StateEnum new_state = TransitionMap(sm->curr_state, state_cmd);
    if (sm->curr_state != new_state) {
        sm->prev_state = sm->curr_state;
        sm->curr_state = new_state;
        sm->slc = StateLifeCycle_Ext;
    }
}
