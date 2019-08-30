#ifndef _SHM_H_
#define _SHM_H_

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h> 

typedef enum { false, true } bool;

typedef struct msgin { //data structure of shared memory "in-ds"
	pid_t pid; //4 bytes
	char line[1024]; //random selection of number
	//dunati epektasi: me thn arxiki anagnwsh arxeiou na kratame to max lin length kai na to vazoume edw
} msgin;

typedef struct msgout {
	pid_t pid; //4 bytes
	int hash; //it's hex of 32 bytes actually + '\0'
} msgout;

int shm_create(key_t keyt, int n);
msgin* shm_attachin(int shm_id);
int shm_detachin(msgin * shm_ptr);
msgout* shm_attachout(int shm_id);
int shm_detachout(msgout * shm_ptr);
int shm_delete(int shm_id);

#endif