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
#include "help.h"

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
        fptr = fopen("randomfile.txt", "r");
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
    //size = 2*struct!!!
///////////////////handle semaphores/////////////////////
    int sem_key = rand(); //yolo
	int mutex=sem_create(sem_key,1,0);
    int fullin=sem_create(sem_key,1,0);
    //int emptyin=sem_create(sem_key,1,0); //or N???
    int fullout=sem_create(sem_key,1,0);
    //int emptyout=sem_create(sem_key,1,0); //or N???
/////////////////////counters////////////////////////////
    int ppcount=0;//posa tupwthikan apo to idio P
//////////////////md5 stuff//////////////////////////////
    
    unsigned char digest[MD5_DIGEST_LENGTH]; //resulting hash
    char string[] = "happy go lucky o so happy and lucky";
    MD5((unsigned char*)&string, strlen(string), (unsigned char*)&digest);    

    char mdString[33];

    for(int i = 0; i < 16; i++)
         sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

    printf("md5 digest: %s\n", mdString);


//////////////////////fork time//////////////////////////
    int pid;
    for (int i = 0; i < pnum; i++) {
        pid = fork();//fork
        if (pid == -1) {
			fprintf(stderr, "Error! Fork failure.\n");
			exit(-1);
        }
/////////////////////////C///////////////////////////////
        else if (pid!=0) {
            printf("My id is %d. I am parent.I am C.\n", getpid());
            //handle children - wait
            //the C: read & hash
            //send back to out-ds
            //kill children
        }
/////////////////////////P///////////////////////////////
        else if (pid==0) {
            printf("My id is %d. I am child.I am P%d\n", getpid(), i);
            //the N Ps: read file
            msg * shm_ptr; //init shared memory of msg size
            shm_ptr = shm_attach(shm_id); //attach to shared memory
            //send struct into in-ds
            //read if out-ds is readable
            //print stuff and update counters
            //if idio Ppid = Ppid: ppcounter++;
        }
        else {//useless else but you can never know with these magic stuff!
            fprintf(stderr, "How and why\n");
            exit(-1);
        }
    }
    
////////////////////////end//////////////////////////////    
    //print finishing message
    fprintf(stdout, "Finished execution, %d times the printed message was from the same process ID.\n", ppcount);
    exit(0);
}
