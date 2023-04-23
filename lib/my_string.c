#include "my_string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vecs.h"

Bool strings_iguais(char *string1, char *string2) {
    int len1 = strlen(string1);
    int len2 = strlen(string2);
    if (len1 != len2) {
        return False;
    }
    for (int i = 0; i < len1; i++) {
        if (string1[i] != string2[i]) {
            return False;
        }
    }
    return True;
}

StringVec *split(char *string, char separador) {
    StringVec *palavras = StringVec_create();
    CharVec *palavra = CharVec_create();

    int i = 0;
    while (string[i] != '\0') {
        if (string[i] == separador && CharVec_len(palavra) > 0) {
            char *delimitado = CharVec_to_string(palavra);
            StringVec_push(palavras, delimitado);
            CharVec_free(&palavra);
            palavra = CharVec_create();
        } else {
            CharVec_push(palavra, string[i]);
        }
        i++;
    }
    if (CharVec_len(palavra) > 0) {
        char *delimitado = CharVec_to_string(palavra);
        StringVec_push(palavras, delimitado);
    }
    CharVec_free(&palavra);
    return palavras;
}

char *new_string_from(char *str) {
    int string_length = strlen(str);
    char *newStr = (char *)malloc(sizeof(char) * (string_length + 1));
    int i;
    for (i = 0; i < string_length; i++) {
        newStr[i] = str[i];
    }
    newStr[i] = '\0';
    return newStr;
}

/*Filas de Char*/

CharQueue *fila_char_cria() {
    CharQueue *fila = (CharQueue *)malloc(sizeof(CharQueue));
    fila->first = NULL;
    fila->last = NULL;
    return fila;
}

Bool fila_char_vazia(CharQueue *fila) {
    return fila->last == NULL;
}

void fila_char_insert(CharQueue *fila, char c) {
    CharNode *novo = (CharNode *)malloc(sizeof(CharNode));
    novo->c = c;
    novo->next = NULL;
    if (fila->last == NULL) {
        fila->first = novo;
    } else {
        fila->last->next = novo;
    }
    fila->last = novo;
}

char fila_char_remove(CharQueue *fila) {
    CharNode *old = fila->first;
    if (old == NULL) {
        printf("Tentou dar remove em uma fila vazia\n");
        exit(1);
    }
    fila->first = old->next;
    if (fila->first == NULL) {
        fila->last = NULL;
    }
    char c = old->c;
    free(old);
    return c;
}

void fila_char_libera(CharQueue *fila) {
    while (fila->first != NULL) {
        CharNode *tmp = fila->first;
        fila->first = tmp->next;
        free(tmp);
    }
}