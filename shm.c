#include "shm.h"

int changed(trace trc1, trace trc2) {
	if (trc1.mode != trc2.mode) {
		return 1; //changed
	}
	else {
		return 0; //not changed
	}
}

int shm_create(key_t keyt) {
    return shmget(keyt, sizeof(trace), IPC_CREAT | IPC_EXCL | 0666);
}

trace* shm_attach(int shm_id) {
    return shmat(shm_id, NULL, 0);
}

int shm_detach(trace * shm_ptr) {
    return shmdt(shm_ptr);
}

int shm_delete(int shm_id) {
    return shmctl(shm_id, IPC_RMID, 0);
}