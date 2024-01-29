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

    // create child process for gpg
    pid_t pid_gpg = fork();
    if (pid_gpg == 0) { 
        int gpg_fd = open(filename, O_RDONLY);
        if (gpg_fd == -1) {
            perror("Error opening gpg file");
            exit(EXIT_FAILURE);
        }
        // redirect input from file
        dup2(gpg_fd, STDIN_FILENO);
        // redirect output to pipe
        dup2(pipe_gpg_gzip[1], STDOUT_FILENO); 
        close(pipe_gpg_gzip[0]);
        close(pipe_gpg_gzip[1]);

        execlp("gpg", "gpg", "--batch", "--passphrase-file", "./passphrase.txt", "--decrypt", NULL);
        perror("gpg");
        exit(EXIT_FAILURE);
    }
    // closing this pipe ends the gpg process from the parent because it is not used for dup2 in the parent anymore
    close(pipe_gpg_gzip[1]);
    
    // create child process for gzip
    pid_t pid_gzip = fork();
    if (pid_gzip == 0) {
        // redirect input from pipe_gpg_gzip inside child process
        dup2(pipe_gpg_gzip[0], STDIN_FILENO);
        // redirect output to pipe_gzip_tar inside child process
        dup2(pipe_gzip_tar[1], STDOUT_FILENO);
        close(pipe_gpg_gzip[0]);
        close(pipe_gzip_tar[1]);
        close(pipe_gzip_tar[0]);

        execlp("gzip", "gzip", "-d", NULL);
        perror("gzip failed");
        exit(EXIT_FAILURE);
    }
    // these are not needed in the parent process for the same reason as above
    close(pipe_gpg_gzip[0]);
    close(pipe_gzip_tar[1]);

    // create child process for tar
    pid_t pid_tar = fork();
    if (pid_tar == 0) {
        // redirect input from pipe_gzip_tar inside child process
        dup2(pipe_gzip_tar[0], STDIN_FILENO);
        // redirect output file to stdout inside child process
        close(pipe_gzip_tar[0]);

        execlp("tar", "tar", "-xv", NULL);
        perror("tar failed");
        exit(EXIT_FAILURE);
    }
    // this is not needed in the parent process for the same reason as above
    close(pipe_gzip_tar[0]); 

    // wait for all child processes to finish
    int status;
    waitpid(pid_gpg, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("Decryption successful.\n");
    } 
    else {
            printf("Decryption failed.\n");
    }
    waitpid(pid_gzip, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        printf("Decompression successful.\n");
    }
    else {
            printf("Decompression failed.\n");
    }
    waitpid(pid_tar, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("Extraction successful.\n");
    }
    else {
            printf("Extraction failed.\n");
    }
    printf("Dercyption, decompression and extraction finished\n");
    return 0;
}
