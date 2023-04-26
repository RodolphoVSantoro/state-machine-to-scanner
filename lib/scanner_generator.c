#include "scanner_generator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error_log.h"
#include "my_string.h"
#include "state_machine.h"
#include "vecs.h"

CharVec* read_delimiter_list(const char* filename) {
    FILE* delimiters_file = fopen(filename, "r");
    if (delimiters_file == NULL) {
        printerrf("Erro ao abrir o arquivo %s", filename);
        exit(1);
    }
    CharVec* delimiters = CharVec_create();
    char caracter;
    while ((caracter = fgetc(delimiters_file)) != EOF) {
        CharVec_push(delimiters, caracter);
    }
    fclose(delimiters_file);

    char* buffer = CharVec_to_string(delimiters);
    CharVec_free(&delimiters);
    StringVec* delimiters_string_vec = split(buffer, '\n');
    free(buffer);

    delimiters = CharVec_create();
    int len = StringVec_len(delimiters_string_vec);
    for (int i = 0; i < len; i++) {
        char* delimiter = StringVec_get(delimiters_string_vec, i);
        int delimiter_len = strlen(delimiter);
        switch (delimiter_len) {
            case 0:
                break;
            case 1:
                CharVec_push(delimiters, delimiter[0]);
                break;
            case 2:
                if (delimiter[0] == '\\' && delimiter[1] == 'n') {
                    CharVec_push(delimiters, '\n');
                }
                break;
            default:
                printerrf("Delimitador invalido: %s", delimiter);
                exit(1);
        }
    }
    StringVec_free(&delimiters_string_vec);
    return delimiters;
}

void salva_token(FILE* resultado_tokens, StateMachine current_state, CharVec* token_vec) {
    char* buffer = CharVec_to_string(token_vec);
    fprintf(resultado_tokens, "%s(%s)\n", current_state.token_name, buffer);
    free(buffer);
}

Bool tem_erro_lexico(int estado_atual, Bool estado_final) {
    if (estado_atual == 0 || estado_final != True) {
        return True;
    }
    return False;
}

Bool token_complete(StateVec* automato_prolog, int estado_atual, char caracter_proximo, Bool is_character_delimiter) {
    if (is_character_delimiter) {
        return True;
    }
    Bool is_next_state_dead = get_transition(automato_prolog, estado_atual, caracter_proximo) == 0;
    if (is_next_state_dead) {
        return True;
    }
    return False;
}

void throw_lexical_error(CharVec* token_confirmed, CharVec* token_read_vec, char proximo_char, int linha_atual, int posicao_na_linha_atual) {
    CharVec_merge(token_confirmed, token_read_vec);
    char* buffer = CharVec_to_string(token_confirmed);
    printerrf("Erro léxico na linha %d, posicao %d, lendo o token '%s' found unexpected '%c'\n", linha_atual, posicao_na_linha_atual, buffer, proximo_char);
    exit(1);
}
