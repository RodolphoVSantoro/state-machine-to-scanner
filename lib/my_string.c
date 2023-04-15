#include "my_string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Bool strings_iguais(char *string1, char *string2) {
    uint len1 = strlen(string1);
    uint len2 = strlen(string2);
    if (len1 != len2) {
        return False;
    }
    for (uint i = 0; i < len1; i++) {
        if (string1[i] != string2[i]) {
            return False;
        }
    }
    return True;
}

char **split(char *string, char separador) {
    uint tamanho_string_original = strlen(string);
    uint quantidade_strings_output = 0;
    for (uint i = 0; i < tamanho_string_original; i++) {
        if (string[i] == separador) {
            quantidade_strings_output++;
        }
    }
    uint *tamanho_strings = (uint *)malloc(sizeof(uint) * (quantidade_strings_output + 1));
    uint tamanho_string = 0;
    uint numero_strings = 0;
    for (uint i = 0; i < tamanho_string_original; i++) {
        if (string[i] == separador) {
            if (tamanho_string > 0) {
                tamanho_strings[numero_strings] = tamanho_string;
            }
            tamanho_string = 0;
        } else {
            tamanho_string++;
        }
    }

    char **strings = (char **)malloc(sizeof(char *) * (quantidade_strings_output + 1));
    uint numero_string = 0;
    uint posicao_char = 0;
    strings[0] = (char *)malloc(sizeof(char) * (tamanho_strings[0] + 1));
    for (uint i = 0; i < tamanho_string_original; i++) {
        if (string[i] == separador) {
            if (posicao_char > 0) {
                strings[numero_string][posicao_char] = 0;
                numero_string++;
                strings[numero_string] = (char *)malloc(sizeof(char) * (tamanho_strings[numero_string] + 1));
            }
            posicao_char = 0;
        } else {
            strings[numero_string][posicao_char] = string[i];
            posicao_char++;
        }
    }
    if (posicao_char > 0) {
        strings[numero_string][posicao_char] = 0;
        numero_string++;
    }
    strings[numero_string] = NULL;
    free(tamanho_strings);
    return strings;
}

uint len_strings(char **strings) {
    uint i = 0;
    while (strings[i] != NULL) {
        i++;
    }
    return i;
}

void libera_strings(char **strings) {
    uint i = 0;
    while (strings[i] != NULL) {
        free(strings[i]);
        i++;
    }

    free(strings);
}