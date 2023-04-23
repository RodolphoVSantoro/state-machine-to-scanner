#include <stdio.h>
#include <stdlib.h>

#include "error_log.h"
#include "state_machine.h"
#include "types.h"
#include "vecs.h"

void print_token_from_string(StateVec *states, char *string) {
    int estado_atual = 1;
    int i = 0;
    while (string[i] != '\0' && estado_atual != 0) {
        estado_atual = get_transition(states, estado_atual, string[i]);
        i++;
    }
    print_token(states, estado_atual);
}

int main() {
    StateVec *prolog_scanner_automata = cria_maquina_de_estado_por_arquivo("./data/prologAutomaton.txt");
    if (prolog_scanner_automata == NULL) {
        printerrf("Erro alocando memoria para maquina de estados");
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

    StateVec_free(&prolog_scanner_automata);
    return 0;
}