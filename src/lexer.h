#ifndef LEXER_H_
#define LEXER_H_

#include "token.h"

typedef struct {
    const char *current_character_pointer;
} Lexer;

Token get_next_token(Lexer *lexer);

#endif // LEXER_H_
