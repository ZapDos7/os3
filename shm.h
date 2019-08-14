#ifndef _SHM_H_
#define _SHM_H_

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

typedef enum { false, true } bool;

typedef struct msg { //data structure of shared memory
	pid_t pid;
	char * line;
} msg;

int shm_create(key_t keyt);
msg* shm_attach(int shm_id);
int shm_detach(msg * shm_ptr);
int shm_delete(int shm_id);

#endif