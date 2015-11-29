/* mshell - a job manager */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>


#include "jobs.h"
#include "common.h"
#include "sighandlers.h"

/*
 * wrapper for the sigaction function
 */
int sigaction_wrapper(int signum, handler_t * handler) {
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_handler = handler;
    action.sa_flags = SA_RESTART;
    
    if(sigaction(signum, &action, NULL) < 0){
        perror("error sigaction_wrapper");
        exit(EXIT_FAILURE);
    }else{
        return 0;
    }
}

/*
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children
 */
void sigchld_handler(int sig) {
    /*if (verbose)
        printf("sigchld_handler: entering\n");

    printf("sigchld_handler : To be implemented\n");

    if (verbose)
        printf("sigchld_handler: exiting\n");*/
    
    pid_t child_pid;
    int child_jid;
    int status;
    
    if(verbose){
        printf("sigchild_handler entering\n");
    }
    
    while ((child_pid = waitpid(-1,&status,WNOHANG |WUNTRACED))>0) {
        if(WIFSTOPPED(status)){
            struct job_t *j = jobs_getjobpid(child_pid);
            if(!j){
                perror("error sigchld_handler struct non existante\n ");
                exit(EXIT_FAILURE);
            }
            j->jb_state = ST;
        }else if(WIFSIGNALED(status)){
            child_jid = jobs_pid2jid(child_pid);
            if(jobs_deletejob(child_pid)){
                if (verbose) {
                    printf("sigchld_handler %d delete\n",child_jid);
                }
            }
            fprintf(stdout,"JOB[%d](%d) terminated by signal %d\n",child_jid,child_pid,WTERMSIG(status));
        }else if (WIFEXITED(status)){
            /*si processus bien fini */
            child_jid = jobs_pid2jid(child_pid);
            if(verbose)
                printf("Kill child pid [%d]\n", child_pid);
            if(jobs_deletejob(child_pid) == 0){
                printf("if jobs delete job\n");
                kill(child_pid, SIGINT);
                if (verbose) {
                    printf("sigchld_handler %d delete\n",child_jid);
                }
            }
            fprintf(stdout,"JOB[%d](%d) finished by signal %d\n",child_jid,child_pid,WTERMSIG(status));
        }
        
    }if (verbose) {
        printf("end of while\n");
    }
    if(!((child_pid == 0) ||(child_pid == -1 && errno == ECHILD))){
        /*ERROR*/
        fprintf(stderr, "ERROR child\n");
    }else{
        if(verbose){
            printf("end sigchld_handler fonction\n");
        }
    }
    


    return;
}

/*
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.
 */

void sigint_handler(int sig) {
    pid_t pid;
    if (verbose)
        printf("sigint_handler: entering\n");

    if (verbose){
        printf("sigint_handler: exiting\n");
    }

    if(verbose){
        printf("sigint_handler entering\n");
    }
    if((pid = jobs_fgpid()) > 0){
        if(verbose){
            printf("sending kill to %d\n",pid);
        }
        if(kill(pid,SIGINT) < 0){
            fprintf(stderr, "Error kill job [%d]",pid);
        }
    }

    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.
 */
void sigtstp_handler(int sig) {
    pid_t pid = -1;
    
    if (verbose)
        printf("sigtstp_handler: entering\n");
    if (verbose){
        printf("sigint_handler entering\n");
    }
    if ((pid=jobs_fgpid())>0){
        if (verbose){
            printf("Sending stop to [%d]\n",pid);
        }
        if (kill(pid,SIGTSTP)<0){
            perror("sigstp_handler : l'envoi du stop a échoué.");
            exit(EXIT_FAILURE);
        }
    }
    if (verbose)
        printf("sigtstp_handler: exiting\n");
    
    return;
}
