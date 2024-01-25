#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// Function to zip-encrypt a directory
void myzip(char *directory);

// Function to unzip-decrypt a compressed file
void myunzip(char *compressedFile);

int main(int argc, char *argv[]) {
    // Check command-line arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <command> <file/directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Extract command and target parameters
    char *command = argv[1];
    char *target = argv[2];

    // Determine the operation based on the command
    if (strcmp(command, "myzip") == 0) {
        myzip(target);
    } else if (strcmp(command, "myunzip") == 0) {
        myunzip(target);
    } else {
        fprintf(stderr, "Invalid command. Use 'myzip' or 'myunzip'\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

// Function to zip-encrypt a directory
void myzip(char *directory) {
    // Set up arguments for tar
    char *tar_args[] = {"/bin/tar", "czf", "-", directory, NULL};

    // Set up arguments for compress (replace with your preferred compressor)
    char *compress_args[] = {"/bin/gzip", "-c", "-", NULL};

    // Set up arguments for gpg
    char *gpg_args[] = {"/bin/gpg", "-c", "-", NULL};

    // Create pipes
    int pipe1[2], pipe2[2];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // Perform tar in a child process
    if (!fork()) {
        dup2(pipe1[1], STDOUT_FILENO);
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);
        execvp("/bin/tar", tar_args);
        perror("execvp tar failed");
        exit(EXIT_FAILURE);
    }

    // Perform compress in another child process
    if (!fork()) {
        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);
        execvp("/bin/gzip", compress_args);
        perror("execvp gzip failed");
        exit(EXIT_FAILURE);
    }

    // Perform gpg in another child process
    if (!fork()) {
        dup2(pipe2[0], STDIN_FILENO);
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);
        execvp("/bin/gpg", gpg_args);
        perror("execvp gpg failed");
        exit(EXIT_FAILURE);
    }

    // Close all pipes in the parent process
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);

    // Wait for all child processes to finish
    wait(NULL);
    wait(NULL);
    wait(NULL);

    printf("myzip completed, terminating\n");
}

void myunzip(char *compressedFile) {
    // Set up arguments for gpg
    char *gpg_args[] = {"/bin/gpg", "-d", "-", NULL};

    // Set up arguments for uncompress (replace with your preferred decompressor)
    char *uncompress_args[] = {"/bin/gzip", "-df", "-", NULL};

    // Create pipes
    int pipe1[2], pipe2[2];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // Perform gpg in a child process
    if (!fork()) {
        printf("Executing gpg...\n");
        fflush(stdout);

        dup2(pipe1[1], STDOUT_FILENO);
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);
        
        if (execvp("/bin/gpg", gpg_args) == -1) {
            perror("execvp gpg failed");
            exit(EXIT_FAILURE);
        }
    }

    // Perform uncompress in another child process
    if (!fork()) {
        printf("Executing gzip...\n");
        fflush(stdout);

        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);

        if (execvp("/bin/gzip", uncompress_args) == -1) {
            perror("execvp gzip failed");
            exit(EXIT_FAILURE);
        }
    }

    // Close all pipes in the parent process
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);

    // Wait for all child processes to finish
    wait(NULL);
    wait(NULL);

    printf("myunzip completed, terminating\n");
}