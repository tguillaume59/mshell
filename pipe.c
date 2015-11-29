/* mshell - a job manager */

#include <stdio.h>
#include "pipe.h"

void do_pipe(char *cmds[MAXCMDS][MAXARGS], int nbcmd, int bg) {
    printf("Not implemented yet\n");

    return;
}

/*void commande1(){
    int fds[2];
    int status;
    pid_t pid;
    
    assert(pipe(fds) == 0);
    switch (pid = fork()) {
        case -1:
            //erreur //
            break;
        case 0:
            close(fds[0]);
            dup2(fds[1],STDOUT_FILENO);
            close(fds[1]);
            execvp(argcmd1[0],argvcmd1);
            perror("execvp");
            exit(EXIT_FAILURE);
        default:
            break;
    }
    close(fds[0]);
    close(fds[1]);
    wait(NULL);
    wait(NULL);
    exit(EXIT_SUCESS);
}*/
