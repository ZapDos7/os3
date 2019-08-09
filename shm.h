#ifndef _SHM_H_
#define _SHM_H_

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

typedef enum { false, true } bool;

typedef struct trace { //data structure of shared memory
	int pid;
	char * msg;
} trace;



#endif