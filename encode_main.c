#include "polyalphabetic_codec.h"
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: encode <src_file> <dst_file>\n");
        return 1;
    }

    // Open the source file and read the string to be encoded
    int src_fd = open(argv[1], O_RDONLY);
    if (src_fd == -1) {
        perror("Error opening source file");
        return 1;
    }

    struct stat st;
    stat(argv[1], &st);
    int size = st.st_size;

    char *buffer_in = malloc(size);
    if (read(src_fd, buffer_in, size) != size) {
        printf("Error reading from source file\n");
        return 1;
    }
    close(src_fd);

    // create the codec
    void * codec = createCodec();
    if (codec == NULL) {
        printf("Error creating codec\n");
        return 1;
    }

    // encode the string
    char *buffer_out = malloc(size);
    int encoded_bytes = encode(buffer_in, buffer_out, codec, size);
    if (encoded_bytes == -1) {
        printf("Error encoding\n");
        return 1;
    }

    // Open the destination file and write the encoded string
    int dst_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst_fd == -1) {
        perror("Error opening destination file");
        return 1;
    }
    if (write(dst_fd, buffer_out, encoded_bytes) != encoded_bytes) {
        printf("Error writing encoded string to destination file\n");
        return 1;
    }
    close(dst_fd);

    freecodec(codec);

    free(buffer_in);
    free(buffer_out);

    return 0;
}
