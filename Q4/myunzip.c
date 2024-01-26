# include "myunzip.h"
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>


int decrypt_file (const char * decrypted_file, const char * gpg) {
    // Open the decrypted file
    int decrypted_fd = open(decrypted_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (decrypted_fd == -1){
        perror("Error opening decrypted file");
        return -1;
    }
    // Open the gpg file
    int gpg_fd = open(gpg, O_RDONLY);
    if (gpg_fd == -1){
        perror("Error creating gpg file");
        return -1;
    }

    pid_t pid = fork();
    if (pid == -1){
        perror("Error forking");
        return -1;
    }
    if (pid == 0){
        // Child process
        // Redirect the output of decrypted file to gpg
        if (dup2(gpg_fd, 0) == -1){
            perror("Error redirecting input");
            return -1;
        }
        // Redirect the input of gpg to gpg_fd
        if (dup2(decrypted_fd, 1) == -1){
            perror("Error redirecting output");
            return -1;
        }
        // Execute gpg from input to output
        execlp("gpg", "gpg", "--batch", "--passphrase-file", "./passphrase.txt", "--decrypt", gpg, NULL);
        perror("Error executing gpg");
        return -1;
    }
    // Parent process
    // Wait for gpg to finish
    else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("Decryption successful.\n");
            // Temporary file can be used here if needed
        } else {
            printf("Decryption failed.\n");
        }
    }
    // Close the decrypted file
    if (close(decrypted_fd) == -1){
        perror("Error closing decrypted file");
        return -1;
    }
    // Close the gpg file
    if (close(gpg_fd) == -1){
        perror("Error closing gpg file");
        return -1;
    }
    return gpg_fd;
}

int decompress (const char * decompressed, const char * gzip){
    // Open the gzip file
    int gzip_fd = open(gzip, O_RDONLY);
    if (gzip_fd == -1){
        perror("Error opening gzip file");
        return -1;
    }
    // Open the decompressed file
    int decompressed_fd = open(decompressed, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (decompressed_fd == -1){
        perror("Error creating decompressed file");
        return -1;
    }

    pid_t pid = fork();
    if (pid == -1){
        perror("Error forking");
        return -1;
    }
    if (pid == 0){
        // Child process
        // Redirect the output of gzip to decompressed
        if (dup2(gzip_fd, 0) == -1){
            perror("Error redirecting input");
            return -1;
        }
        // Redirect the input of decompressed to decompressed_fd
        if (dup2(decompressed_fd, 1) == -1){
            perror("Error redirecting output");
            return -1;
        }
        // Execute gzip from input to output
        execlp("gzip", "gzip", "-d", NULL);
        perror("Error executing gzip");
        return -1;
    }
    // Parent process
    // Wait for gzip to finish
    else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("Decompression successful.\n");
            // Temporary file can be used here if needed
        } else {
            printf("Decompression failed.\n");
        }
    }
    // Close the gzip file
    if (close(gzip_fd) == -1){
        perror("Error closing gzip file");
        return -1;
    }
    // Close the decompressed file
    if (close(decompressed_fd) == -1){
        perror("Error closing decompressed file");
        return -1;
    }
    return decompressed_fd;
}

int extract_tar_file (const char * tarfile){
    // Open the tar file
    int tar_fd = open(tarfile, O_RDONLY);
    if (tar_fd == -1){
        perror("Error opening tar file");
        return -1;
    }

    pid_t pid = fork();
    if (pid == -1){
        perror("Error forking");
        return -1;
    }
    if (pid == 0){
        // Child process
        // Redirect the output of tar to current directory
        if (dup2(tar_fd, 0) == -1){
            perror("Error redirecting input");
            return -1;
        }
        // Execute tar from input to current directory
        execlp("tar", "tar", "-xv",  NULL);
        perror("Error executing tar");
        return -1;
    }
    // Parent process
    // Wait for tar to finish
    else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("Extraction successful.\n");
            // Temporary file can be used here if needed
        } else {
            printf("Extraction failed.\n");
        }
    }
    // Close the tar file
    if (close(tar_fd) == -1){
        perror("Error closing tar file");
        return -1;
    }
    return tar_fd;
}

int main(int argc, char const *argv[])
{
    if (argc != 2){
        printf("Usage: %s <filename>\n", argv[0]);
        return -1;
    }
    // Get the filename
    const char * filename = argv[1];

    char gzip_tmp[] = "gzip_XXXXXX";
    char decrypted_tmp[] = "decrypted_XXXXXX";
    char tar_tmp[] = "tar_XXXXXX";

    // Create temporary files
    int gzip_fd = mkstemp(gzip_tmp);
    if (gzip_fd == -1){
        perror("Error creating temporary gzip file");
        return -1;
    }
    int decrypted_fd = mkstemp(decrypted_tmp);
    if (decrypted_fd == -1){
        perror("Error creating temporary decrypted file");
        return -1;
    }
    int tar_fd = mkstemp(tar_tmp);
    if (tar_fd == -1){
        perror("Error creating temporary tar file");
        return -1;
    }

    // decrypt the file
    if (decrypt_file(decrypted_tmp, filename) == -1){
        return -1;
    }
    // decompress the file
    if (decompress(gzip_tmp, decrypted_tmp) == -1){
        return -1;
    }
    // extract the tar file
    if (extract_tar_file(gzip_tmp) == -1){
        return -1;
    }

    // Remove the temporary files
    if (unlink(gzip_tmp) == -1){
        perror("Error removing temporary gzip file");
        return -1;
    }
    if (unlink(decrypted_tmp) == -1){
        perror("Error removing temporary decrypted file");
        return -1;
    }
    if (unlink(tar_tmp) == -1){
        perror("Error removing temporary tar file");
        return -1;
    }
}
