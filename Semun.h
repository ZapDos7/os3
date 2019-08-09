#ifndef _SEMUN_H_
#define _SEMUN_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>
//#include <unistd.h>
//#include <sys/types.h>

typedef union semun semun;

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

//my own functions for semaphore handling stuff!
int sem_create(key_t keyt, int num_sems, int val);
int sem_delete(int sem_id);
int sem_down(int sem_id, int sem_num);
int sem_up(int sem_id, int sem_num);
int sem_get(int sem_id, int n);
int sem_set(int sem_id, int n, int val); //say that 15 times faster

#endif