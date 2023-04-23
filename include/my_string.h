#ifndef MY_STRING_H
#define MY_STRING_H

#include "types.h"

Bool strings_iguais(char *string1, char *string2);
StringVec *split(char *string, char separador);
char *new_string_from(char *str);

/*Filas de Char*/

CharQueue *fila_char_cria();
Bool fila_char_vazia(CharQueue *fila);
void fila_char_insert(CharQueue *fila, char c);
char fila_char_remove(CharQueue *fila);
void fila_char_libera(CharQueue *fila);

#endif
