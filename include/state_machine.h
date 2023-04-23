#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "types.h"

int get_transition(StateVec *states, int current_state, uchar symbol);
void print_token(StateVec *states, int current_state);
void print_non_zero_transitions(StateVec *states, int n_states);
void StateMachines_libera(StateVec *states, int n_states);
StateMachine StateMachine_default();
StateVec *cria_maquina_de_estado_por_arquivo(const char fname[]);

#endif
