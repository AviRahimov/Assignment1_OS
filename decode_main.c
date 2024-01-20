#include "polyalphabetic_codec.h" // Assuming the codec functions are in this header file

int main(int argc, char *argv[]) {
    char key[62] = {0};

    if (argc != 2) {
        printf("Usage: decode <key>\n");
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

    // read from stdin, decode, and write to stdout
    char buffer_in[1024] = {0};
    char buffer_out[1024] = {0};
    int read_bytes = 0;
    int decoded_bytes = 0;
    while ((read_bytes = read(0, buffer_in, 1024)) > 0) {
        decoded_bytes = decode(buffer_in, buffer_out, codec, read_bytes);
        if (decoded_bytes == -1) {
            printf("Error decoding\n");
            return 1;
        }
        write(1, buffer_out, decoded_bytes);
    }

    freecodec(codec);
}
