#include "lexer.h"

#include <stdbool.h>
#include <stdio.h>

static char peek(Lexer *lexer) {
    return *lexer->current_character_pointer;
}

static bool at_eof(Lexer *lexer) {
    return (peek(lexer) == '\0');
}

static char consume(Lexer *lexer) {
    if(!at_eof(lexer)) {
        return *(lexer->current_character_pointer++);
    }
    return *lexer->current_character_pointer;
}

static bool is_digit(char c) {
    return (c >= '0' && c <= '9');
}

static void consume_number(Lexer *lexer) {
    while(!at_eof(lexer) && is_digit(peek(lexer))) consume(lexer);
    if(peek(lexer) != '.') return;
    consume(lexer);
    while(!at_eof(lexer) && is_digit(peek(lexer))) consume(lexer);
}

static bool is_whitespace(char c) {
    return (c == '\r' || c == '\n' || c == '\t' || c == ' ');
}

Token get_next_token(Lexer *lexer) {
    while(is_whitespace(peek(lexer))) consume(lexer);
    const char *token_start = lexer->current_character_pointer;
    switch(peek(lexer)) {
        case '\0': {
            return (Token){ .type = Token_Type_EOF, .slice = { .begin = token_start, .length = 1 } };
        } break;
        case '+': {
            consume(lexer);
            return (Token){ .type = Token_Type_PLUS, .slice = { .begin = token_start, .length = 1 } };
        } break;
        case '-': {
            consume(lexer);
            return (Token){ .type = Token_Type_MINUS, .slice = { .begin = token_start, .length = 1 } };
        } break;
        case '*': {
            consume(lexer);
            return (Token){ .type = Token_Type_STAR, .slice = { .begin = token_start, .length = 1 } };
        } break;
        case '/': {
            consume(lexer);
            return (Token){ .type = Token_Type_FORWARD_SLASH, .slice = { .begin = token_start, .length = 1 } };
        } break;
        case '(': {
            consume(lexer);
            return (Token){ .type = Token_Type_LEFT_PAREN, .slice = { .begin = token_start, .length = 1 } };
        } break;
        case ')': {
            consume(lexer);
            return (Token){ .type = Token_Type_RIGHT_PAREN, .slice = { .begin = token_start, .length = 1 } };
        } break;
        default: {
            if(is_digit(peek(lexer))) { // TODO(richard): Implement proper error handling, and complete number parsing.
                consume_number(lexer);
                return (Token){ .type = Token_Type_NUMBER, .slice = { .begin = token_start, .length = lexer->current_character_pointer - token_start } };
            } else {
                fprintf(stderr, "[LEXER ERROR]: Unidentified character '%c'.\n", peek(lexer));
                consume(lexer);
            }
        } break;
    }
    return (Token){ .type = Token_Type_INVALID, .slice = { .begin = token_start, .length = 1 } };
}
