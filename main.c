#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <unistd.h>

#include "shm.h"
#include "Semun.h"

int main (int argc, char*argv[]) {
    int pnum;
    if (argc>3) {
        fprintf(stderr, "Incorrect amount of arguements, please fix this issue and start again.\n");
        exit(-1);
    }
    else if (argc==2) {
        pnum = atoi(argv[1]);
    }
    else if (argc==1) {
        pnum = 5; //default value
    }
    else {
        fprintf(stderr, "How did you do that?\n");
        exit(-1);
    }

    printf("%d\n", pnum);
    
    //create shared memory
    //handle semaphores

    int pids[pnum+2]; //table of pids returned by fork, one is C, the rest are P + 1 for parent

    for (int i = 0; i < pnum+2; i++) {
        pids[i] = fork();    //fork
        if (pids[i]==0) {
            printf("My id is %d. I am parent.\n", getpid());
        } 
        else if (pids[i]>0) {
            printf("My id is %d. I am child.\n", getpid());
        }
        else {
            fprintf(stderr, "How and why\n");
            exit(-1);
        }
    }
    

    //the N Ps: read file
    //send struct into in-ds
    //the C: read & hash
    //send back to out-ds
    //the N Ps: read if out-ds is readable
    //print stuff and update counters

    //kill processes
    
    //print finishing message

    exit(0);
}