//~~~~~~~~~~~~~~~~~~~~~~~~
//Name:Paul Simbulan
//Student ID: 1812663
//~~~~~~~~~~~~~~~~~~~~~~~~

#include "pq.h"

#include "node.h"

#include <stdio.h>
#include <stdlib.h>

struct PriorityQueue {
    uint32_t capacity; // max size of Priority Queue
    uint32_t curr_size; // current size of Prio. Q.
    Node **pq_node;
};

//malloc for Priority Queue
PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *pq = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    pq->curr_size = 0;
    pq->capacity = capacity;
    if (pq) {
        pq->pq_node = (Node **) malloc(sizeof(Node *) * capacity);
        if (!pq->pq_node) {
            free(pq);
            return NULL;
        }
        for (uint32_t i = 0; i < capacity; i++) {
            pq->pq_node[i] = (Node *) malloc(sizeof(Node *));
        }
    }
    return pq;
}

//deletes PQ along with nodes inside of PQ
void pq_delete(PriorityQueue **q) {
    if ((*q) && (*q)->pq_node) {
        for (uint32_t i = 0; i < (*q)->curr_size; i++) {
            node_delete(&(*q)->pq_node[i]);
        }
        free((*q)->pq_node);
        free((*q));
        (*q) = NULL;
    }
}

//returns true/false if PQ is empty
bool pq_empty(PriorityQueue *q) {
    return (q->curr_size <= 0);
}

//returns true/false if PQ is full
bool pq_full(PriorityQueue *q) {
    return (q->curr_size == q->capacity);
}

//returns current size of PQ
uint32_t pq_size(PriorityQueue *q) {
    return q->curr_size;
}

//uses insertion sort
//from low priority -> high priority (tail -> head)
bool enqueue(PriorityQueue *q, Node *n) {
    if (q == NULL) {
        return false;
    }

    if (q->curr_size == q->capacity) {
        return false;
    }
    //finding right position of node
    int pos = 0;
    for (pos = 0; pos < (int) q->curr_size; ++pos) {
        if (n->frequency <= q->pq_node[pos]->frequency) {
            break;
        }
    }

    //moving everthing to the right of pos by 1
    for (int i = (int) (q->curr_size - 1); i >= pos; i--) {
        q->pq_node[i + 1] = q->pq_node[i];
    }
    q->pq_node[pos] = n;
    q->curr_size += 1;
    return true;
}

//dequeues tail
bool dequeue(PriorityQueue *q, Node **n) {
    if (q == NULL) {
        return false;
    }

    if (q->curr_size <= 0) {
        return false;
    }

    (*n) = q->pq_node[0];
    for (uint32_t i = 0; i < (q->curr_size - 1); i++) {
        q->pq_node[i] = q->pq_node[i + 1];
    }
    q->curr_size -= 1;
    return true;
}

//peeks tail
Node *peek_tail(PriorityQueue *q) {
    if (q == NULL) {
        return false;
    }

    if (q->curr_size <= 0) {
        return false;
    }

    return q->pq_node[0];
}

void pq_print(PriorityQueue *q) {
    printf("q: ");

    for (uint32_t i = 0; i < q->curr_size; i++) {
        printf("(%c,%lu) ", q->pq_node[i]->symbol, q->pq_node[i]->frequency);
    }
    printf("\n");
}
