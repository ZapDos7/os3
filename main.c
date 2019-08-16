#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include "shm.h"
#include "Semun.h"

int main (int argc, char*argv[]) {
///////////////initialize//////////////
    int pnum;
    FILE * fptr;
    if (argc>3) {
        fprintf(stderr, "Incorrect amount of arguements, please fix this issue and start again.\n");
        exit(-1);
    }
    else if (argc==2) {
        pnum = atoi(argv[1]);
        fptr = fopen("randomfile.txt", "r");
    }
    else if (argc==1) {
        pnum = 5; //default value
    }
    else {
        fprintf(stderr, "How did you do that?\n");
        exit(-1);
    }
    printf("%d\n", pnum);
    if (fptr == NULL) {
        fprintf(stderr, "Can't find input file file!\n");
		exit(-1);
    }
///////////////create shared memory//////////////////////
    int shm_id, shm_key;
    if ((shm_key = ftok("main.c", 'O')) == -1) {//lab
	    fprintf(stderr,"Shm key error.\n");
	    exit(-1);
	}
	shm_id = shm_create(shm_key);
///////////////////handle semaphores/////////////////////

//////////////////////fork time//////////////////////////
    int pids[pnum+2]; //table of pids returned by fork, 1 is C, the rest are P, + 1 for parent

    for (int i = 0; i < pnum+2; i++) {
        pids[i] = fork();    //fork
        if (pids[i]==0) {
            printf("My id is %d. I am parent.\n", getpid());
            //handle children
            //kill children
        } 
        else if (pids[i]>0) {
            if(i==0) { //C
                printf("My id is %d. I am child. I am C.\n", getpid());
                //the C: read & hash
                //send back to out-ds
            }
            else {
                printf("My id is %d. I am child.I am P%d\n", getpid(), i);
                //the N Ps: read file
                

                //send struct into in-ds

                //the N Ps: read if out-ds is readable
                //print stuff and update counters
            }
        }
        else {
            fprintf(stderr, "How and why\n");
            exit(-1);
        }
    }
    
    
    //print finishing message

    exit(0);
}