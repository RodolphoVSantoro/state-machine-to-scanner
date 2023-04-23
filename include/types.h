#ifndef TYPES_H
#define TYPES_H
#define N_UCHAR 256

enum BOOL { False,
            True };
typedef enum BOOL Bool;

typedef unsigned char uchar;

struct CHAR_NODE {
    char c;
    struct CHAR_NODE* next;
};
typedef struct CHAR_NODE CharNode;

struct CHAR_QUEUE {
    CharNode* first;
    CharNode* last;
};

typedef struct CHAR_QUEUE CharQueue;

typedef struct TRANSITION {
    uchar symbol;
    int transition_into;
} Transition;

typedef struct STATE_MACHINE {
    int transitions_into[N_UCHAR];
    char* token_name;
    Bool isFinal;
} StateMachine;

typedef struct STRING_VEC {
    char** data;
    int size;
    int capacity;
} StringVec;

typedef struct CHAR_VEC {
    char* data;
    int size;
    int capacity;
} CharVec;

typedef struct STATE_VEC {
    StateMachine* data;
    int size;
    int capacity;
} StateVec;

#endif