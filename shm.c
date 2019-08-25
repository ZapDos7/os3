#include "shm.h"


int shm_create(key_t keyt) {
    return shmget(keyt, sizeof(msg), IPC_CREAT | IPC_EXCL | 0666);
}

msg* shm_attach(int shm_id) {
    return shmat(shm_id, NULL, 0);
}

int shm_detach(msg * shm_ptr) {
    return shmdt(shm_ptr);
}

int shm_delete(int shm_id) {
    return shmctl(shm_id, IPC_RMID, 0);
}
/*
int get_pid() {
    return msg.pid;
}
char * get_msg() {

}

void set_pid(pid_t ppid) {

}
void set_msg(char * mmsg){

}*/