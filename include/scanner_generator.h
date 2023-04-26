#ifndef SCANNER_GENERATOR_H
#define SCANNER_GENERATOR_H

#include <stdio.h>

#include "vecs.h"

CharVec* read_delimiter_list(const char* filename);
void salva_token(FILE* resultado_tokens, StateMachine current_state, CharVec* token_vec);
Bool tem_erro_lexico(int estado_atual, Bool estado_final);
Bool token_complete(StateVec* automato_prolog, int estado_atual, char caracter_proximo, Bool is_character_delimiter);
void throw_lexical_error(CharVec* token_confirmed, CharVec* token_read_vec, char proximo_char, int linha_atual, int posicao_na_linha_atual);

#endif