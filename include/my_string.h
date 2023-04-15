#ifndef MY_STRING_H
#define MY_STRING_H

#include "types.h"

Bool strings_iguais(char *string1, char *string2);
char **split(char *string, char separador);
uint len_strings(char **strings);
void libera_strings(char **strings);

#endif
