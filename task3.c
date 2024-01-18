# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>

#define NUM_OF_CHARS 256

// This struct represents a codec object.
typedef struct {
    char map[NUM_OF_CHARS];
    char reverseMap[NUM_OF_CHARS];
} MyCodec;

// This function receives a char array with fixed size of 62 bytes, representing a map of the alphabet.
// The function should return a pointer
void * createCodec(char key[62]) {
    MyCodec * codec = (MyCodec *) malloc(sizeof(MyCodec));
    if (codec == NULL) {
        return NULL;
    }
    // Initialize the map and reverseMap
    int i = 0;
    for (i = 0; i < NUM_OF_CHARS; i++) {
        codec->map[i] = 0;
    }
    for (int i = 0; i < NUM_OF_CHARS; i++) {
        codec->reverseMap[i] = 0;
    }

    // malloc and initialize the unique_arr in size of 256 available chars
    int * unique_arr = (int *) calloc(NUM_OF_CHARS, sizeof(int));
    if (unique_arr == NULL) {
        return NULL;
    }

    // validate that every key is mapped to a unique char, otherwise return NULL
    for (int i = 0; i < 62; i++) {
        // check if the key is valid
        if (unique_arr[key[i]] == 1) {
            return NULL;
        }
        unique_arr[key[i]] = 1;
    }

    free(unique_arr);

    // deep copy the key
    for (int i = 0; i < 62; i++) {
        if (i < 26){
            codec->map[i + 97] = key[i];
        } else if (i < 52) {
            codec->map[i + 65 - 26] = key[i];
        } else {
            codec->map[i - 52 + 48] = key[i];
        }

        if (i < 26){
            codec->reverseMap[key[i]] = i + 97;
        } else if (i < 52) {
            codec->reverseMap[key[i]] = i + 65 - 26;
        } else {
            codec->reverseMap[key[i]] = i - 52 + 48;
        }

    }

    return codec;
}

// This function receives ponters for: input char array, output char array, codec, and the size of the input array.
// The function should encode the input array and store the result in the output array.
// The function retuerns the number of bytes written to the output array.
int encode (const char * in, char * out, void * codec, int size) {
    int encoded_bytes = 0;

    // verify that the codec is not NULL
    if (codec == NULL) {
        return -1;
    }

    MyCodec * c = (MyCodec *) codec;
    for (int i = 0; i < size; i++) {
        out[i] = c->map[in[i]];
        encoded_bytes++;
    }
    return encoded_bytes;
}

// This function receives ponters for: input char array, output char array, codec, and the size of the input array.
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
        out[i] = c->reverseMap[in[i]];
        decoded_bytes++;
    }
    return decoded_bytes;
}

// This function receives a pointer to a codec and frees all the memory allocated for it.
void freecodec(void * codec) {
    free(codec);
}

int main(int argc, char *argv[]) {
    char key[62] = {0};

    if (argc != 2) {
        printf("Usage: cipher <key>\n");
        return 1;
    }

    // validate the key
    for (int i = 0; i < 62; i++) {
        if (argv[1][i] == '\0') {
            break;
        }
        if (argv[1][i] < 48 || (argv[1][i] > 57 && argv[1][i] < 65) || (argv[1][i] > 90 && argv[1][i] < 97) || argv[1][i] > 122) {
            printf("Invalid key\n");
            return 1;
        }
        key[i] = argv[1][i];
    }

    // create the codec
    void * codec = createCodec(key);
    if (codec == NULL) {
        printf("Error creating codec\n");
        return 1;
    }

    // read from stdin and write to stdout
    char buffer_in[1024] = {0};
    char buffer_out[1024] = {0};
    int read_bytes = 0;
    int encoded_bytes = 0;
    int decoded_bytes = 0;
    while ((read_bytes = read(0, buffer_in, 1024)) > 0) {
        encoded_bytes = encode(buffer_in, buffer_out, codec, read_bytes);
        if (encoded_bytes == -1) {
            printf("Error encoding\n");
            return 1;
        }
        decoded_bytes = decode(buffer_out, buffer_in, codec, encoded_bytes);
        if (decoded_bytes == -1) {
            printf("Error decoding\n");
            return 1;
        }
        write(1, buffer_out, decoded_bytes);
    }

    freecodec(codec);
}

    
    