//~~~~~~~~~~~~~~~~~~~~~~~~
//Name:Paul Simbulan
//Student ID: 1812663
//~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include "code.h"
#include "defines.h"
#include "node.h"

#include <stdint.h>

//builds huffman tree (encoding part)
Node *build_tree(uint64_t hist[static ALPHABET]);

void build_codes(Node *root, Code table[static ALPHABET]);

Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]);

void delete_tree(Node **root);

#endif
