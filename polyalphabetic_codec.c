#include "polyalphabetic_codec.h"

// This function receives a char array with fixed size of 62 bytes, representing a map of the alphabet.
// The function should return a pointer
void * createCodec() {
    MyCodec * codec = (MyCodec *) malloc(sizeof(MyCodec));
    if (codec == NULL) {
        return NULL;
    }

    // Initialize the map and reverseMap
    int i = 0;
    for (i = 0; i < NUM_OF_CHARS; i++) {
        codec->map[i] = i;
        codec->reverseMap[i] = i;
    }

    // Create a shuffled version of the character set a-z, A-Z, 0-9
    char key[62] = "zxcvbnmasdfghjklqwertyuiopZXCVBNMASDFGHJKLQWERTYUIOP1234567890";

    for (int i = 0; i < 62; i++) {
        if (i < 26){
            codec->map[i + 97] = key[i];
            codec->reverseMap[(unsigned char)key[i]] = i + 97;
        } else if (i < 52) {
            codec->map[i + 65 - 26] = key[i];
            codec->reverseMap[(unsigned char)key[i]] = i + 65 - 26;
        } else {
            codec->map[i - 52 + 48] = key[i];
            codec->reverseMap[(unsigned char)key[i]] = i - 52 + 48;
        }
    }

    return codec;
}



// This function receives pointers for: input char array, output char array, codec, and the size of the input array.
// The function should encode the input array and store the result in the output array.
// The function returns the number of bytes written to the output array.
int encode (const char * in, char * out, void * codec, int size) {
    int encoded_bytes = 0;

    // verify that the codec is not NULL
    if (codec == NULL) {
        return -1;
    }

    MyCodec * c = (MyCodec *) codec;
    for (int i = 0; i < size; i++) {
        out[i] = c->map[(unsigned char)in[i]];
        encoded_bytes++;
    }
    return encoded_bytes;
}

// This function receives pointers for: input char array, output char array, codec, and the size of the input array.
// The function should decode the input array and store the result in the output array.
// The function returns the number of bytes written to the output array.
int decode (const char * in, char * out, void * codec, int size) {
    int decoded_bytes = 0;

    // verify that the codec is not NULL
    if (codec == NULL) {
        return -1;
    }

    MyCodec * c = (MyCodec *) codec;
    for (int i = 0; i < size; i++) {
        out[i] = c->reverseMap[(unsigned char)in[i]];
        decoded_bytes++;
    }
    return decoded_bytes;
}

// This function receives a pointer to a codec and frees all the memory allocated for it.
void freecodec(void * codec) {
    free(codec);
}