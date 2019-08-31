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
    if (fptr == NULL) {
        fprintf(stderr, "Can't find input file file!\n");
		exit(-1);
    }
    fprintf(stdout, "Given numbers:\nP is %d\nK is %d.\n", pnum, k);
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
        else if (pid!=0) {
            //parent controls all but does not much
            //kill kids
       }
        else if (pid==0) {
            if (i!=0) {
                printf("My id is %d. I am child. P%d\n", getpid(), i);
                //while (1) {
                ////////////////////file handling////////////////////////
                int num_of_file_lines=0;
                char chr;
                chr = getc(fptr);
                while (chr != EOF) {
                    if (chr == '\n') {
                        num_of_file_lines++;
                    }
                    chr = getc(fptr);
                }
                fprintf(stderr, "Num of lines in text file = %d\n", num_of_file_lines);
                fseek(fptr, 0, SEEK_SET); //rewind
                //now put in lines
                char buffer[num_of_file_lines+1][1024];
                char temp[1024]; //bad practice!
                int i=0;
                while (fgets(temp, sizeof(temp), fptr)!=NULL) {
                    //fprintf(stderr, "%s\n", temp);
                    strcpy(buffer[i], temp);
                    i++;
                    //fprintf(stderr, "\n%d\n", i);
                }
                fclose(fptr);
                    srand(getpid()); //so each P has different seed
                    int ppcount=0; //counter ana P
                    int temp_line = rand()%num_of_file_lines; //rand()%lines of text file
                    char * ptemp=NULL;
                    ptemp = malloc(1024);
                    strcpy(ptemp, buffer[temp_line]);
                    //send struct into in-ds
                    msgin * shm_ptr; //init shared memory of msg size
                    shm_ptr = shm_attachin(shm_id); //attach to shared memory
                    shm_ptr = malloc(sizeof(msgin));
                    sem_down(emptyin, 1);
                        shm_ptr->pid = getpid();
                        strcpy(shm_ptr->line, ptemp);
                    sem_up(fullin, 1);
                    
                    //WAIT - semaphore stuff - while i have sent a message, i wait
                    //when i get one back i send the next
                    //read if out-ds is readable
                    //print stuff aka the hash of out-ds message and check for PID match
                    //if idio Ppid = Ppid: ppcounter++;
                    //send ppcounter to shared memory?
                    free(ptemp);
                //}
            }
            else { //i==0
                printf("My id is %d. Child. I am C.\n", getpid());
                int sumppcount=0;
                msgin * shm_ptr;
                shm_ptr = shm_attachin(shm_id);
                int ctemp; //edw 8a balw to hash
                msgin * tmp;
                tmp = malloc(sizeof(msgin));
                //the C: read & hash
                sem_down(fullin, 1);
                    memcpy(tmp, shm_ptr,sizeof(msgin)); //correct? probably not!!
                    tmp->pid = shm_ptr->pid;
                    strcpy(tmp->line, shm_ptr->line);
                sem_up(emptyin, 1);
                /////////////////////////////////////md5//////////////////
                unsigned char digest[MD5_DIGEST_LENGTH]; //resulting hash, length=16 predefined
                MD5((unsigned char*)&tmp->line, strlen(tmp->line), (unsigned char*)&digest);    
                char mdString[33];
                for(int i = 0; i < 16; i++) {
                    sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
                }
                printf("md5 digest: %s\n", mdString);
                ctemp=atoi(mdString);
            
                //now C is producer
                msgout * shm_ptr2;
                shm_ptr2 = shm_attachout(shm_id);
                sem_down(emptyout, 1);
                    //write
                    sem_up(fullout, 1);


                    //send back to out-ds
                    //if K times
                    //inform children that it is all over, k is done - signal?
                    //read all ppcounter[], sum & print
                    fprintf(stdout, "Finished execution with %d P processes and %d repetitions, %d times the printed message was from the same process ID.\n", pnum, k, sumppcount);
                    //kill children
    
            }
        }
    }
    

////////////////////////end//////////////////////////////    
    exit(0);
}