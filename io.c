//~~~~~~~~~~~~~~~~~~~~~~~~
//Name:Paul Simbulan
//Student ID: 1812663
//~~~~~~~~~~~~~~~~~~~~~~~~

#include "io.h"

#include <stdio.h>
#include <stdlib.h>

//read in nbytes of bytes from infile and store it into *buf
//returns bytes succesfully read
int read_bytes(int infile, uint8_t *buf, int nbytes) {
    int bytes_read = 0;
    bytes_read = read(infile, buf, nbytes);

    //reads in more bytes, if first read didn't fully read all of it
    //breaks out of loop if there really no more bytes to read
    while (bytes_read < nbytes) {
        int curr_bytes_read = read(infile, &buf[bytes_read], (nbytes - bytes_read));
        if (curr_bytes_read <= 0) {
            break;
        }
        bytes_read += curr_bytes_read;
    }

    return bytes_read;
}

//writes in nbytes of bytes to outfile from *buf
//returns bytes succesfully written
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    int bytes_written = 0;
    bytes_written = write(outfile, buf, nbytes);

    //writes in more bytes, if first write didn't fully write all of it
    //breaks out of loop if there really no more bytes to write
    while (bytes_written < nbytes) {
        int curr_bytes_written = write(outfile, &buf[bytes_written], (nbytes - bytes_written));
        if (curr_bytes_written <= 0) {
            break;
        }
        bytes_written += curr_bytes_written;
    }

    return bytes_written;
}

//helper function
//gets bit i from uint8_t array n

uint8_t get_bit(uint8_t *n, uint32_t i) {

    int group = (i / 8);
    int index = (i % 8);
    uint8_t temp = n[group];
    temp = (temp << (7 - index));
    temp = (temp >> 7);
    return temp;
}

//helper function
//sets bit i from uint8_t array n
void set_bit(uint8_t *n, uint32_t i) {

    int group = (i / 8);
    int index = (i % 8);
    uint8_t temp = 1;
    temp = (temp << index);
    n[group] = (n[group] | temp);
}

//helper function
//clears bit i from uint8_t array n
void clr_bit(uint8_t *n, uint32_t i) {
    int group = (i / 8);
    int index = i % 8;
    uint8_t temp = 1;
    temp = (temp << index);
    temp = (~temp);
    n[group] = (n[group] & temp);
}

//reads in a BLOCK bytes (4096) from infile
//uses static bit buffer that resets once whole block is read
//stores current bit from bit buffer to *bit
//returns wether or not there theres more bits to read
bool read_bit(int infile, uint8_t *bit) {

    static uint64_t bit_buffer = 0;
    static uint64_t bits_read = 0;
    static uint8_t buf[BLOCK];

    //if bit buffer excceeds bits_read
    //read in more BLOCK size of bytes
    if (bit_buffer == bits_read) {
        bit_buffer = 0;
        bits_read = 0;

        bits_read += (read_bytes(infile, &buf[bit_buffer], BLOCK) * 8);
    }

    //if bits read is greater than buffer
    //there is more bits left to read
    if (bits_read > bit_buffer) {

        *bit = get_bit(buf, bit_buffer);
        bit_buffer++;
    }

    //returns wether or not there theres more bits to read
    return (bit_buffer < bits_read);
}

//for each bit in Code *c, get bit and store into buf[BLOCK]
//writes codes into outfile onces buf[BLOCK] is full (contains 4096 bytes)
static uint8_t buf[BLOCK];
static uint64_t bit_index = 0;
void write_code(int outfile, Code *c) {
    for (uint32_t i = 0; i < code_size(c); i++) {
        if (get_bit(c->bits, i)) {
            set_bit(&buf[0], bit_index);
        } else {
            clr_bit(&buf[0], bit_index);
        }
        bit_index += 1;
    }

    if (bit_index >= (BLOCK * 8)) {
        write_bytes(outfile, buf, BLOCK);
        bit_index = 0;
    }
}

//flushes remaning bits in buf[BLOCK] int ooutfile
void flush_codes(int outfile) {
    if (bit_index > 0) {

        int nbytes = ((bit_index / 8) + ((bit_index % 8) ? 1 : 0));
        write_bytes(outfile, buf, nbytes);
    }
}
