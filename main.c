#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>


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
    int sem_key = rand(); //yolo
	int mutex=sem_create(sem_key,1,0);
    /*win,wout,rin,rout?*/
    int full=sem_create(sem_key,1,0);
    int empty=sem_create(sem_key,1,0); //or N???
/////////////////////counters////////////////////////////
    int ppcount=0;//posa tupwthikan apo to idio P
//////////////////////fork time//////////////////////////
    int pids[pnum+2]; //table of pids returned by fork, 1 is C, the rest are P, + 1 for parent
    for (int i = 0; i < pnum+2; i++) {
        pids[i] = fork();    //fork
        if (pids[i] == -1) {
			fprintf(stderr, "Error! Fork failure.\n");
			exit(-1);
        }
///////////////////parent////////////////////////////////
        time_t t;
        int status;
        if (pids[i]==0) {
            printf("My id is %d. I am parent.\n", getpid());
            //handle children
            for (i=0;i<pnum+2;i++) {
                /*waitpid(pids[i]);
                fprintf(stderr, "Target taken down.\n");*/
                /*if ((pids[i] = waitpid(pids[i], &status, WNOHANG)) == -1)
                    perror("wait() error");
                else if (pids[i] == 0) {
                    time(&t);
                    printf("child is still running at %s", ctime(&t));
                    sleep(1);
                }
                else {
                    if (WIFEXITED(status)) {
                        printf("child exited with status of %d\n", WEXITSTATUS(status));
                    }
                    else {
                        puts("child did not exit successfully");
                    }
                }
            }
*/
            
            //kill children
            for (i=0;i<pnum+2;i++) {
                kill(pids[i], SIGKILL);
                fprintf(stderr, "Target taken down.\n");
            }
        } 
        //else if (pids[i]>0) {
/////////////////////////C///////////////////////////////
            if(i==0) { 
                printf("My id is %d. I am child. I am C.\n", getpid());
                //the C: read & hash
                //send back to out-ds
            }
            else {
/////////////////////////P///////////////////////////////
                printf("My id is %d. I am child.I am P%d\n", getpid(), i);
                //the N Ps: read file
                msg * shm_ptr; //init shared memory of msg size
                shm_ptr = shm_attach(shm_id); //attach to shared memory

                //send struct into in-ds

                //the N Ps: read if out-ds is readable
                //print stuff and update counters
                //if idio Ppid = Ppid: ppcounter++;
            }
        }
        else {
            fprintf(stderr, "How and why\n");
            exit(-1);
        }
    }
    
////////////////////////end//////////////////////////////    
    //print finishing message
    fprintf(stdout, "Finished execution, %d times the printed message was from the same process ID.\n", ppcount);
    exit(0);
}