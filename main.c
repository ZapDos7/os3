#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <unistd.h>

#include "shm.h"

int main (int argc, char*argv[]) {
    if (argc!=2) {
        fprintf(stderr, "Incorrect amount of arguements, please fix this issue and start again.\n");
        exit(-1);
    }
    
    int pnum = atoi(argv[1]);
    printf("%d\n", pnum);
    exit(0);
}