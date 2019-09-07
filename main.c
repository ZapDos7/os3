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

int main(int argc, char*argv[]) {
/////////////////////////////initialize//////////////////
    //printf("1st arguement is number of P processes, 2nd arguement is number of repetitions. Default values: Pnum = , k = .\n");
    int pnum, k;
    int sumppcount = 0;
    FILE * fptr;

    if ((argc > 4) || (argc == 2)) {
        fprintf(stderr, "Incorrect amount of arguements, please fix this issue and start again.\n");
        exit(-1);
    } else if (argc == 3) {
        pnum = atoi(argv[1]);
        k = atoi(argv[2]);
        fptr = fopen("randomfile.txt", "r");
    } else if (argc == 1) {
        pnum = 5; //default value
        k = 10; //default value
        fptr = fopen("randomfile.txt", "r");
    } else {
        fprintf(stderr, "How did you do that?\n");
        exit(-1);
    }
    if (fptr == NULL) {
        fprintf(stderr, "Can't find input file file!\n");
        exit(-1);
    } else {
        fclose(fptr);
    }
    fptr = NULL;

    fprintf(stdout, "Given numbers:\nP is %d\nK is %d.\n", pnum, k);
///////////////create shared memory//////////////////////
    int shm_id, shm_key;
    if ((shm_key = ftok("main.c", 'O')) == -1) {//lab
        fprintf(stderr, "Shm key error.\n");
        exit(-1);
    }

    shm_key = rand() % 60000 + 10000;
    shm_id = shm_create(shm_key);
///////////////////initi semaphores//////////////////////
    int sem_key = rand(); //yolo

    int fullin = sem_create(sem_key + 1, 1, 0); //initially empty
    int emptyin = sem_create(sem_key + 2, 1, 1); //in-ds

    int fullout = sem_create(sem_key + 3, 1, 0); //initially empty
    int emptyout = sem_create(sem_key + 4, 1, 1); //out-ds me ton C na einai producer kai oi P oi consumers

/*    printf("Parent shm_id = %d \n ", shm_id);
    printf("Parent fullin = %d \n ", fullin);
    printf("Parent emptyin = %d \n ", emptyin);
    printf("Parent fullout = %d \n ", fullout);
    printf("Parent emptyout = %d \n ", emptyout);*/
    //we dont need mutexes afou queue length = 1 message
//////////////////////fork time//////////////////////////
    int pid;
    int status;
    for (int i = 0; i <= pnum; i++) {
        pid = fork(); //fork
        if (pid == -1) {
            fprintf(stderr, "Error! Fork failure.\n");
            exit(-1);
        } else if (pid == 0) {
//////////////////////Ps/////////////////////////////////
            if (i < pnum) {
                FILE * fptr = fopen("randomfile.txt", "r"); //count lines
                int num_of_file_lines = 0;
                char chr;
                chr = getc(fptr);
                while (chr != EOF) {
                    if (chr == '\n') {
                        num_of_file_lines++;
                    }
                    chr = getc(fptr);
                }

                printf("My id is %d. I am child. P%d. Num lines: %d \n", getpid(), i, num_of_file_lines);

                char line_buffer[1024];
                char temp[1024];

                srand(getpid()); //so each P has different seed

                int ppcount = 0; //counter ana P

                //send struct into in-ds
                msgin * shm_ptr_in; //init shared memory of msg size
                msgout * shm_ptr_out;
                shm_ptr_in = shm_attachin(shm_id); //attach to shared memory
                shm_ptr_out = (msgout *) (((char*) shm_ptr_in) + sizeof (msgin)); //cast sto struct mou me offset gia na to baloume sti swsti thesi sto shared memory (sizeof msgin is basically the offset)

                while (1) {
////////////////////file handling////////////////////////
                    fseek(fptr, 0, SEEK_SET); //rewind

                    int temp_line = rand() % num_of_file_lines; //rand()%lines of text file
                    int n = 0;
                    while (fgets(temp, sizeof (temp), fptr) != NULL) {
                        strcpy(line_buffer, temp);
                        n++;

                        if (n >= temp_line) {
                            break;
                        }
                        //fprintf(stderr, "\n%d\n", n);
                    }

                    sem_down(emptyin, 0);
                        shm_ptr_in->pid = getpid();
                        strcpy(shm_ptr_in->line, line_buffer);
                    sem_up(fullin, 0);

                    msgout * tmp;
                    tmp = malloc(sizeof (msgout));

                    //Ps are the consumers now!
                    sem_down(fullout, 0); //read if out-ds is readable
                        memcpy(tmp, shm_ptr_out, sizeof (msgout));
                        tmp->pid = shm_ptr_out->pid; //auto 8a tsekarw an einai idio me to diko m
                        strcpy(tmp->hash, shm_ptr_out->hash);
                    sem_up(emptyout, 0);

                    //check if poison!
                    if (tmp->pid == -1) {
                        free(tmp);
                        break;
                    } else if (tmp->pid == getpid()) {//print stuff aka the hash of out-ds message and check for PID match
                        ppcount++; //if idio Ppid = Ppid: ppcounter++;
                        fprintf(stdout, "I received this hash is 0x%s. ", (tmp->hash/* && 0xff*/));
                        fprintf(stdout, "I got back my own message hashed, my pid is %d.\n", getpid());
                    }//Ps print: hash, my pid, and if !=, pid_original
                    else {
                        //tmp doesn't have a value
                        fprintf(stdout, "I received this hash is 0x%s. ", (tmp->hash/* && 0xff*/));
                        fprintf(stdout, "I got back another's message, tmp pid=%d, my pid = %d.\n", tmp->pid, getpid());
                    }

                    free(tmp);
                }

                shm_detachin(shm_ptr_in);

                fclose(fptr);
                //printf("My id is %d. I am child. P%d. Bye \n", getpid(), i);

                return ppcount;
            }//////////////////////C//////////////////////////////////
            else { //i==pnum ara o C einai o "last"
                printf("My id is %d. Child. I am C. kcouunt= %d \n", getpid(), k);

                int kcount = k; //k--
                msgin * shm_ptr_in;
                msgout * shm_ptr_out;
                shm_ptr_in = shm_attachin(shm_id);
                shm_ptr_out = (msgout *) (((char*) shm_ptr_in) + sizeof (msgin)); //same logic here

                char ctemp[33] = {0};
                msgin * tmp;
                tmp = malloc(sizeof (msgin));
                unsigned char digest[MD5_DIGEST_LENGTH]; //resulting hash, length=16 predefined
                char mdString[33];


                //the C: read & hash
                while (kcount != 0) {//if K times
                    sem_down(fullin, 0);
                        tmp->pid = shm_ptr_in->pid;
                        strcpy(tmp->line, shm_ptr_in->line);
                    sem_up(emptyin, 0);

/////////////////////////////////////md5//////////////////
                    MD5((unsigned char*) &tmp->line, strlen(tmp->line), (unsigned char*) &digest);
                    for (int i = 0; i < 16; i++) {
                        sprintf(&mdString[i * 2], "%02x", (unsigned int) digest[i]);
                    }
                    strncpy(ctemp, mdString, 32);
                    //now C is producer, send back to out-ds

                    sem_down(emptyout, 0);
                        shm_ptr_out->pid = tmp->pid; //dont alter the pid
                        strcpy(shm_ptr_out->hash, ctemp);
                    sem_up(fullout, 0);
                    kcount--;
                }
                if (kcount == 0) {
                    //printf("My id is %d. Child. I am C. killing all Ps \n", getpid());
                    for (int q = 0; q < pnum; q++) { //inform children that it is all over
                        //first do pnum reads - no actual body or need to access data!
                        sem_down(fullin, 0);
                        sem_up(emptyin, 0);

                        //now i poison - give out pnum struct of type {pid=-1, hash=whatever}
                        sem_down(emptyout, 0);
                            shm_ptr_out->pid = -1; //so that each P knows it is over
                            strcpy(shm_ptr_out->hash, "It's over.");
                        sem_up(fullout, 0);
                    }
                } else {
                    fprintf(stderr, "Oh no how did you do this.\n");
                    exit(-1);
                }
                shm_detachin(shm_ptr_in);
                //free(tmp);
                //printf("My id is %d. Child. I am C. Bye\n", getpid());
                return 0;
            }
        }
    }
////////////////////////end//////////////////////////////    
    for (int i = 0; i <= pnum; i++) {
        wait(&status);
        sumppcount = sumppcount + WEXITSTATUS(status);
    }
    
    shm_delete(shm_id);
    fprintf(stdout, "Finished execution with %d P processes and %d repetitions, %d times the printed message was from the same process ID.\n", pnum, k, sumppcount);
    return 0;
}