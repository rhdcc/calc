#include <stdio.h>
#include <stdbool.h>
#include <string.h> // memcpy, memset
#include <stdlib.h> // atof

#include "token.h"
#include "data_structs.h"
#include "lexer.h"
#include "arena.h"

const char *TOKEN_STRINGS[] = {
    "INVALID", "PLUS", "MINUS", "STAR", "FORWARD SLASH", "NUMBER", "LEFT_PAREN", "RIGHT_PAREN", "EOF"
};

int precedence(Token_Type type) {
    // NOTE(richard): A precedence value closer to 0 implies a higher priority.
    switch(type) {
        case Token_Type_STAR: return 1;
        case Token_Type_FORWARD_SLASH: return 1;
        case Token_Type_PLUS: return 2;
        case Token_Type_MINUS: return 2;
        default: {
            fprintf(stderr, "[PARSER ERROR]: Token type '%s' is not an operator and has no precedence.\n", TOKEN_STRINGS[type]);
            return -1;
        }
    }
}

bool shunting_yard(Queue *output_queue, Lexer *lexer) {
    bool success = true;
    Stack operator_stack = {0};
    Token t = {0};
    do {
        t = get_next_token(lexer);

        switch(t.type) {
            case Token_Type_EOF: break;
            case Token_Type_INVALID: {
                success = false;
            } break;
            case Token_Type_NUMBER: queue_push(output_queue, t); break;
            case Token_Type_LEFT_PAREN: stack_push(&operator_stack, t); break;
            case Token_Type_RIGHT_PAREN: {
                bool found_left_paren = false;
                while(!stack_is_empty(&operator_stack)) {
                    Token top = stack_top(&operator_stack);
                    if(top.type == Token_Type_LEFT_PAREN) {
                        found_left_paren = true;
                        stack_pop(&operator_stack);
                        break;
                    }
                    queue_push(output_queue, stack_pop(&operator_stack));
                }
                if(!found_left_paren) {
                    fprintf(stderr, "[PARSER ERROR]: Mismatched right parenthesis.\n");
                    success = false;
                }
            } break;
            default: {
                 if(!stack_is_empty(&operator_stack)) {
                    Token top = stack_top(&operator_stack);
                    while(!stack_is_empty(&operator_stack) &&
                          top.type != Token_Type_LEFT_PAREN &&
                          precedence(top.type) < precedence(t.type)) {
                        queue_push(output_queue, stack_pop(&operator_stack));
                        top = stack_top(&operator_stack);
                    }
                }
                stack_push(&operator_stack, t);
            } break;
        }
    } while(t.type != Token_Type_EOF);

    while(!stack_is_empty(&operator_stack)) {
        queue_push(output_queue, stack_pop(&operator_stack));
    }

    stack_free(&operator_stack);
    return success;
}

#define MAX_NUMBER_LENGTH 24
double parse_RPN(Queue *input_queue) {
    printf("Reverse Polish Notation: ");

    Stack operands = {0};
    Arena string_arena = {0};
    bool success = true;
    double ret = 0;
    while(!queue_is_empty(input_queue)) {
        Token t = queue_pop(input_queue);
        char *token_slice = arena_allocate(&string_arena, t.slice.length + 1);
        memcpy(token_slice, t.slice.begin, t.slice.length);
        token_slice[t.slice.length] = '\0';
        printf("%s ", token_slice);

        switch(t.type) {
            case Token_Type_NUMBER: {
                stack_push(&operands, t);
            } break;
            case Token_Type_PLUS:
            case Token_Type_MINUS:
            case Token_Type_STAR:
            case Token_Type_FORWARD_SLASH: {
                if(operands.count < 2) {
                    fprintf(stderr, "\n[PARSER ERROR]: Insufficient operands to perform operation (Currently %zd operand(s) on stack, but '%s' requires 2).", operands.count, token_slice);
                    success = false;
                    goto parse_end;
                }

                Token right = stack_pop(&operands);
                Token left = stack_pop(&operands);
                char *right_slice = arena_allocate(&string_arena, right.slice.length + 1);
                memcpy(right_slice, right.slice.begin, right.slice.length);
                right_slice[right.slice.length] = '\0';
                char *left_slice = arena_allocate(&string_arena, left.slice.length + 1);
                memcpy(left_slice, left.slice.begin, left.slice.length);
                left_slice[left.slice.length] = '\0';

                double right_value = atof(right_slice);
                double left_value = atof(left_slice);

                double result = 0;
                if(t.type == Token_Type_PLUS) result = left_value + right_value;
                if(t.type == Token_Type_MINUS) result = left_value - right_value;
                if(t.type == Token_Type_STAR) result = left_value * right_value;
                if(t.type == Token_Type_FORWARD_SLASH) result = left_value / right_value;

                char *result_str = arena_allocate(&string_arena, MAX_NUMBER_LENGTH);
                snprintf(result_str, MAX_NUMBER_LENGTH, "%f", result);

                Token result_token = { .type = Token_Type_NUMBER, .slice = { .begin = result_str, .length = strlen(result_str) } };
                stack_push(&operands, result_token);
            } break;
            default: {
                fprintf(stderr, "\n[PARSER ERROR]: Unexpected token: '%s' with type %s.", token_slice, TOKEN_STRINGS[t.type]);
                success = false;
                goto parse_end;
            }
        }
    }

    if(operands.count != 1) {
        fprintf(stderr, "\n[PARSER ERROR]: Operand stack count must be 1, instead it is %zd.", operands.count);
        success = false;
        goto parse_end;
    } else {
        Token last = stack_pop(&operands);
        char *last_slice = arena_allocate(&string_arena, last.slice.length + 1);
        memcpy(last_slice, last.slice.begin, last.slice.length);
        last_slice[last.slice.length] = '\0';
        ret = atof(last_slice);
    }

parse_end:
    if(!success) {
        fprintf(stderr, "\n *** Aborting parsing of Reverse Polish Notation... ***\n");
    }
    stack_free(&operands);
    arena_free(&string_arena);
    return ret;
}

void usage(void) {
    printf("Usage:\n Type in a valid mathematical expression (e.g '1 + 2 * 3') to see the result.\n Type 'exit' to exit program.\n Type 'help' for these instructions.\n");
}

int main(void) {
    usage();
    char line[1024];
    do {
        printf("> ");
        memset(line, 0, sizeof(line));
        if(!fgets(line, sizeof(line), stdin)) {
           fprintf(stderr, "Failed to read line from standard input.\n");
           break;
        }
        if(strcmp(line, "exit\n") == 0) break;
        if(strcmp(line, "help\n") == 0) {
            usage();
            continue;
        }

        Lexer lexer = { line };
        Queue output_queue = {0};
        if(shunting_yard(&output_queue, &lexer)) {
            double ret = parse_RPN(&output_queue);
            printf("\nResult: %f\n", ret);
        }
        queue_free(&output_queue);
    } while(true);

    return 0;
}
