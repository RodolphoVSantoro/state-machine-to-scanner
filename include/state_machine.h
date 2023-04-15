#include "types.h"
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
typedef struct TRANSITION {
    uchar symbol;
    int transition_into;
} Transition;

typedef struct STATE_MACHINE {
    uint transitions_into[N_UCHAR];
    char *token_name;
    Bool isFinal;
} StateMachine;
uint get_transition(StateMachine states[], uint current_state, uchar symbol);
void print_non_zero_transitions(StateMachine states[], uint n_states);
void print_token(StateMachine *states, uint current_state);
void StateMachine_libera(StateMachine *states, uint n_states);
StateMachine *cria_maquina_de_estado_por_arquivo(const char fname[], uint *n_states);
#endif
