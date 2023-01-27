//~~~~~~~~~~~~~~~~~~~~~~~~
//Name:Paul Simbulan
//Student ID: 1812663
//~~~~~~~~~~~~~~~~~~~~~~~~

#include "code.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

Code code_init(void) {
    Code c;
    c.top = 0;
    //initialize to 0
    for (int i = 0; i < MAX_CODE_SIZE; i++) {
        c.bits[i] = 0;
    }
    return c;
}

uint32_t code_size(Code *c) {
    return c->top;
}

bool code_empty(Code *c) {
    return (c->top == 0);
}

bool code_full(Code *c) {
    return (c->top >= 256);
}

bool code_push_bit(Code *c, uint8_t bit) {
    if (c == NULL) {
        return false;
    }
    if (c->top >= 256) {
        return false;
    }

    //if bit being pushed is 1
    //perform OR bitwise operation with 1
    //on the right index and right group
    //if bit is 0, just ignore, top just increments by 1
    if (bit) {
        int group = (c->top / 8);
        int index = (c->top % 8);
        uint8_t temp = 1;
        temp = (temp << index);
        c->bits[group] = (c->bits[group] | temp);
    }

    c->top += 1;
    return true;
}

//get bit on top of stack
//stores top bit on *bit
bool code_pop_bit(Code *c, uint8_t *bit) {
    if (c == NULL) {
        return false;
    }
    if (c->top == 0) {
        return false;
    }
    int group = ((c->top - 1) / 8);
    int index = ((c->top - 1) % 8);

    //getting bit
    uint8_t temp = c->bits[group];
    temp = (temp << (7 - index));
    temp = (temp >> 7);
    *bit = temp;

    //setting bit back to 0
    if (temp) {
        temp = 1;
        temp = (temp << index);
        temp = (~temp);
        c->bits[group] = (c->bits[group] & temp);
    }
    c->top -= 1;
    return true;
}

void code_print(Code *c) {
    int group = ((c->top - 1) / 8);
    printf("codes: \n");
    for (int i = 0; i < group; i++) {
        printf("%d.\t%d\n", i, c->bits[i]);
    }
}
