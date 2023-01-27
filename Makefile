CC	=	clang
CFLAGS	=	-Wall	-Wextra	-Werror	-Wpedantic

all:	encode	decode	entropy

encode	:	encode.o	node.o	huffman.o	pq.o	stack.o	io.o	code.o
	$(CC)	-o	encode	encode.o	node.o	huffman.o	pq.o	stack.o	io.o	code.o

encode.o	:	encode.c	node.h	huffman.h	pq.h	stack.h	io.h	header.h	code.h
	$(CC)	$(CLAGS)	-c	encode.c

decode	:	decode.o	node.o	huffman.o	pq.o	stack.o	io.o	code.o
	$(CC)	-o	decode	decode.o	node.o	huffman.o	pq.o	stack.o	io.o	code.o

decode.o	:	decode.c	node.h	huffman.h	pq.h	stack.h	io.h	header.h	code.h
	$(CC)	$(CFLAGS)	-c	decode.c

entropy	:	entropy.o

	$(CC)	-o  entropy entropy.o	-lm

entropy.o	:	entropy.c
	$(CC)	$(CLAGS)	-c	entropy.c

code.o	:	code.c	code.h
	$(CC)	$(CFLAGS)	-c	code.c

huffman.o	:	huffman.c	huffman.h	node.h	stack.h	header.h	pq.h
	$(CC)	$(CFLAGS)	-c	huffman.c

io.o	:	io.c	io.h
	$(CC)	$(CFLAGS)	-c	io.c

node.o	:	node.c	node.h
	$(CC)	$(CFLAGS)	-c	node.c

pq.o	:	pq.c	pq.h	node.h
	$(CC)	$(CFLAGS)	-c	pq.c

stack.o	:	stack.c	stack.h	
	$(CC)	$(CFLAGS)	-c	stack.c

clean	:	
	rm	-f	encode	decode	entropy	*.o

format	:	
	clang-format	-i	-style=file	*.c	*.h

