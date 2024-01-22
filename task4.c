#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

void myzip(const char *directory){
    int pipefd[2];

    //create pipe for communication between tar and gzip
    if(pipe(pipefd) == -1){
        perror("pipe create was failed");
        exit(EXIT_FAILURE);
    }

    //fork the tar
    if(!fork()){
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        execlp("tar","tar","-cf","-",directory,NULL);
        perror("Error in tar");
        EXIT(EXIT_FAILURE);
    }
}