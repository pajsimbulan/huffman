~~~~~~~~~~~~~~~~~~~~~~~~~
Program: Huffman Encoder
program accepts input from a file or stdin.  Each byte is represented as a symbol from the extentded ASCII Table.
For each symbol, its occurrences throughout the whole program is counted and is put into a Priority Queue, where 
tail -> head   is to  lowest frequency -> highest frequency.  It is then put into a tree of which its position on the tree
is the encoded value.   The formation of the tree is inputted into the outfile.  
Encoded value for each symbol is inputted into the outfile or stdout.  A header is inputted in the outfile, as well, 
which one of its data containts a secret value that shows that it is encoded by this program.
~~~~~~~~~~~~~~~~~~~~~~~~~

~~~~~~~~~~~~~~~~~~~~~~~~~
Program: Huffman Decoder
program accepts input from a file or stdin.  It reads in the first block of bytes which includes a header which one of its data contains a secret value.
This secret value should match the Decoder's secret value or else it won't be able to decode the file.  It reads the next set of values which is the tree's formation.  It reconstructs the tree and then
gathers the next set of values, which are the symbols.  The symbol's code is needed to traverse the tree and decode the message
~~~~~~~~~~~~~~~~~~~~~~~~~

~~~~~~~~~~~~~~~~~~~~~~~~~
Files:
1. encode.c
2. decode.c
3. entropy.c
4. defines.h
5. header.h
6. node.h
7. node.c
8. pq.h
9. pq.c
10. code.h
11. code.c
12. io.h
13. io.c
14. stack.h
15. stack.c
16. huffman.h
17. huffman.c
18. Makefile
19. README.md (this)
20. DESIGN.pdf
~~~~~~~~~~~~~~~~~~~~~~~~

~~~~~~~~~~~~~~~~~~~~~~~~
How to RUN and BUILD:
-typing "make" or "make all" builds the program
-typing "make encode," "make decode", "make entropy," only builds the encode, decode, or entropy program, respectively.
- ./encode -v -h -i <infile>  -o <outfile> are the commands for running the encode.  -v prints the statistics when encoding a file
- -h prints on how to run the program but exits the program right away.  When infile or outfile are not supplied, stdin is substituted for input and stdout for ouput.
-./decode -v -h -i <infile> -o <outfile> are commands for decode.  Commands are the same for encode.
- typing "make clean" removes all the binary files an .o files that were built
- "make format" formats the .c's and .h's files
~~~~~~~~~~~~~~~~~~~~~~~~
