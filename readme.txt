Ioanna Zapalidi
1115201400044
3rd assignment
Operating Systems
Winter Semester 2018-19

This tarball contains:
~ this readme
~ main.c
~ Makefile
~ shm.c & shm.h (files about shared memory from previous project)
~ Semun.c & Semun.h (files about semaphore handling)
~ hashtable.c & hashtable.h (files about hashtables)

Instructions:
In the proper folder, run the following commands:
1. $make            //creates executable
2. $make run        //execute
3. $make val        //execute with valgrind for memory check
4. $make clean      //cleans directory

Specifications:
~ The chosen IPC way is, like the previous assignment, shared memory (as dictated), which is handled using semaphores.
~ The shared memory has the size of a struct "message", which contains the process id (hashed or not) and a char * which contains the actual message, the line of the text file.
~ If there is no specified N through the command line, the default value is 10.
~ The hash function used is the MD5 from the openssl library.

Notes used:
~ Notes from the course's eclass
~ Files from the previous assignment