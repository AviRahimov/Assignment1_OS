# include "myzip.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <string.h>


#define ONE_MB 1048576 // 1 MB in bytes
#define KB 1024 // 1 KB in bytes
#define CHUNK_SIZE 512 // 4 KB in bytes

int create_tar_file(const char * tarfile, const char **filenames, const int num_files) {
    int tar_fd = open(tarfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (tar_fd == -1){
        perror("Error creating temporary tar file");
        return -1;
    }

    pid_t pid = fork();
    if (pid == 0){
        // Child process
        // Redirect the output of tarfile to gzip
        if (dup2(tar_fd, 1) == -1){
            perror("Error redirecting output");
            return -1;
        }
        // Construct the tar command with multiple filenames
        char* args[num_files + 3]; // +3 for "tar", "-cf", "-", and NULL terminator

        char tr[] = "tar";
        args[0] = tr;
        char cf[] = "-cf";
        args[1] = cf;
        char dash[] = "-";
        args[2] = dash;
        for (int i = 0; i < num_files; ++i) {
            args[i + 3] = (char *)filenames[i];
        }
        args[num_files + 3] = NULL;

        // Execute tar on all the files
        execvp("tar", args);
        perror("Error executing tar");
        return -1; // Add this line to prevent code execution in the child after execlp
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("Collection successful.\n");
            // Temporary file can be used here if needed
        } else {
            printf("Collection failed.\n");
            return -1; // Add this line to handle the failure case
        }
    }
    if (close(tar_fd) == -1){
        perror("Error closing tar file");
        return -1;
    }
    return tar_fd;
}

int compress (const char * tarfile, const char * gzip) {
    int tar_fd = open(tarfile, O_RDONLY);
    if (tar_fd == -1){
        perror("Error opening tar file");
        return -1;
    }
    int gzip_fd = open(gzip, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (gzip_fd == -1){
        perror("Error creating tar file");
        return -1;
    }

    pid_t pid = fork();
    if (pid == -1){
        perror("Error forking");
        return -1;
    }
    if (pid == 0){
        // Child process
        // Redirect the output of tarfile to gzip
        if (dup2(tar_fd, 0) == -1){
            perror("Error redirecting input");
            return -1;
        }
        // Redirect the input of gzip to gzip_fd
        if (dup2(gzip_fd, 1) == -1){
            perror("Error redirecting output");
            return -1;
        }
        // Execute gzip from input to output
        execlp("gzip", "gzip", NULL);
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
            printf("Compression successful.\n");
            // Temporary file can be used here if needed
        } else {
            printf("Compression failed.\n");
        }
    }
    // Close the tar file
    if (close(tar_fd) == -1){
        perror("Error closing tar file");
        return -1;
    }
    // Close the gzip file
    if (close(gzip_fd) == -1){
        perror("Error closing gzip file");
        return -1;
    }
    return gzip_fd;
}

int encrypt_file (const char * compressed_tarfile, const char * gpg) {
    int gzip_fd = open(compressed_tarfile, O_RDONLY);
    int encrypted_fd = open(gpg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    // Encrypt the compressed tar file using gpg
    pid_t pid = fork();
    if (pid == -1){
        perror("Error forking");
        return -1;
    }
    if (pid == 0){
        // Child process
        // Redirect the input of gpg to compressed_tarfile
        if (dup2(gzip_fd, 0) == -1){
            perror("Error redirecting input");
            return -1;
        }
        // Redirect the output of gpg to encrypted_fd
        if (dup2(encrypted_fd, 1) == -1){
            perror("Error redirecting output");
            return -1;
        }
        // Execute gpg and pass passphrase
        execlp("gpg", "gpg", "--batch", "--passphrase-file", "passphrase.txt", "--symmetric", NULL);
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
            printf("Encryption successful.\n");
            // Temporary file can be used here if needed
        } else {
            printf("Encryption failed.\n");
        }
    }
    // Close the compressed tar file
    if (close(gzip_fd) == -1){
        perror("Error closing compressed tar file");
        return -1;
    }
    // Close the encrypted file
    if (close(encrypted_fd) == -1){
        perror("Error closing encrypted file");
        return -1;
    }
    return encrypted_fd;
}

int main(int argc, char *argv[]) {
    // The program should be called with the following arguments:
    // myzip <file1> <file2> ... <fileN>
    
    // Check the number of arguments
    if (argc < 2){
        printf("Usage: %s <file1> <file2> ... <fileN>\n", argv[0]);
        return -1;
    }
    int ret = 0;
    // Create a tar file
    const char ** FILENAMES = (const char **) &argv[1];
    int num_files = argc - 1;
    char tar_tmp [] = "tarXXXXXX";
    ret = create_tar_file (tar_tmp, FILENAMES, num_files);
    if (ret == -1){
        return -1;
    }
    // Compress the tar file
    char gzip_tmp [] = "gzipXXXXXX";
    ret = compress(tar_tmp, gzip_tmp);
    if (ret == -1){
        return -1;
    }
    // Encrypt the compressed tar file
    char gpg_tmp[] = "gpgXXXXXX";
    ret = encrypt_file(gzip_tmp, gpg_tmp);
    if (ret == -1){
        return -1;
    }

    // Remove the temporary files
    if (remove(tar_tmp) == -1){
        perror("Error removing tar file");
        return -1;
    }
    if (remove(gzip_tmp) == -1){
        perror("Error removing gzip file");
        return -1;
    }
    // add .gpg extension to the gpg_tmp file
    //strcat(gpg_tmp, ".gpg");
    // add .tar.gz.gpg extension to the FileNAMES[0] file
    char * filename = (char *) FILENAMES[0];
    strcat(filename, ".tar.gz.gpg");
    if (rename(gpg_tmp, filename) == -1){
        perror("Error renaming file");
        return -1;
    }

    // if (remove(gpg_tmp) == -1){
    //     perror("Error removing gpg file");
    //     return -1;
    // }
    
    return 0;

}