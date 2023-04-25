#ifndef VECS_H
#define VECS_H

#include "types.h"

StringVec *StringVec_create();
void StringVec_push(StringVec *vec, char *str);
char *StringVec_pop(StringVec *vec);
char *StringVec_get(StringVec *vec, int index);
void StringVec_set(StringVec *vec, int index, char *str);
int StringVec_len(StringVec *vec);
void StringVec_free(StringVec **vec);
Bool StringVec_contains(StringVec *vec, char *str);

CharVec *CharVec_create();
void CharVec_push(CharVec *vec, char c);
char CharVec_pop(CharVec *vec);
char CharVec_get(CharVec *vec, int index);
void CharVec_set(CharVec *vec, int index, char c);
int CharVec_len(CharVec *vec);
void CharVec_resize(CharVec *vec, int capacity);
void CharVec_reset(CharVec *vec);
void CharVec_free(CharVec **vec);
char *CharVec_to_string(CharVec *vec);
Bool CharVec_contains(CharVec *vec, char c);
CharVec *CharVec_copy_slice(CharVec *vec, int start, int end);
void CharVec_merge(CharVec *destino, CharVec *fonte);

StateVec *StateVec_create();
void StateVec_push(StateVec *vec, StateMachine state);
StateMachine StateVec_pop(StateVec *vec);
StateMachine StateVec_get(StateVec *vec, int index);
void StateVec_set(StateVec *vec, int index, StateMachine state);
int StateVec_len(StateVec *vec);
void StateVec_resize(StateVec *vec, int capacity);
void StateVec_free(StateVec **vec);
StateMachine *StateVec_to_state_machine(StateVec *vec, int *size);

#endif