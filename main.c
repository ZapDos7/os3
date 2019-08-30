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
    //printf("1st arguement is number of P processes, 2nd arguement is number of repetitions. Default values: Pnum = , k = .\n");
    int pnum, k;
    FILE * fptr;
    if ((argc>4)||(argc==2)) {
        fprintf(stderr, "Incorrect amount of arguements, please fix this issue and start again.\n");
        exit(-1);
    }
    else if (argc==3) {
        pnum = atoi(argv[1]);
        k = atoi(argv[2]);
        fptr = fopen("randomfile.txt", "r");
    }
    else if (argc==1) {
        pnum = 5; //default value
        k = 10; //default value
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
    fprintf(stdout, "Given numbers:\nP is %d\nK is %d.\n", pnum, k);
////////////////////file handling////////////////////////
    if (fptr) {
        fseek(fptr, 0, SEEK_END);
        long fsize = ftell(fptr);
        fseek(fptr, 0, SEEK_SET);

        char *buffer = malloc(fsize + 1);
        if (buffer) {
            /*fsize =*/ fread(buffer, 1, fsize, fptr);
        }
        fclose(fptr);
    }
    //file contents in buffer so after fork all P's will have that, not very good memorywise but it's faster than N* read and write
///////////////create shared memory//////////////////////
    int shm_id, shm_key;
    if ((shm_key = ftok("main.c", 'O')) == -1) {//lab
	    fprintf(stderr,"Shm key error.\n");
	    exit(-1);
	}
	shm_id = shm_create(shm_key, pnum);
///////////////////handle semaphores/////////////////////
    int sem_key = rand(); //yolo

    int fullin=sem_create(sem_key,1,0); //initially empty
    int emptyin=sem_create(sem_key,1,1); //in-ds

    int fullout=sem_create(sem_key,1,0); //initially empty
    int emptyout=sem_create(sem_key,1,1); //out-ds me ton C na einai producer kai oi P oi consumers
    //we dont need mutexes afou queue length = 1 message
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
            printf("My id is %d. Parent. I am C.\n", getpid());
            //handle children - wait
            //the C: read & hash
            //send back to out-ds
            //inform children that it is all over, k is done
            int sumppcount=0;
            //read all ppcounter[], sum & print
            fprintf(stdout, "Finished execution, %d times the printed message was from the same process ID.\n", sumppcount);
            //kill children
        }
/////////////////////////P///////////////////////////////
        else if (pid==0) {
            printf("My id is %d. I am child. P%d\n", getpid(), i);
            msgin * shm_ptr; //init shared memory of msg size
            shm_ptr = shm_attachin(shm_id); //attach to shared memory
            srand(getpid());
            int ppcount=0; //counter ana P
            /*if (buffer) {

            }*/
            //rand()%lines of text file
            while (1) {
                //the N Ps: read file
                //send struct into in-ds
                //WAIT - semaphore stuff - while i have sent a message, i wait
                //when i get one back i send the next
                //read if out-ds is readable
                //print stuff aka the hash of out-ds message and check for PID match
                //if idio Ppid = Ppid: ppcounter++;
                //send ppcounter to C ? or shared memory?
            }
            
        }

        else {//useless else but you can never know with these magic stuff!
            fprintf(stderr, "How and why\n");
            exit(-1);
        }
    }
    
////////////////////////end//////////////////////////////    
    exit(0);
}


//////////////////md5 stuff//////////////////////////////
    /*
    unsigned char digest[MD5_DIGEST_LENGTH]; //resulting hash, length=16 predefined
    char string[] = "happy go lucky o so happy and lucky";
    MD5((unsigned char*)&string, strlen(string), (unsigned char*)&digest);    

    char mdString[33];

    for(int i = 0; i < 16; i++)
         sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

    printf("md5 digest: %s\n", mdString);
*/