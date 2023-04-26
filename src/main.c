#include <stdio.h>
#include <stdlib.h>

#include "error_log.h"
#include "my_string.h"
#include "scanner_generator.h"
#include "state_machine.h"
#include "types.h"
#include "vecs.h"

int main() {
    const char* program_file_name = "./data/prolog_program2.txt";
    FILE* programa = fopen(program_file_name, "r");
    if (programa == NULL) {
        printerrf("Erro ao abrir o arquivo %s", program_file_name);
        return -1;
    }

    const char* automato_file_name = "./data/prolog_automaton.txt";
    StateVec* automato_prolog = cria_maquina_de_estado_por_arquivo(automato_file_name);
    if (automato_prolog == NULL) {
        printerrf("Erro alocando memoria para maquina de estados");
        return -1;
    }

    const char* delimiters_file_name = "./data/prolog_delimiters.txt";
    CharVec* delimiters = read_delimiter_list(delimiters_file_name);

    int linha_atual = 1;
    int posicao_na_linha_atual = 1;

    const char* result_tokens_file_name = "./data/resultado_tokens.txt";
    FILE* result_token_file = fopen(result_tokens_file_name, "w");

    CharVec* token_read_vec = CharVec_create();
    CharVec* confirmed_token_vec = CharVec_create();

    int estado_atual = 1;

    char caracter_proximo = fgetc(programa);

    while (caracter_proximo != EOF) {
        int proximo_estado = get_transition(automato_prolog, estado_atual, caracter_proximo);
        StateMachine current_state = StateVec_get(automato_prolog, estado_atual);
        Bool is_character_delimiter = CharVec_contains(delimiters, caracter_proximo);

        if (!is_character_delimiter && proximo_estado != 0) {
            estado_atual = proximo_estado;
            CharVec_push(token_read_vec, caracter_proximo);
        }

        StateMachine next_state = StateVec_get(automato_prolog, proximo_estado);
        if (next_state.isFinal) {
            char* buffer = CharVec_to_string(token_read_vec);
            free(buffer);
            CharVec_merge(confirmed_token_vec, token_read_vec);
            CharVec_reset(token_read_vec);
        }
        if (proximo_estado == 0) {
            Bool is_token_complete = token_complete(automato_prolog, estado_atual, caracter_proximo, is_character_delimiter);
            Bool empty_delimiter = is_character_delimiter && CharVec_len(confirmed_token_vec) == 0;
            if (!empty_delimiter && is_token_complete) {
                Bool erro_lexico = tem_erro_lexico(estado_atual, current_state.isFinal);
                if (erro_lexico) {
                    if (CharVec_len(confirmed_token_vec) == 0) {
                        throw_lexical_error(confirmed_token_vec, token_read_vec, caracter_proximo, linha_atual, posicao_na_linha_atual);
                    }
                }
                // consume token
                salva_token(result_token_file, current_state, confirmed_token_vec);
                estado_atual = 1;
                CharVec_reset(confirmed_token_vec);
            }
        }

        // cursores de posicao para o indicar onde ocorre erro lexico
        posicao_na_linha_atual++;
        if (caracter_proximo == '\n') {
            linha_atual++;
            posicao_na_linha_atual = 1;
        }

        caracter_proximo = fgetc(programa);
    }
    StateMachine current_node = StateVec_get(automato_prolog, estado_atual);
    if (CharVec_len(confirmed_token_vec) > 0) {
        if (current_node.isFinal) {
            salva_token(result_token_file, current_node, confirmed_token_vec);
        } else {
            throw_lexical_error(confirmed_token_vec, token_read_vec, linha_atual, caracter_proximo, posicao_na_linha_atual);
        }
    }

    fclose(programa);
    fclose(result_token_file);

    free(automato_prolog);
    CharVec_free(&delimiters);
    CharVec_free(&token_read_vec);
    CharVec_free(&confirmed_token_vec);

    printf("Tokens lidos com sucesso!\nSalvo em %s\n", result_tokens_file_name);
    return 0;
}
