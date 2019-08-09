#include "Semun.h"

int sem_create(key_t keyt, int num_sems, int val) {
	int sem_id,err;
	semun arg;
	if ( (keyt < 0) || (num_sems <= 0) ) {
		fprintf(stderr,"Wrong key or number of semaphores, please fix.\n");
		exit(-1);
	}
	sem_id = semget(keyt, num_sems, IPC_CREAT | 0666);
	if (sem_id < -1) {
		fprintf(stderr,"Error in semget.\n");
		exit(-1);
	}
	arg.val = val;
	for (int i = 0; i < num_sems; i++) {
		err = semctl(sem_id,i,SETVAL,arg);
		if (err < 0) {
			exit(-1);
		}
	}
	return sem_id;
}

int sem_delete(int sem_id) {
    if (sem_id <= -1) {
        fprintf(stderr,"Can't delete a semaphore with that ID.\n");
        exit(-1);
    }
    return semctl(sem_id, 0, IPC_RMID);
}

int sem_down(int sem_id, int sem_num) {
    struct sembuf smbf; //short sem_num; short sem_op; short sem_flg;
    if ( (sem_id < 0) || (sem_num < 0) ) {
        fprintf(stderr,"Error in semaphore down, please fix.\n");
		exit(-1);
    }
    else {
        smbf.sem_num = sem_num;
        smbf.sem_op = -1;
        smbf.sem_flg = 0;
        return semop(sem_id, &smbf, 1);
    }
}

int sem_up(int sem_id, int sem_num) {
    struct sembuf smbf; //short sem_num; short sem_op; short sem_flg;
    if ( (sem_id < 0) || (sem_num < 0) ) {
        fprintf(stderr,"Error in semaphore up, please fix.\n");
		exit(-1);
    }
    else {
        smbf.sem_num = sem_num;
        smbf.sem_op = 1; //h moni allagi metaksu up kai down :3
        smbf.sem_flg = 0;
        return semop(sem_id, &smbf, 1);
    }
}

int sem_get(int sem_id, int n) {
    semun arg;
    if ( (sem_id < 0) || (n < 0) ) {
        fprintf(stderr,"Error in semaphore get, please fix.\n");
        exit(-1);
    }
    return semctl(sem_id, n, GETVAL, arg);
}

int sem_set(int sem_id, int n, int val) {
    semun arg;
    if ( (sem_id < 0) || (n < 0) ) {
        fprintf(stderr,"Error in semaphore get, please fix.\n");
        exit(-1);
    }
    arg.val = val;
    return semctl(sem_id, n, GETVAL, arg);
} 