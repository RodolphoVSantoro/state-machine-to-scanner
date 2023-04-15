#define BUFFER_SIZE (1024 * 1024)
#include <stdio.h>

#include "my_string.h"
#include "state_machine.h"
#include "types.h"

void computa_token(int* estado_anterior, int* estado_atual, char* buffer, FILE* resultado_tokens) {
    if (*estado_atual != 1) {
        salva_token(estado_anterior, estado_atual, buffer, resultado_tokens);
    }
    *estado_atual = 1;
    *estado_anterior = 1;
}

void salva_token(int* estado_anterior, int* estado_atual, char* buffer, FILE* resultado_tokens) {
    fprintf(resultado_tokens, "Estado atual: %d, Estado Anterior: %d, Token: %s", estado_anterior, estado_atual, buffer);
}

int main() {
    const char* program_file_name = "./data/prolog_program.txt";
    FILE* programa = fopen(program_file_name, "r");
    if (programa == NULL) {
        printf("Erro ao abrir o arquivo %s", program_file_name);
        return -1;
    }

    const char* automato_file_name = "./data/prolog_automato.txt";
    StateMachine* automato_prolog = create_state_machine_from_file(automato_file_name);
    if (automato_prolog == NULL) {
        printf("Erro alocando memoria para maquina de estados");
        return -1;
    }

    int linha_atual = 1;
    int posicao_na_linha_atual = 1;
    Bool erro_lexico = False;
    const char* result_tokens_file = "./data/resultado_tokens.txt";
    FILE* resultado_tokens = fopen(result_tokens_file, "w");

    char buffer[BUFFER_SIZE];
    int pos_buffer = 0;

    int estado_atual = 1;
    int estado_anterior = 1;

    do {
        char c = fgetc(programa);
        if (c == EOF) {
            break;
        }

        if (c == ' ' || c == '\n' || c == '\t') {
            buffer[pos_buffer] = 0;
            computa_token(&estado_anterior, &estado_atual, buffer, resultado_tokens);

            posicao_na_linha_atual++;
            if (c == '\n') {
                linha_atual++;
            }

            pos_buffer = 0;
            continue;
        }

        buffer[pos_buffer] = c;
        pos_buffer++;
        estado_anterior = estado_atual;

        estado_atual = get_transition(automato_prolog, estado_atual, c);
    } while (True);

    fclose(resultado_tokens);
    fclose(programa);
    free(automato_prolog);
    if (erro_lexico) {
        buffer[pos_buffer] = 0;
        printf("Erro léxico na linha %d, posicao %d, lendo o token '%s'\n", linha_atual, posicao_na_linha_atual, buffer);
    }
    return 0;
}
