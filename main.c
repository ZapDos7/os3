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
/////////////////////////////initialize//////////////////
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
///////////////////initi semaphores//////////////////////
    int sem_key = rand(); //yolo

    int fullin=sem_create(sem_key,1,0); //initially empty
    int emptyin=sem_create(sem_key,1,1); //in-ds

    int fullout=sem_create(sem_key,1,0); //initially empty
    int emptyout=sem_create(sem_key,1,1); //out-ds me ton C na einai producer kai oi P oi consumers
    //we dont need mutexes afou queue length = 1 message
//////////////////////fork time//////////////////////////
    int pid;
    int status;
    for (int i = 0; i < pnum; i++) {
        pid = fork();//fork
        if (pid == -1) {
			fprintf(stderr, "Error! Fork failure.\n");
			exit(-1);
        }
//////////////////////Parent/////////////////////////////
        else if (pid!=0) {
            wait(&status);
            //read all ppcounter[], sum & print
            int sumppcount=0;
            shm_delete(shm_id); //check
            fprintf(stdout, "Finished execution with %d P processes and %d repetitions, %d times the printed message was from the same process ID.\n", pnum, k, sumppcount);
            kill(pid, SIGKILL); //kill children
            fclose(fptr);
       }
        else if (pid==0) {
//////////////////////Ps/////////////////////////////////
            if (i!=pnum) {
                printf("My id is %d. I am child. P%d\n", getpid(), i);
                while (1) {
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
                    srand(getpid()); //so each P has different seed
                    int ppcount=0; //counter ana P
                    int temp_line = rand()%num_of_file_lines; //rand()%lines of text file
                    char * ptemp=NULL;
                    ptemp = malloc(1024);
                    strcpy(ptemp, buffer[temp_line]);
                    //send struct into in-ds
                    msgin * shm_ptr_in; //init shared memory of msg size
                    shm_ptr_in = shm_attachin(shm_id); //attach to shared memory
                    shm_ptr_in = malloc(sizeof(msgin));
                    sem_down(emptyin, 1);
                        shm_ptr_in->pid = getpid();
                        strcpy(shm_ptr_in->line, ptemp);
                    sem_up(fullin, 1);
                        
                    //WAIT - semaphore stuff - while i have sent a message, i wait
                    //when i get one back i send the next
                    msgout * shm_ptr_out; //struct {pid + int}
                    shm_ptr_out = shm_attachout(shm_id+sizeof(msgin));
                    shm_ptr_out = malloc(sizeof(msgout));
                    msgout * tmp;
                    tmp = malloc(sizeof(msgout));
                    //Ps are the consumers now!
                    sem_down(fullout, 1);//read if out-ds is readable
                        //memcpy(tmp,shm_ptr_out, sizeof(msgin));
                        tmp->pid = shm_ptr_out->pid; //auto 8a tsekarw an einai idio me to diko m
                        tmp->hash = shm_ptr_out->hash; //auto to tupwnw always!
                    sem_up(emptyout, 1);

                    //check it poison!
                    if (tmp->pid==-1) {
                        //steile to ppcount sto shared memory!
                        exit(0);
                    }
                    else if (tmp->pid==getpid()) {//print stuff aka the hash of out-ds message and check for PID match
                        ppcount++;//if idio Ppid = Ppid: ppcounter++;
                        fprintf(stdout, "I received this hash is 0x%x. ", (tmp->hash/* && 0xff*/));
                        fprintf(stdout, "I got back my own message hashed, my pid is %d.\n", getpid());
                    }//Ps print: hash, my pid, and if !=, pid_original
                    else {
                        fprintf(stdout, "I received this hash is 0x%x. ", (tmp->hash/* && 0xff*/));
                        fprintf(stdout, "I got back another's message, tmp pid=%d, my pid = %d.\n", tmp->pid, getpid());
                    }
                    shm_detachin(shm_ptr_in);
                    shm_detachout(shm_ptr_out);
                    free(ptemp);
                    free(tmp);
                }
            }
//////////////////////C//////////////////////////////////
            else { //i==pnum ara o C einai o "last"
                printf("My id is %d. Child. I am C.\n", getpid());
                int kcount=k; //k--
                msgin * shm_ptr_in;
                shm_ptr_in = shm_attachin(shm_id);
                int ctemp; //edw 8a balw to hash
                msgin * tmp;
                tmp = malloc(sizeof(msgin));
                unsigned char digest[MD5_DIGEST_LENGTH]; //resulting hash, length=16 predefined
                char mdString[33];
                msgout * shm_ptr_out;
                shm_ptr_out = shm_attachout(shm_id+sizeof(msgin));

                //the C: read & hash
                while(kcount!=0) {//if K times
                    sem_down(fullin, 1);
                        //memcpy(tmp, shm_ptr_in,sizeof(msgin)); //correct? probably not!!
                        tmp->pid = shm_ptr_in->pid;
                        strcpy(tmp->line, shm_ptr_in->line);
                    sem_up(emptyin, 1);
                    /////////////////////////////////////md5//////////////////
                    MD5((unsigned char*)&tmp->line, strlen(tmp->line), (unsigned char*)&digest);    
                    for(int i = 0; i < 16; i++) {
                        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
                    }
                    //printf("md5 digest: %s\n", mdString);
                    ctemp=atoi(mdString);
                    //now C is producer, send back to out-ds
                    shm_ptr_out = malloc(sizeof(msgout));
                    sem_down(emptyout, 1);
                        //write
                        shm_ptr_out->pid = tmp->pid; //dont alter the pid
                        shm_ptr_out->hash = ctemp;
                    sem_up(fullout, 1);
                    kcount--;
                }
                if(kcount==0) {
                    for (int q = 0; q < pnum; q++) { //inform children that it is all over
                        //first do pnum reads
                        sem_down(fullin, 1);
                            //dummy read so that another P can write in
                            //memcpy(tmp, shm_ptr_in,sizeof(msgin)); //correct? probably not!!
                            tmp->pid = shm_ptr_in->pid;
                            strcpy(tmp->line, shm_ptr_in->line);
                        sem_up(emptyin, 1);
                        //now i poison
                        //give out pnum struct of type {pid=-1, hash=whatever}
                        sem_down(emptyout, 1);
                            //write
                            shm_ptr_out->pid = -1;//so that each P knows it is over
                            shm_ptr_out->hash = getpid(); //doesn't matter but it's an int
                        sem_up(fullout, 1);
                        //I do the sum
                        //I send the sum to Parent?
                    }
                }
                else {
                    fprintf(stderr, "Oh no how did you do this.\n");
                    exit(-1);
                }
                shm_detachin(shm_ptr_in);
                shm_detachout(shm_ptr_out);
                //shm_delete(shm_id); //is this enough? does parent do this?
                free(tmp);
                //ctemp is int -> cant be freed
            }
        }
        else {
            fprintf(stderr, "How and why?\n");
            exit(-1);
        }
    }
////////////////////////end//////////////////////////////    
    exit(0);
}