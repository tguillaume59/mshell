/*/
//  main.c
//  GestionnaireDeTravaux
//
//  Created by guillaume on 04/11/2015.
//  Copyright © 2015 guillaume. All rights reserved.
/*/

#include <stdio.h>
#include <limits.h>
#include <sys/signal.h>
#include <unistd.h>
#include <stdlib.h>

#include "cmd.h"
#include "common.h"
#include "jobs.h"
#include "pipe.h"
#include "sighandlers.h"

#define MAXJOBS 16


int sigaction_wrapper(int signum, handler_t *handler);
void sigint_handler(int sig);

static struct job_t stJob[MAXJOBS];

/*int main(int argc, const char * argv[]) {
    
    return 0;
}*/

int sigaction_wrapper(int signum, handler_t *handler){
    struct sigaction action;
    action.sa_handler = *handler;
    sigemptyset(&action.sa_mask);
    sigaction(signum, &action, NULL);
    
    return 0;
    
}

void sigint_handler(int sig){
    if(verbose){
        printf("sigint_handler entering\n");
    }
    if((pid = jobs_fgpid()) > 0){
        if(verbose){
            printf("sending kill to %d\n",pid);
        }
        if(kill(pid,SIGINT) < 0){
            //error
        }
    }
}

void sigstpp_handler(int sig){
    /* chercher le job en avant plan/*
     /*envoyer le kill(pid sigstop)*/
}
void sigchld_handler(int sig){
    pid_t child_pid;
    int child_jid;
    int status;
    
    if(verbose){
        printf("sigchild_handler entering\n");
        while ((child_pid = waitpid(-1,&status,WNOHANG |WUNTRACED))>0) {
            if(WIFSTOPPED(status)){
                struct job_t *j = jobs_getjobpid(child_pid);
                if(!j){
                    //error
                }
                j->jb_state = ST;
            }else if(WIFSIGNALED(status)){
                child_jid = jobs_pidzjid(child_pid);
                if(jobs_deletejob(child_jid)){
                    if (verbose) {
                        printf("sigchld_handler %d delete\n",child_jid);
                    }
                }
                fprintf(stdout,"JOB[%d](%d) terminated by signal %d\n",child_jid,child_pid,WTERMSIG(status));
            }else if (WIFEXITED(status)){
                /* ... */
            }
            
        }
        if(!((child_pid == 0) ||(child_pid == -1 && erno == ECHILD))){
            /*ERROR*/
        }
    }
}

void waitfg(pid_t pid){
    struct job_t *j = jobs_getjobpid(pid);
    if(!j){
        return;
    }
    while (j->jb_pid == pid && j->jb_state == FG) {
        sleep(1);/*avec sleep on verifie quand meme alors que pause se bloque*/
    }
}