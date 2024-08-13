#ifndef DATA_STRUCTS_H_
#define DATA_STRUCTS_H_

#include <stdbool.h>
#include <stddef.h>
#include "token.h"
#include "arena.h"

typedef struct {
    Token *memory;
    size_t stride;
    size_t count;
    size_t capacity;
} Stack;

void stack_push(Stack *stack, Token token);
Token stack_pop(Stack *stack);
Token stack_top(Stack *stack);
bool stack_is_empty(Stack *stack);
void stack_free(Stack *stack);

typedef struct QueueNode QueueNode;

struct QueueNode {
    QueueNode *next;
    QueueNode *prev;
    Token value;
};

typedef struct {
    QueueNode *front;
    QueueNode *back;
    Arena arena;
    size_t count;
} Queue;

QueueNode *create_queue_node(Queue *queue);
void queue_push(Queue *queue, Token token);
Token queue_pop(Queue *queue);
bool queue_is_empty(Queue *queue);
void queue_free(Queue *queue);

#endif // DATA_STRUCTS_H_
