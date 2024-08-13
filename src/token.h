#ifndef TOKEN_H_
#define TOKEN_H_

#include <stddef.h>

typedef struct {
    const char *begin;
    size_t length;
} StringSlice;

typedef enum {
    Token_Type_INVALID = 0,
    Token_Type_PLUS,
    Token_Type_MINUS,
    Token_Type_STAR,
    Token_Type_FORWARD_SLASH,
    Token_Type_NUMBER,
    Token_Type_LEFT_PAREN,
    Token_Type_RIGHT_PAREN,
    Token_Type_EOF
} Token_Type;

typedef struct {
    Token_Type type;
    StringSlice slice;
} Token;

#endif // TOKEN_H_
