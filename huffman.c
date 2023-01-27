//~~~~~~~~~~~~~~~~~~~~~~~~
//Name:Paul Simbulan
//Student ID: 1812663
//~~~~~~~~~~~~~~~~~~~~~~~~

#include "huffman.h"

#include "header.h"
#include "node.h"
#include "pq.h"
#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

//builds tree from pre-initialized histogram table
Node *build_tree(uint64_t hist[static ALPHABET]) {
    PriorityQueue *pq = pq_create(ALPHABET);

    //for each symbol with frequency > 0 on histogram
    //table , create a node for it and enqueue
    for (int i = 0; i < ALPHABET; i++) {
        if (hist[i] > 0) {
            enqueue(pq, node_create(i, hist[i]));
        }
    }

    //while enqeueue size > 1 and top node is not a parent node
    //dequeue for left node and dequeue for right node again
    //and join two nodes with a parent node of symbol '$' and
    //frequency equal to sum of both left and right node
    while (pq_size(pq) >= 1) {
        if ((pq_size(pq) == 1) && (peek_tail(pq)->symbol == '$')) {
            break;
        }
        Node *left;
        dequeue(pq, &left);
        Node *right;
        dequeue(pq, &right);
        Node *parent = node_join(left, right);
        enqueue(pq, parent);
    }

    //get tree
    Node *tree;
    dequeue(pq, &tree);

    //free queue
    pq_delete(&pq);

    return tree;
}

//helper function for build codes
//traverses tree with post_order
//if it goes to left, push bit 0 to a Code c
//right, push bit 1
//if it hits a leaf node, get leaf symbol, store Code c
//then pop a bit
void post_order_traversal(Node *n, Code *c, Code table[static ALPHABET]) {
    uint8_t temp;
    if (n->left != n) {
        code_push_bit(c, 0);
        post_order_traversal(n->left, c, table);
    }
    if (n->right != n) {
        code_push_bit(c, 1);
        post_order_traversal(n->right, c, table);
    }

    table[n->symbol] = code_init();
    for (uint32_t i = 0; i <= (code_size(c) / 8); i++) {
        table[n->symbol].bits[i] = c->bits[i];
        table[n->symbol].top = c->top;
    }

    code_pop_bit(c, &temp);
}

//build code table, where for each symbol has a particular code
//based on post order traversing tree
void build_codes(Node *root, Code table[static ALPHABET]) {
    Code c = code_init();
    post_order_traversal(root, &c, table);
}

//rebuilds tree with tree[array]
//for each element, if L, next element in array is a symbol, push that symbol onto stack
//if 'I', pop stack for right node, pop again for left node, then join with a parent then push parent onto stack
Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
    Stack *s = stack_create(ALPHABET);
    Node *left;
    Node *right;
    Node *parent;
    for (int i = 0; i < nbytes; i++) {
        if (tree[i] == 'L') {
            stack_push(s, node_create(tree[i + 1], 0));
        } else if (tree[i] == 'I') {
            stack_pop(s, &right);
            stack_pop(s, &left);
            parent = node_join(left, right);
            stack_push(s, parent);
        }
    }

    return parent;
    stack_delete(&s);
}

//deletes tree using post_order_traversal
void delete_tree(Node **root) {
    if ((*root) == NULL) {
        return;
    }
    if (((*root)->left) != (*root)) {
        delete_tree((&(*root)->left));
    }
    if (((*root)->right) != (*root)) {
        delete_tree((&(*root)->right));
    }
    node_delete(root);
}
