//~~~~~~~~~~~~~~~~~~~~~~~~
//Name:Paul Simbulan
//Student ID: 1812663
//~~~~~~~~~~~~~~~~~~~~~~~~

#include "stack.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct Stack {
    uint32_t top; // next element on stack
    uint32_t capacity; // max size of stack
    Node **items; // stack of nodes
};

//malloc for stack
Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    s->capacity = capacity;
    if (s) {
        s->items = (Node **) malloc(sizeof(Node *) * capacity);
        if (!s->items) {
            free(s);
            return NULL;
        }
        for (uint32_t i = 0; i < capacity; i++) {
            s->items[i] = (Node *) malloc(sizeof(Node *));
        }
    }
    return s;
}

//free's malloc'd stack and nodes inside of it
void stack_delete(Stack **s) {
    if ((*s) && (*s)->items) {
        for (uint32_t i = 0; i < (*s)->top; i++) {
            node_delete(&(*s)->items[i]);
        }
        free((*s)->items);
        free((*s));
        (*s) = NULL;
    }
}

//returns true/false if stack is empty
bool stack_empty(Stack *s) {
    return (s->top == 0);
}

//returns true/false if stack is full
bool stack_full(Stack *s) {
    return (s->top == s->capacity);
}

//returns current size of stack
uint32_t stack_size(Stack *s) {
    return (s->top);
}

//pushes Node n onto stack
bool stack_push(Stack *s, Node *n) {
    if (s == NULL) {
        return false;
    }
    if (s->top == s->capacity) {
        return false;
    }

    s->items[s->top] = n;
    s->top += 1;
    return true;
}

//pops top of stack and store node value to *n
//returns true if successful, else false
bool stack_pop(Stack *s, Node **n) {
    if (s == NULL) {
        return false;
    }
    if (s->top == 0) {
        return false;
    }

    s->top -= 1;
    (*n) = s->items[s->top];
    s->items[s->top] = NULL;
    return true;
}

void stack_print(Stack *s) {
    printf("s: ");
    for (uint32_t i = 0; i < s->top; i++) {
        printf("(%c,%lu) ", s->items[i]->symbol, s->items[i]->frequency);
    }
    printf("\ntop index = %d\n", s->top);
}
