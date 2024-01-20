#ifndef POLYALPHABETIC_CODEC_H
#define POLYALPHABETIC_CODEC_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define NUM_OF_CHARS 256

// This struct represents a codec object.
typedef struct {
    char map[NUM_OF_CHARS];
    char reverseMap[NUM_OF_CHARS];
} MyCodec;

// Function prototypes
void * createCodec(char key[62]);
int encode (const char * in, char * out, void * codec, int size);
int decode (const char * in, char * out, void * codec, int size);
void freecodec(void * codec);

#endif // POLYALPHABETIC_CODEC_H
