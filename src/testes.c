#include <stdio.h>
#include <stdlib.h>

#include "my_string.h"
#include "state_machine.h"
#include "types.h"

void print_token_from_string(StateMachine *states, char *string) {
    uint estado_atual = 1;
    uint i = 0;
    while (string[i] != '\0' && estado_atual != 0) {
        estado_atual = get_transition(states, estado_atual, string[i]);
        i++;
    }
    print_token(states, estado_atual);
}

int main() {
    uint n_states;
    StateMachine *prolog_scanner_automata = cria_maquina_de_estado_por_arquivo("./data/prologAutomaton.txt", &n_states);
    if (prolog_scanner_automata == NULL) {
        printf("Erro alocando memoria para maquina de estados");
        return -1;
    }
    print_token_from_string(prolog_scanner_automata, "?-");
    printf("\n");
    print_token_from_string(prolog_scanner_automata, "?--");
    printf("\n");
    print_token_from_string(prolog_scanner_automata, "abba2adasSADAS");
    printf("\n");
    print_token_from_string(prolog_scanner_automata, "Aasf21312321as2");
    printf("\n");
    print_token_from_string(prolog_scanner_automata, "123213123");
    printf("\n");
    print_token_from_string(prolog_scanner_automata, "1231asd2313");
    printf("\n");

    StateMachine_libera(prolog_scanner_automata, n_states);
    return 0;
}