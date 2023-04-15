#include "state_machine.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_string.h"
#include "types.h"

static void printerrf(char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}

void print_non_zero_transitions(StateMachine states[], uint numero_estados) {
    uint i = 0;
    for (i = 0; i < numero_estados; i++) {
        uint zero_transition_states = N_UCHAR;
        for (uint j = 0; j < N_UCHAR; j++) {
            if (states[i].transitions_into[j] != 0) {
                zero_transition_states--;
                printf("f(%d, '%c') = %d\n", i, j, states[i].transitions_into[j]);
            }
        }
        if (zero_transition_states != 0) {
            printf("As demais transicoes do estado %d sao para 0\n", i);
        }
    }
}

void print_token(StateMachine *states, uint current_state) {
    if (states[current_state].token_name == NULL) {
        printf("Invalid Token\n");
    } else {
        printf("Token: \"%s\"\n", states[current_state].token_name);
    }
}

uint get_transition(StateMachine states[], uint current_state, uchar symbol) {
    return states[current_state].transitions_into[symbol];
}

void StateMachine_libera(StateMachine *states, uint n_states) {
    for (uint i = 0; i < n_states; i++) {
        if (states[i].isFinal) {
            free(states[i].token_name);
        }
    }
    free(states);
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

// TODO: fila de char para ter linhas de tamanho arbitrario
char *le_linha(FILE *file) {
    int i = 0;
    char c = 0;
    char *linha = malloc(200 * sizeof(char));
    do {
        c = fgetc(file);
        if (c == EOF || c == '\n') {
            break;
        }
        if (i >= 200) {
            printerrf("Linha %d muito grande", i);
            exit(1);
        }
        linha[i] = c;
        i++;
    } while (True);
    linha[i] = 0;
    return linha;
}

uint numero_linhas(FILE *f) {
    uint linhas = 0;
    do {
        char c = fgetc(f);
        if (c == '\n') {
            linhas++;
        }
    } while (!feof(f));
    fseek(f, 0, SEEK_SET);
    return linhas;
}

uint numero_estados(FILE *arquivo) {
    uint estados = 0;
    Bool start, e, n, d;
    start = e = n = d = False;
    do {
        char c = fgetc(arquivo);
        if (c == '\n') {
            start = True;
            e = n = d = False;
        } else if (start && c == 'e') {
            e = True;
            n = d = False;
        } else if (start && e && c == 'n') {
            n = True;
            d = False;
        } else if (start && e && n && c == 'd') {
            start = e = n = d = False;
            estados++;
        } else {
            start = e = n = d = False;
        }
    } while (!feof(arquivo));
    fseek(arquivo, 0, SEEK_SET);
    return estados;
}

void erro_lendo_maquina_estado(int linha, int numero_palavras, char **palavras) {
    printerrf("Erro lendo automato, linha %d\n", linha);
    printerrf("Esperava 2 identificadores, ou fim de linha, ou fim de arquivo\nLeu %d palavras\nPalavras:\n", numero_palavras);
    int j = 0;
    for (j = 0; j < numero_palavras - 1; j++) {
        printerrf("%s, ", palavras[j]);
    }
    printerrf("%s\n", palavras[j]);
    exit(1);
}

void erro_lendo_maquina_estado_final_flag(int linha, int numero_palavras, char **palavras) {
    printerrf("Erro lendo automato, linha %d\n", linha);
    if (numero_palavras >= 2) {
        printerrf("Esperava identificador do tipo f ou t, leu %s\n", palavras[1]);
    } else {
        printerrf("Esperava pelo menos 2 identificadores na linha %d\n", linha);
    }
    exit(1);
}

void erro_lendo_maquina_estado_token_faltando(uint linha, char **palavras) {
    printerrf("Erro lendo automato, linha %d\n", linha);
    printerrf("Esperava nome de token apos identificador %s\n", palavras[1]);
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
Transition **cria_transicoes_de_palavras(char **palavras) {
    Transition **transitions;

    int transition_into = atoi(palavras[1]);

    if (strings_iguais(palavras[0], "num")) {
        int total_transitions = '9' - '0' + 1;
        transitions = (Transition **)malloc(sizeof(Transition *) * (total_transitions + 1));
        adiciona_transicoes_simbolos_sequenciais(transitions, transition_into, total_transitions, '9');
        return transitions;
    }

    if (strings_iguais(palavras[0], "min")) {
        int total_transitions = 'z' - 'a' + 1;
        transitions = (Transition **)malloc(sizeof(Transition *) * (total_transitions + 1));
        adiciona_transicoes_simbolos_sequenciais(transitions, transition_into, total_transitions, 'z');
        return transitions;
    }

    if (strings_iguais(palavras[0], "mai")) {
        int total_transitions = ('Z' - 'A' + 1);
        transitions = (Transition **)malloc(sizeof(Transition *) * (total_transitions + 1));
        adiciona_transicoes_simbolos_sequenciais(transitions, transition_into, total_transitions, 'Z');
        return transitions;
    }

    // caso nenhum dos anteriores, é um simbolo de 1 caractere
    transitions = (Transition **)malloc(sizeof(Transition *) * 2);
    transitions[0] = (Transition *)malloc(sizeof(Transition));
    *(transitions[0]) = cria_transicao(palavras[0][0], transition_into);
    transitions[1] = NULL;

    return transitions;
}

void copia_transicoes(Transition **transitions, Transition *transicoes, uint *indice_transicoes) {
    uint j = 0;
    while (transitions[j] != NULL) {
        transicoes[*indice_transicoes] = *(transitions[j]);
        j++;
        (*indice_transicoes)++;
    }
}

void adiciona_estado(StateMachine *state_machine, uint *state_index, Transition *transicoes, char **palavras, uint numero_palavras, uint *indice_transicoes, uint linha) {
    Bool is_final;
    char *token_name = NULL;
    if (palavras[1][0] == 'f' && palavras[1][1] == 0) {
        is_final = False;
    } else if (palavras[1][0] == 't' && palavras[1][1] == 0) {
        is_final = True;
        if (numero_palavras == 3) {
            token_name = palavras[2];
        } else {
            erro_lendo_maquina_estado_token_faltando(linha, palavras);
        }
    } else {
        erro_lendo_maquina_estado_final_flag(linha, numero_palavras, palavras);
    }
    state_machine[*state_index] = cria_estado(transicoes, *indice_transicoes, 0, is_final, token_name);
    (*state_index)++;
}

StateMachine *cria_maquina_de_estado_por_arquivo(const char fname[], uint *n_estados) {
    FILE *arquivo_automato = fopen(fname, "r");
    if (arquivo_automato == NULL) {
        printerrf("Erro ao ler o arquivo %s", fname);
        exit(1);
    }

    uint linhas = numero_linhas(arquivo_automato);
    *n_estados = numero_estados(arquivo_automato);

    StateMachine *state_machine = (StateMachine *)malloc(sizeof(StateMachine) * (*n_estados + 1));

    if (state_machine == NULL) {
        return NULL;
    }
    // estado morto
    state_machine[0] = cria_estado(NULL, 0, 0, False, NULL);
    uint state_index = 1;

    Transition transicoes[256];
    uint indice_transicoes = 0;

    for (uint n_linha = 0; n_linha < linhas; n_linha++) {
        char *linha = le_linha(arquivo_automato);
        char **palavras = split(linha, ' ');
        uint numero_palavras = len_strings(palavras);

        Bool linha_invalida = numero_palavras != 2 && numero_palavras != 3 && (numero_palavras != 1 || palavras[0][0] != '\n');
        if (linha_invalida) {
            erro_lendo_maquina_estado(n_linha, numero_palavras, palavras);
        }

        if (strings_iguais(palavras[0], "end")) {
            adiciona_estado(state_machine, &state_index, transicoes, palavras, numero_palavras, &indice_transicoes, n_linha);
            indice_transicoes = 0;
        } else {
            Transition **transitions = cria_transicoes_de_palavras(palavras);
            copia_transicoes(transitions, transicoes, &indice_transicoes);
            libera_transition_2d(transitions);
        }

        free(linha);
        libera_strings(palavras);
    }

    fclose(arquivo_automato);
    return state_machine;
}