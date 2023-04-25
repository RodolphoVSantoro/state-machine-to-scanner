#include <stdio.h>
#include <stdlib.h>

#include "my_string.h"
#include "scanner_generator.h"
#include "state_machine.h"
#include "types.h"
#include "vecs.h"

int main() {
    const char* program_file_name = "./data/prolog_program.txt";
    FILE* programa = fopen(program_file_name, "r");
    if (programa == NULL) {
        printf("Erro ao abrir o arquivo %s", program_file_name);
        return -1;
    }

    const char* automato_file_name = "./data/prolog_automaton.txt";
    StateVec* automato_prolog = cria_maquina_de_estado_por_arquivo(automato_file_name);
    if (automato_prolog == NULL) {
        printf("Erro alocando memoria para maquina de estados");
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
    int estado_anterior = 1;

    char caracter_atual = fgetc(programa);
    CharVec_push(token_read_vec, caracter_atual);
    char caracter_proximo = EOF;
    if (caracter_atual != EOF) {
        caracter_proximo = fgetc(programa);
    }
    while (caracter_proximo != EOF) {
        StateMachine current_node = StateVec_get(automato_prolog, estado_atual);

        // TODO: rever posicao dessa logica
        Bool is_character_delimiter = CharVec_contains(delimiters, caracter_atual);
        if (!is_character_delimiter) {
            // printf("Lendo caracter: '%c'\n", caracter_atual);
            CharVec_push(token_read_vec, caracter_atual);
        }

        // TODO: rever onde colocar essa logica
        if (current_node.isFinal) {
            CharVec_merge(confirmed_token_vec, token_read_vec);
            CharVec_reset(token_read_vec);
        }

        if (token_complete(automato_prolog, estado_atual, caracter_proximo, is_character_delimiter)) {
            Bool erro_lexico = tem_erro_lexico(estado_atual, current_node.isFinal);
            if (erro_lexico) {
                throw_lexical_error(confirmed_token_vec, token_read_vec, linha_atual, posicao_na_linha_atual);
            }
            // char* buffer = CharVec_to_string(confirmed_token_vec);
            // printf("Token lido: %s\n", buffer);
            // free(buffer);
            // printf("Salvando: estado atual: %d, estado final: %d, caracter atual: '%c', caracter proximo: '%c'\n", estado_atual, current_node.isFinal, caracter_atual, caracter_proximo);
            salva_token(result_token_file, estado_anterior, estado_atual, confirmed_token_vec);
            estado_atual = 1;
            estado_anterior = 1;
            CharVec_reset(confirmed_token_vec);
        } else {
            estado_anterior = estado_atual;
            estado_atual = get_transition(automato_prolog, estado_atual, caracter_atual);
        }

        // cursores de posicao para o indicar onde ocorre erro lexico
        posicao_na_linha_atual++;
        if (caracter_atual == '\n') {
            linha_atual++;
            posicao_na_linha_atual = 1;
        }

        caracter_atual = caracter_proximo;
        caracter_proximo = fgetc(programa);
    }
    StateMachine current_node = StateVec_get(automato_prolog, estado_anterior);
    if (CharVec_len(confirmed_token_vec) > 0) {
        if (current_node.isFinal) {
            salva_token(result_token_file, estado_anterior, estado_atual, confirmed_token_vec);
        } else {
            throw_lexical_error(confirmed_token_vec, token_read_vec, linha_atual, posicao_na_linha_atual);
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
