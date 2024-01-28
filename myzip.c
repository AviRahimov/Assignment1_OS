#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return -1;
    }

    int pipe_tar_gzip[2];
    int pipe_gzip_gpg[2];
    int output_file = open("output", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (pipe(pipe_tar_gzip) == -1 || pipe(pipe_gzip_gpg) == -1) {
        perror("Pipe creation failed");
        return -1;
    }

    pid_t pid_tar = fork();
    if (pid_tar == 0) { // Child process for tar
        close(pipe_tar_gzip[0]); // Close unused read end
        dup2(pipe_tar_gzip[1], STDOUT_FILENO); // Redirect output to pipe

        char *tar_args[argc + 2];
        tar_args[0] = (char *)"tar";
        tar_args[1] = (char *)"-cf";
        tar_args[2] = (char *)"-"; // Add "-" to write to stdout
        // for (int i = 1; i < argc; i++) {
        //     tar_args[i + 2] = argv[i];
        // }
        tar_args[3] = argv[1];
        tar_args[argc + 2] = NULL;
        
        execvp("tar", tar_args);
        perror("tar");
        exit(EXIT_FAILURE);
    }
    close(pipe_tar_gzip[1]); // Close write end in parent

    pid_t pid_gzip = fork();
    if (pid_gzip == 0) { // Child process for gzip
        close(pipe_gzip_gpg[0]); // Close unused read end
        dup2(pipe_tar_gzip[0], STDIN_FILENO); // Redirect input from pipe_tar_gzip
        dup2(pipe_gzip_gpg[1], STDOUT_FILENO); // Redirect output to pipe_gzip_gpg

        execlp("gzip", "gzip", NULL);
        perror("gzip");
        exit(EXIT_FAILURE);
    }
    close(pipe_tar_gzip[0]); // Close read end in parent
    close(pipe_gzip_gpg[1]); // Close write end in parent

    pid_t pid_gpg = fork();
    if (pid_gpg == 0) { // Child process for gpg
        dup2(pipe_gzip_gpg[0], STDIN_FILENO); // Redirect input from pipe_gzip_gpg
        dup2(output_file, STDOUT_FILENO); // Redirect output to output_file

        execlp("gpg", "gpg", "--batch", "--passphrase-file", "./passphrase.txt", "--symmetric", NULL);
        perror("gpg");
        exit(EXIT_FAILURE);
    }
    close(pipe_gzip_gpg[0]); // Close read end in parent

    // Wait for all child processes to finish
    waitpid(pid_tar, NULL, 0);
    waitpid(pid_gzip, NULL, 0);
    waitpid(pid_gpg, NULL, 0);
    
    close(output_file);

    printf("Operation completed successfully.\n");
    return 0;
}
