#include "vecs.h"

#include <stdio.h>
#include <stdlib.h>

#include "error_log.h"

/**
 *
 * String Vec
 *
 */

StringVec *StringVec_create() {
    StringVec *vec = (StringVec *)malloc(sizeof(StringVec));
    vec->data = (char **)malloc(sizeof(char *));
    vec->size = 0;
    vec->capacity = 1;
    return vec;
}

void StringVec_push(StringVec *vec, char *str) {
    int new_capacity = vec->capacity;
    if (vec->size >= vec->capacity) {
        new_capacity *= 2;
        vec->data = (char **)realloc(vec->data, new_capacity * sizeof(char *));
    }
    if (vec->data == NULL) {
        printerrf("Error increasing vec to %d\n", new_capacity);
        exit(1);
    }
    vec->capacity = new_capacity;
    vec->data[vec->size] = str;
    vec->size++;
}

char *StringVec_pop(StringVec *vec) {
    if (vec->size == 0) {
        printerrf("Error popping empty vec\n");
        exit(1);
    }
    vec->size--;
    return vec->data[vec->size];
}

char *StringVec_get(StringVec *vec, int index) {
    if (index >= vec->size || index < 0) {
        printerrf("Error getting index %d from string vec of size %d\n", index, vec->size);
        exit(1);
    }
    return vec->data[index];
}

void StringVec_set(StringVec *vec, int index, char *str) {
    if (index >= vec->size || index < 0) {
        printerrf("Error setting index %d from string vec of size %d\n", index, vec->size);
        exit(1);
    }
    vec->data[index] = str;
}

int StringVec_len(StringVec *vec) {
    return vec->size;
}

void StringVec_free(StringVec **vec) {
    for (int i = 0; i < (*vec)->size; i++) {
        free((*vec)->data[i]);
        (*vec)->data[i] = NULL;
    }
    free((*vec)->data);
    (*vec)->data = NULL;
    free((*vec));
    *vec = NULL;
}

/**
 *
 * Char Vec
 *
 */

CharVec *CharVec_create() {
    CharVec *vec = (CharVec *)malloc(sizeof(CharVec));
    vec->data = (char *)malloc(sizeof(char));
    vec->size = 0;
    vec->capacity = 1;
    return vec;
}

void CharVec_push(CharVec *vec, char c) {
    int new_capacity = vec->capacity;
    if (vec->size >= vec->capacity) {
        new_capacity *= 2;
        vec->data = (char *)realloc(vec->data, new_capacity * sizeof(char));
    }
    if (vec->data == NULL) {
        printerrf("Error increasing vec from %d to %d\n", vec->capacity, vec->capacity * 2);
        exit(1);
    }
    vec->capacity = new_capacity;
    vec->data[vec->size] = c;
    vec->size++;
}

char CharVec_pop(CharVec *vec) {
    if (vec->size == 0) {
        printerrf("Error popping empty vec\n");
        exit(1);
    }
    vec->size--;
    return vec->data[vec->size];
}

char CharVec_get(CharVec *vec, int index) {
    if (index >= vec->size || index < 0) {
        printerrf("Error getting index %d from char vec of size %d\n", index, vec->size);
        exit(1);
    }
    return vec->data[index];
}

void CharVec_resize(CharVec *vec, int capacity) {
    if (capacity == 0) {
        printerrf("Error cant resize vec to 0(use reset)\n");
        exit(1);
    }
    vec->data = (char *)realloc(vec->data, capacity * sizeof(char));
    if (vec->data == NULL) {
        printerrf("Error resizing vec\n");
        exit(1);
    }
    if (capacity > vec->capacity) {
        for (int i = vec->capacity; i < capacity; i++) {
            vec->data[i] = 0;
        }
    }
    vec->capacity = capacity;
}

void CharVec_reset(CharVec *vec) {
    vec->size = 0;
    vec->capacity = 1;
}

void CharVec_set(CharVec *vec, int index, char c) {
    if (index > vec->size || index < 0) {
        printerrf("Error setting index %d from char vec of size %d\n", index, vec->size);
        exit(1);
    }
    if (index == vec->size) {
        CharVec_push(vec, c);
        return;
    }
    vec->data[index] = c;
}

int CharVec_len(CharVec *vec) {
    return vec->size;
}

void CharVec_free(CharVec **vec) {
    free((*vec)->data);
    (*vec)->data = NULL;
    free((*vec));
    *vec = NULL;
}

char *CharVec_to_string(CharVec *vec) {
    char *string = (char *)malloc((vec->size + 1) * sizeof(char));
    for (int i = 0; i < vec->size; i++) {
        string[i] = vec->data[i];
    }
    string[vec->size] = '\0';
    return string;
}

/**
 *
 * State macnine vec
 *
 */

StateVec *StateVec_create() {
    StateVec *vec = (StateVec *)malloc(sizeof(StateVec));
    vec->data = (StateMachine *)malloc(sizeof(StateMachine));
    vec->size = 0;
    vec->capacity = 1;
    return vec;
}

void StateVec_push(StateVec *vec, StateMachine c) {
    int new_capacity = vec->capacity;
    if (vec->size >= vec->capacity) {
        new_capacity *= 2;
        vec->data = (StateMachine *)realloc(vec->data, new_capacity * sizeof(StateMachine));
    }
    if (vec->data == NULL) {
        printerrf("Error increasing vec to %d\n", vec->capacity * 2);
        exit(1);
    }
    vec->capacity = new_capacity;
    vec->data[vec->size] = c;
    vec->size++;
}

StateMachine StateVec_pop(StateVec *vec) {
    if (vec->size == 0) {
        printerrf("Error popping empty vec\n");
        exit(1);
    }
    vec->size--;
    return vec->data[vec->size];
}

StateMachine StateVec_get(StateVec *vec, int index) {
    if (index >= vec->size || index < 0) {
        printerrf("Error getting index %d from StateMachine vec of size %d\n", index, vec->size);
        exit(1);
    }
    return vec->data[index];
}

void StateVec_set(StateVec *vec, int index, StateMachine m) {
    if (index > vec->size || index < 0) {
        printerrf("Error setting index %d from StateMachine vec of size %d\n", index, vec->size);
        exit(1);
    }
    if (index == vec->size) {
        StateVec_push(vec, m);
        return;
    }
    vec->data[index] = m;
}

void StateVec_resize(StateVec *vec, int capacity) {
    vec->data = (StateMachine *)realloc(vec->data, capacity * sizeof(StateMachine));
    if (vec->data == NULL) {
        printerrf("Error resizing vec\n");
        exit(1);
    }
    if (capacity > vec->capacity) {
        for (int i = vec->capacity; i < capacity; i++) {
            vec->data[i] = StateMachine_default();
        }
    }
    vec->capacity = capacity;
}

int StateVec_len(StateVec *vec) {
    return vec->size;
}

void StateVec_free(StateVec **vec) {
    free((*vec)->data);
    (*vec)->data = NULL;
    free(*vec);
    *vec = NULL;
}

StateMachine *StateVec_to_state_machine(StateVec *vec, int *size) {
    *size = vec->size;
    StateMachine *machines = (StateMachine *)malloc(sizeof(StateMachine) * vec->size);
    for (int i = 0; i < vec->size; i++) {
        machines[i] = vec->data[i];
    }
    return machines;
}