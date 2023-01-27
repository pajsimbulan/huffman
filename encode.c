#include "code.h"
#include "header.h"
#include "huffman.h"
#include "io.h"
#include "node.h"
#include "pq.h"
#include "stack.h"

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

//histogram var
uint64_t hist[ALPHABET];

//code table
Code table[ALPHABET];

//tree dumped array, dump array size
uint8_t dump[MAX_TREE_SIZE];
uint32_t dump_size = 0;

//helper function
//dumps tree to sequences of chars
void tree_dump(Node *n) {

    if (n->left != n) {
        tree_dump(n->left);
    }
    if (n->right != n) {
        tree_dump(n->right);
    }
    if ((n->symbol == '$') && ((n != n->left) || (n != n->right))) {
        dump[dump_size++] = 'I';
    } else {
        dump[dump_size++] = 'L';
        dump[dump_size++] = n->symbol;
    }
}

//helper fuction
//for bug decoding purposes
void tree_print(Node *n) {
    if (n == NULL) {
        return;
    }
    if (n->left != n) {
        tree_print(n->left);
    }
    if (n->right != n) {
        tree_print(n->right);
    }
    printf("(%c,%lu)\n", n->symbol, n->frequency);
}

int main(int argc, char **argv) {
    char opt;
    int fd_infile = 0;
    int fd_outfile = 1;
    bool print_stats = false;
    while ((opt = getopt(argc, argv, "vhi:o:")) != -1) {
        switch (opt) {
        case 'h':
            printf("SYNOPSIS\n");
            printf("  A Huffman encoder.\n");
            printf("  Compresses a file using the Huffman coding algorithm.\n\n");
            printf("USAGE\n");
            printf("  ./encode [-h] [-i infile] [-o outfile]\n\n");
            printf("OPTIONS\n");
            printf("  -h\t\tProgram usage and help.\n");
            printf("  -v\t\tPrint compression statistics.\n");
            printf("  -i infile\t\tInput file to compress.\n");
            printf("  -o outfile\t\tOutput of compressed data\n");
            exit(EXIT_SUCCESS);
            break;

        case 'i': fd_infile = open(optarg, O_RDONLY); break;

        case 'o': fd_outfile = open(optarg, O_WRONLY); break;

        case 'v': print_stats = true; break;
        }
    }

    //encoding
    //buffer that holds 4096 kb
    uint8_t *buffer = (uint8_t *) malloc(sizeof(uint8_t) * BLOCK);

    //first time reading from infile
    //if bytes_read empty, exit since infile is empty
    int bytes_read = 0;
    bytes_read += read_bytes(fd_infile, buffer, BLOCK);
    if (bytes_read <= 0) {
        fprintf(stderr, "Empty file.  Exiting program\n");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    //constructing histogram frequency
    for (int i = 0; i < bytes_read; i++) {
        hist[buffer[i]]++;
    }

    // i/o file permission variables
    struct stat statbuf;
    fstat(fd_infile, &statbuf);
    fchmod(fd_outfile, statbuf.st_mode);

    //constructing tree from histogram frequency
    Node *tree = build_tree(hist);

    //constructing code table
    build_codes(tree, table);

    //dumping tree
    tree_dump(tree);

    //constructing header
    Header encoded_header;
    encoded_header.magic = MAGIC;
    encoded_header.permissions = statbuf.st_mode;
    encoded_header.tree_size = dump_size;
    encoded_header.file_size = statbuf.st_size;
    write_bytes(fd_outfile, ((uint8_t *) &encoded_header), sizeof(Header));
    //writing dumped tree contents to outfile
    write_bytes(fd_outfile, dump, dump_size);

    //writing encoded codes to outfile
    for (int i = 0; i < bytes_read; i++) {
        write_code(fd_outfile, &table[buffer[i]]);
    }
    flush_codes(fd_outfile);

    //stats printing
    if (print_stats) {
        int infile_size = statbuf.st_size;
        fprintf(stderr, "Compressed file size: %d bytes\n", infile_size);
        fstat(fd_outfile, &statbuf);
        int outfile_size = statbuf.st_size;
        fprintf(stderr, "Decompressed file size : %d bytes\n", outfile_size);
        float size = (100 * (1 - ((float) outfile_size / (float) infile_size)));
        fprintf(stderr, "Space Saving: %.2f%c\n", size, 37);
    }

    //freeing malloc'ed vars
    delete_tree(&tree);
    free(buffer);
    close(fd_infile);
    close(fd_outfile);

    return (EXIT_SUCCESS);
}
