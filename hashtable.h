#ifndef _HT_H_
#define _HT_T_

#include <stdlib.h>
#include <string.h>

#include "shm.h"

long int x_to_the_n (int x,int n);

typedef struct node node;
struct node {
	int key;
	msg message;
	node * next;
};

node * new_node(int key1, msg msg1);
void delete_node(node * nd);

typedef struct HT {
	int size;//size of HT
	int count;//posa objects exei mesa tou
	node * head;
	//node * tail;
	node ** items;
} HT;

HT* HTcreate();
void HTdestroy(HT* ht);
int HThash(char * s);
void HTinsert(node nd, int key, HT* ht); //insert the node nd: key to the array with head as head
node * HTsearch(int key, HT* ht); //0 if not exists, !0 if exists
HT * HTdelete(int key, HT* ht); //delete the value associated with the key from the array with the head as head, else nothing (if keydoesnt exist)

#endif