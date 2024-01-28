#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return -1;
    }
    const char *filename = argv[1];

    int pipe_gpg_gzip[2];
    int pipe_gzip_tar[2];

    if (pipe(pipe_gpg_gzip) == -1 || pipe(pipe_gzip_tar) == -1) {
        perror("Pipe creation failed");
        return -1;
    }

    pid_t pid_gpg = fork();
    // Child process for gpg
    if (pid_gpg == 0) { 
        int gpg_fd = open(filename, O_RDONLY);
        if (gpg_fd == -1) {
            perror("Error opening gpg file");
            exit(EXIT_FAILURE);
        }
        dup2(gpg_fd, STDIN_FILENO); // Redirect input from file
        dup2(pipe_gpg_gzip[1], STDOUT_FILENO); // Redirect output to pipe
        close(pipe_gpg_gzip[0]);
        close(pipe_gpg_gzip[1]);

        execlp("gpg", "gpg", "--batch", "--passphrase-file", "./passphrase.txt", "--decrypt", NULL);
        perror("gpg");
        exit(EXIT_FAILURE);
    }
    close(pipe_gpg_gzip[1]); // Close write end in parent

    pid_t pid_gzip = fork();
    if (pid_gzip == 0) { // Child process for gzip
        dup2(pipe_gpg_gzip[0], STDIN_FILENO); // Redirect input from pipe_gpg_gzip
        dup2(pipe_gzip_tar[1], STDOUT_FILENO); // Redirect output to pipe_gzip_tar
        close(pipe_gpg_gzip[0]);
        close(pipe_gzip_tar[1]);
        close(pipe_gzip_tar[0]);

        execlp("gzip", "gzip", "-d", NULL);
        perror("gzip");
        exit(EXIT_FAILURE);
    }
    close(pipe_gpg_gzip[0]); // Close read end in parent
    close(pipe_gzip_tar[1]); // Close write end in parent

    pid_t pid_tar = fork();
    if (pid_tar == 0) { // Child process for tar
        dup2(pipe_gzip_tar[0], STDIN_FILENO); // Redirect input from pipe_gzip_tar
        close(pipe_gzip_tar[0]);

        execlp("tar", "tar", "-xv", NULL);
        perror("tar");
        exit(EXIT_FAILURE);
    }
    close(pipe_gzip_tar[0]); 

    // wait for all child processes to finish
    waitpid(pid_gpg, NULL, 0);
    waitpid(pid_gzip, NULL, 0);
    waitpid(pid_tar, NULL, 0);

    printf("Decoding operation completed successfully.\n");
    return 0;
}
