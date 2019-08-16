#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <unistd.h>

#include "shm.h"
#include "Semun.h"

int main (int argc, char*argv[]) {
    int pnum;
    char * filename;
    if (argc>4) {
        fprintf(stderr, "Incorrect amount of arguements, please fix this issue and start again.\n");
        exit(-1);
    }
    else if (argc==3) {
        pnum = atoi(argv[1]);
        filename = argv[2];
    }
    else if (argc==2) {
        pnum = 5; //default value
    }
    else {
        fprintf(stderr, "How did you do that?\n");
        exit(-1);
    }
    printf("%d\n", pnum);
    printf("%s\n", filename);

    //file reading -> into a buffer for the Ps
    char * buf = 0;
    long length;
    FILE * f = fopen (filename, "rb");
    int errno;
    if (f) {
        int val = fseek (f, 0, SEEK_END);//check the error-code
        if(val!=0) {
            //val=errno;
            fprintf(stderr, "Error code for fseek(1) is %d\n",val);
        }
        length = ftell (f);
        fseek (f, 0, SEEK_SET);//check the error-code
        buf = malloc (length);
        if (buf) {
            fread (buf, 1, length, f);//check the error-code
        }
        fclose (f);//check the error-code
    }
    
    //create shared memory
    //handle semaphores

    int pids[pnum+2]; //table of pids returned by fork, one is C, the rest are P + 1 for parent

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
                if (buf) {
                    //read();
                }

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