#include "data_structs.h"
#include "token.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static void stack_grow_maybe_(Stack *stack) {
    if(stack->count + 1 > stack->capacity) {
        stack->capacity = (stack->capacity) ? stack->capacity*2 : 8;
        stack->memory = realloc(stack->memory, stack->capacity * sizeof(*stack->memory));
        if(!stack->memory) {
            fprintf(stderr, "[MEMEORY ERROR]: Failed to allocate '%zd' bytes of stack memory.\n", stack->capacity * sizeof(*stack->memory));
        }
    }
}

void stack_push(Stack *stack, Token token) {
    stack_grow_maybe_(stack);
    stack->memory[stack->count++] = token;
}

Token stack_pop(Stack *stack) {
    if(stack->count == 0) {
        fprintf(stderr, "[STACK ERROR]: Attempt to pop off an empty stack.\n");
        return (Token){0};
    }
    return stack->memory[--stack->count];
}

Token stack_top(Stack *stack) {
    if(stack->count == 0) {
        fprintf(stderr, "[STACK ERROR]: Attempting to access top of empty stack.\n");
        return (Token){0};
    }
    size_t top = (stack->count == 0) ? 0 : stack->count - 1;
    return stack->memory[top];
}

bool stack_is_empty(Stack *stack) {
    return stack->count == 0;
}

void stack_free(Stack *stack) {
    if(stack->memory) {
        free(stack->memory);
    }
    stack->memory = NULL;
}

QueueNode *create_queue_node(Queue *queue) {
    void *ret = arena_allocate(&queue->arena, sizeof(*queue->front));
    return (QueueNode *)ret;
}

void queue_push(Queue *queue, Token token) {
    if(queue->count == 0) {
        queue->front = create_queue_node(queue);
        queue->back = queue->front;
        queue->front->value = token;
        queue->front->next = NULL;
        queue->front->prev = NULL;
    } else {
        queue->back->next = create_queue_node(queue);
        queue->back->next->prev = queue->back;
        queue->back->next->next = NULL;
        queue->back->next->value = token;
        queue->back = queue->back->next;
    }
    queue->count++;
}

Token queue_pop(Queue *queue) {
    if(!queue->count) {
        fprintf(stderr, "[QUEUE ERROR]: Attemping to pop of an empty queue.\n");
        return (Token){0};
    }
    QueueNode *head = queue->front;
    queue->front = queue->front->next;
    if(queue->front) {
        queue->front->prev = NULL;
    }
    queue->count--;
    return head->value;
}

bool queue_is_empty(Queue *queue) {
    return queue->count == 0;
}

void queue_free(Queue *queue) {
    arena_free(&queue->arena);
}
