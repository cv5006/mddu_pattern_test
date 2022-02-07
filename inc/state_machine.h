#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_


// State & Transition Map
#define STATE_MACHINE_N_STATES 4

typedef enum {
    State_Off = 0,
    State_Disable = 1,
    State_Enable = 2,
    State_Error = 3,
} StateEnum;

static StateEnum TransitionMap(StateEnum curr, StateEnum cmd)
{
    if (curr == cmd) {
        return curr;
    }

    switch (curr) {
    /*  |- From State -|  |---------------------------------- To State ----------------------------------| |- If valid -| |else| */
    case State_Off:     if (0              || cmd==State_Disable || 0                 || 0               ) {return cmd; } break;
    case State_Disable: if (cmd==State_Off || 0                  || cmd==State_Enable || 0               ) {return cmd; } break;
    case State_Enable:  if (cmd==State_Off || cmd==State_Disable || 0                 || cmd==State_Error) {return cmd; } break;
    case State_Error:   if (cmd==State_Off || cmd==State_Disable || 0                 || 0               ) {return cmd; } break;
    default: break;
    }

    return curr;
}


// State Functions
typedef void (*StateFuncPtr) (void);

typedef struct StateEntityStruct
{
    StateFuncPtr on_enter;
    StateFuncPtr on_run;
    StateFuncPtr on_exit;
} StateEntityStruct;

static void DummyStateFnc() {};

StateEntityStruct CreateStateEntity(StateFuncPtr ent, StateFuncPtr run, StateFuncPtr ext)
{
    StateEntityStruct res;
    res.on_enter = (ent != NULL) ? ent : DummyStateFnc;
    res.on_run =   (run != NULL) ? run : DummyStateFnc;
    res.on_exit =  (ext != NULL) ? ext : DummyStateFnc;
    return res;
}


// State Machine
typedef struct StateMachineStruct
{
    StateEnum curr_state;
    StateEnum prev_state;
    StateFuncPtr active_func;

    StateEntityStruct entity[STATE_MACHINE_N_STATES];
} StateMachineStruct;


// State Machine Interfaces
void InitStateMachine(StateMachineStruct* sm)
{
    for (int i = 0; i < STATE_MACHINE_N_STATES; i++) {
        sm->entity[i].on_enter = NULL;
        sm->entity[i].on_run   = NULL;
        sm->entity[i].on_exit  = NULL;
    }
    sm->active_func = NULL;
}

// TODO: NULL func ptr exception
void RunStateMachine(StateMachineStruct* sm)
{
    sm->active_func();
    

    if (sm->active_func == sm->entity[sm->curr_state].on_run) {
        return;
    }

    if (sm->active_func == sm->entity[sm->curr_state].on_enter) {
        sm->active_func =sm->entity[sm->curr_state].on_run;
        return;
    }

    if (sm->active_func == sm->entity[sm->prev_state].on_exit) {
        sm->active_func = sm->entity[sm->curr_state].on_enter;
        return;
    }
}

void StateTrainsition(StateMachineStruct* sm, StateEnum state_cmd)
{
    StateEnum new_state = TransitionMap(sm->curr_state, state_cmd);
    if (sm->curr_state != new_state) {
        sm->prev_state = sm->curr_state;
        sm->curr_state = new_state;
        sm->active_func = sm->entity[sm->prev_state].on_exit;
    }
}


#endif //STATE_MACHINE_H_