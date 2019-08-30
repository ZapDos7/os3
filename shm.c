#include "shm.h"

//create
int shm_create(key_t keyt, int n) {
    return shmget(keyt, sizeof(msgin)+sizeof(msgout)+n*sizeof(int), IPC_CREAT | IPC_EXCL | 0666);
}
//in attach && detach
msgin* shm_attachin(int shm_id) {
    return shmat(shm_id, NULL, 0);
}
int shm_detachin(msgin * shm_ptr) {
    return shmdt(shm_ptr);
}
//out attach && detach
msgout* shm_attachout(int shm_id) {
    return shmat(shm_id, NULL, 0);
}
int shm_detachout(msgout * shm_ptr) {
    return shmdt(shm_ptr);
}
//delete
int shm_delete(int shm_id) {
    return shmctl(shm_id, IPC_RMID, 0);
}

/*
we would need to have different create & delete functions for the in-ds and out-ds if we had 2 shared memories but now we have 1 so we only need different attach/detach functions
*/