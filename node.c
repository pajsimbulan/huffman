//~~~~~~~~~~~~~~~~~~~~~~~~
//Name:Paul Simbulan
//Student ID: 1812663
//~~~~~~~~~~~~~~~~~~~~~~~~

#include "node.h"

#include <stdio.h>
#include <stdlib.h>

//malloc for Node
//Node->left and Node->right instantialize as to be pointed to itself
//to prevent seg faults
Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *n = (Node *) malloc(sizeof(Node));
    if (n) {
        n->symbol = symbol;
        n->frequency = frequency;
        n->left = n;
        n->right = n;
    }
    return n;
}

//free malloc'd node
void node_delete(Node **n) {
    if ((*n)) {
        free((*n));
        (*n) = NULL;
    }
}

//create a parent node with children left and right joined
//parent node symbol '$' and frequency sum of both left and right children
Node *node_join(Node *left, Node *right) {
    if ((left == NULL) || (right == NULL)) {
        return NULL;
    }

    Node *n = node_create('$', (left->frequency + right->frequency));
    n->left = left;
    n->right = right;

    return n;
}

void node_print(Node *n) {
    printf("got hre\n");
    printf("curr: s[%c] f[%lu]\t left: s[%c] f[%lu]\t right: s[%c] f[%lu]\n ", n->symbol,
        n->frequency, n->left->symbol, n->left->frequency, n->right->symbol, n->right->frequency);
}
