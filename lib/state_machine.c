#include "state_machine.h"

#include <stdio.h>
#include <stdlib.h>

#include "error_log.h"
#include "my_string.h"
#include "types.h"
#include "vecs.h"

void print_non_zero_transitions(StateVec *states, int numero_estados) {
    int i = 0;
    for (i = 0; i < numero_estados; i++) {
        int zero_transition_states = N_UCHAR;
        for (int j = 0; j < N_UCHAR; j++) {
            StateMachine state = StateVec_get(states, i);
            if (state.transitions_into[j] != 0) {
                zero_transition_states--;
                printf("f(%d, '%c') = %d\n", i, j, state.transitions_into[j]);
            }
        }
        if (zero_transition_states != 0) {
            printf("As demais transicoes do estado %d sao para 0\n", i);
        }
    }
}

void print_token(StateVec *states, int current_state) {
    StateMachine state = StateVec_get(states, current_state);
    if (state.token_name == NULL) {
        printf("Invalid Token\n");
    } else {
        printf("Token: \"%s\"\n", state.token_name);
    }
}

int get_transition(StateVec *states, int current_state, uchar symbol) {
    StateMachine state = StateVec_get(states, current_state);
    return state.transitions_into[symbol];
}

StateMachine cria_estado(Transition transitions[], int transitions_amount, int default_transition, Bool isFinal, char *token_name) {
    StateMachine state;

    state.isFinal = isFinal;
    state.token_name = token_name;
    for (int i = 0; i < N_UCHAR; i++) {
        state.transitions_into[i] = default_transition;
    }

    for (int i = 0; i < transitions_amount; i++) {
        uchar symbol = transitions[i].symbol;
        state.transitions_into[symbol] = transitions[i].transition_into;
    }
    return state;
}

Transition cria_transicao(uchar symbol, int into) {
    Transition transition;
    transition.symbol = symbol;
    transition.transition_into = into;
    return transition;
}

CharVec *le_automato(FILE *file) {
    CharVec *vec = CharVec_create();
    char c = fgetc(file);
    while (c != EOF) {
        CharVec_push(vec, c);
        c = fgetc(file);
    }
    return vec;
}

StateMachine StateMachine_default() {
    StateMachine state;
    state.token_name = NULL;
    state.isFinal = False;
    for (int i = 0; i < N_UCHAR; i++) {
        state.transitions_into[i] = 0;
    }
    return state;
}

void erro_lendo_maquina_estado(int linha, int numero_palavras, StringVec *palavras) {
    printerrf("Erro lendo automato, linha %d\n", linha);
    if (numero_palavras == 0) {
        printerrf("Esperava 2 identificadores, ou fim de linha, ou fim de arquivo\nLeu nenhuma palavras\n");
        exit(1);
    }
    printerrf("Esperava 2 identificadores, ou fim de linha, ou fim de arquivo\nLeu %d palavras\nPalavras:\n", numero_palavras);
    int j = 0;
    char *palavra = NULL;
    for (j = 0; j < numero_palavras - 1; j++) {
        palavra = StringVec_get(palavras, j);
        printerrf("%s, ", palavra);
    }
    palavra = StringVec_get(palavras, j);
    printerrf("%s\n", palavra);
    exit(1);
}

void erro_lendo_maquina_estado_final_flag(int linha, int numero_palavras, StringVec *palavras) {
    printerrf("Erro lendo automato, linha %d\n", linha);
    if (numero_palavras >= 2) {
        printerrf("Esperava identificador do tipo f ou t, leu %s\n", palavras[1]);
    } else {
        printerrf("Esperava pelo menos 2 identificadores na linha %d\n", linha);
    }
    exit(1);
}

void erro_lendo_maquina_estado_token_faltando(int linha, StringVec *palavras) {
    printerrf("Erro lendo automato, linha %d\n", linha);
    printerrf("Esperava nome de token apos identificador %s\n", StringVec_get(palavras, 0));
    exit(1);
}

void libera_transition_2d(Transition **transitions) {
    int i = 0;
    while (transitions[i] != NULL) {
        free(transitions[i]);
        i++;
    }
    free(transitions);
}

// recebe array de total_transitions + 1 elementos, o ultimo elemento é NULL
void adiciona_transicoes_simbolos_sequenciais(Transition **transitions, int transition_into, int total_de_transitions, uchar ultimo_simbolo) {
    for (int i = 0; i < total_de_transitions; i++) {
        int posicao = total_de_transitions - i - 1;
        transitions[posicao] = (Transition *)malloc(sizeof(Transition));
        *(transitions[posicao]) = cria_transicao(ultimo_simbolo - (uchar)i, transition_into);
    }
    transitions[total_de_transitions] = NULL;
}

// sempre chamado com 2 palavras
Transition **cria_transicoes_de_palavras(StringVec *palavras) {
    Transition **transitions;

    char *transition_into_str = StringVec_get(palavras, 1);
    int transition_into = atoi(transition_into_str);

    char *expression = StringVec_get(palavras, 0);

    if (strings_iguais(expression, "num")) {
        int total_transitions = '9' - '0' + 1;
        transitions = (Transition **)malloc(sizeof(Transition *) * (total_transitions + 1));
        adiciona_transicoes_simbolos_sequenciais(transitions, transition_into, total_transitions, '9');
        return transitions;
    }

    if (strings_iguais(expression, "min")) {
        int total_transitions = 'z' - 'a' + 1;
        transitions = (Transition **)malloc(sizeof(Transition *) * (total_transitions + 1));
        adiciona_transicoes_simbolos_sequenciais(transitions, transition_into, total_transitions, 'z');
        return transitions;
    }

    if (strings_iguais(expression, "mai")) {
        int total_transitions = ('Z' - 'A' + 1);
        transitions = (Transition **)malloc(sizeof(Transition *) * (total_transitions + 1));
        adiciona_transicoes_simbolos_sequenciais(transitions, transition_into, total_transitions, 'Z');
        return transitions;
    }

    // caso nenhum dos anteriores, é um simbolo de 1 caractere
    transitions = (Transition **)malloc(sizeof(Transition *) * 2);
    transitions[0] = (Transition *)malloc(sizeof(Transition));
    *(transitions[0]) = cria_transicao(expression[0], transition_into);
    transitions[1] = NULL;

    return transitions;
}

void copia_transicoes(Transition **transitions, Transition *transicoes, int *indice_transicoes) {
    int j = 0;
    while (transitions[j] != NULL) {
        transicoes[*indice_transicoes] = *(transitions[j]);
        j++;
        (*indice_transicoes)++;
    }
}

void adiciona_estado(StateVec *state_machine_vec, Transition *transicoes, StringVec *palavras, int *indice_transicoes, int linha) {
    Bool is_final = False;
    char *token_name = NULL;
    int numero_palavras = StringVec_len(palavras);
    char *is_final_str = StringVec_get(palavras, 1);

    if (is_final_str[0] == 'f' && is_final_str[1] == 0) {
        is_final = False;
    } else if (is_final_str[0] == 't' && is_final_str[1] == 0) {
        is_final = True;
        if (numero_palavras == 3) {
            token_name = new_string_from(StringVec_get(palavras, 2));
        } else {
            erro_lendo_maquina_estado_token_faltando(linha, palavras);
        }
    } else {
        erro_lendo_maquina_estado_final_flag(linha, numero_palavras, palavras);
    }
    StateMachine novo_estado = cria_estado(transicoes, *indice_transicoes, 0, is_final, token_name);
    *indice_transicoes = *indice_transicoes + 1;
    StateVec_push(state_machine_vec, novo_estado);
}

StateVec *cria_maquina_de_estado_por_arquivo(const char fname[]) {
    FILE *arquivo_automato = fopen(fname, "r");
    if (arquivo_automato == NULL) {
        printerrf("Erro ao ler o arquivo %s", fname);
        exit(1);
    }
    CharVec *automato_char_vec = le_automato(arquivo_automato);
    fclose(arquivo_automato);
    char *automato_str = CharVec_to_string(automato_char_vec);
    free(automato_char_vec);

    StateVec *state_machine_vec = StateVec_create();
    StateVec_push(state_machine_vec, StateMachine_default());

    Transition transicoes[256];
    int indice_transicoes = 0;

    StringVec *linhas = split(automato_str, '\n');
    int n_linhas = StringVec_len(linhas);
    for (int n_linha = 0; n_linha < n_linhas; n_linha++) {
        char *linha = StringVec_get(linhas, n_linha);

        StringVec *palavras = split(linha, ' ');

        int numero_palavras = StringVec_len(palavras);
        Bool linha_invalida = numero_palavras < 1 || numero_palavras > 3;
        if (linha_invalida) {
            erro_lendo_maquina_estado(n_linha, numero_palavras, palavras);
        }

        if (strings_iguais(StringVec_get(palavras, 0), "end")) {
            adiciona_estado(state_machine_vec, transicoes, palavras, &indice_transicoes, n_linha);
            indice_transicoes = 0;
        } else {
            Transition **transitions = cria_transicoes_de_palavras(palavras);
            copia_transicoes(transitions, transicoes, &indice_transicoes);
            libera_transition_2d(transitions);
            transitions = NULL;
        }

        StringVec_free(&palavras);
    }
    StringVec_free(&linhas);

    return state_machine_vec;
}