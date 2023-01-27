//~~~~~~~~~~~~~~~~~~~~~~~~
//Name:Paul Simbulan
//Student ID: 1812663
//~~~~~~~~~~~~~~~~~~~~~~~~

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
    while ((opt = getopt(argc, argv, "vhi:o:")) != -1) 
    {
        switch (opt) {
        case 'h':
            printf("SYNOPSIS\n");
            printf("  A Huffman decoder.\n");
            printf("  Decompresses a file using the Huffman coding algorithm.\n\n");
            printf("USAGE\n");
            printf("  ./decode [-h] [-i infile] [-o outfile]\n\n");
            printf("OPTIONS\n");
            printf("  -h\t\tProgram usage and help.\n");
            printf("  -v\t\tPrint decompression statistics.\n");
            printf("  -i infile\t\tInput file to compress.\n");
            printf("  -o outfile\t\tOutput of decompressed data\n");
            exit(EXIT_SUCCESS);
            break;

        case 'i': fd_infile = open(optarg, O_RDONLY); break;

        case 'o': fd_outfile = open(optarg, O_WRONLY);

        case 'v': print_stats = true; break;
        }
    }

    // i/o file permission variables
    struct stat statbuf;
    fstat(fd_infile, &statbuf);
    fchmod(fd_outfile, statbuf.st_mode);

    //constructing header header
    //and first time reading from infile
    //if bytes_read is 0, exit
    Header header;
    header.magic = 0;
    int bytes_read = 0;
    bytes_read += read_bytes(fd_infile, ((uint8_t *) &header), sizeof(Header));
    if (bytes_read <= 0) {
        fprintf(stderr, "Empty file.  Exiting program\n");
        exit(EXIT_FAILURE);
    }
    if (header.magic != MAGIC) {
        fprintf(stderr, "Error: unable to read header\n");
        exit(EXIT_FAILURE);
    }

    //re-constructing tree
    uint8_t dumped_tree[MAX_TREE_SIZE];
    read_bytes(fd_infile, &dumped_tree[0], header.tree_size);
    Node *rebuilded_tree = rebuild_tree(header.tree_size, dumped_tree);

    //reading decoded codes and traversing tree
    Node *n = rebuilded_tree;
    uint8_t *bit = (uint8_t *) malloc(sizeof(uint8_t));
    for (uint32_t i = 0; i < header.file_size;) {

        if ((n->right == n) && (n->left == n)) {
            *bit = n->symbol;
            write_bytes(fd_outfile, bit, 1);
            n = rebuilded_tree;
            i++;
        }
        if (!read_bit(fd_infile, bit)) {
            break;
        }
        if ((*bit) == 1) {
            n = n->right;
        }

        else if ((*bit) == 0) {
            n = n->left;
        }
    }

    //stats printing
    if (print_stats) {
        int infile_size = statbuf.st_size;
        fprintf(stderr, "Compressed file size: %d bytes\n", infile_size);
        fstat(fd_outfile, &statbuf);
        int outfile_size = statbuf.st_size;
        fprintf(stderr, "Decompressed file size : %d bytes\n", outfile_size);
        float size = (100 * (1 - ((float) infile_size / (float) outfile_size)));
        fprintf(stderr, "Space Saving: %.2f%c \n", size, 37);
    }

    //freeing malloc'd variables
    close(fd_infile);
    close(fd_outfile);
    free(bit);

    return EXIT_SUCCESS;
}
