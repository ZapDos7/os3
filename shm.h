#ifndef _SHM_H_
#define _SHM_H_

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

typedef enum { false, true } bool;

struct trace { //domi gia tin shared memory mou
	char address[8];//hex
	char mode; //R or W
	bool pm1;//apo poia process irthe, if true then PM1, else PM2
	//flag edited?
};

typedef struct trace trace;

int changed(trace trc1, trace trc2);

int shm_create(key_t keyt);
trace* shm_attach(int shm_id);
int shm_detach(trace * shm_ptr);
int shm_delete(int shm_id);

#endif