#include "hashtable.h"

long int x_to_the_n (int x,int n) { //anti gia pow()
	int number = 1;
	for (int i = 0; i < n; ++i)
		number *= x;
	return(number);
}

node * new_node(int key1, msg msg1) {
	node * i = malloc(sizeof(node));
	i->key = key1;
	//memcpy(&(i->msg),&smg1,sizeof(msg)); //FIX
	i->next = NULL;
	return i;
}

void delete_node(node * nd) {
	//	free(nd->key);
	free(&nd->message);
	free(&nd->next);
	free(nd);
}

////////////////HT funs////////////////

HT* HTcreate() {
	HT * ht = malloc(sizeof(HT));
	ht->size = 50;
	ht->count = 0;
	ht->head = NULL;
	//ht->trail = NULL;
	ht->items = NULL;
}

void HTdestroy(HT* ht) {
	for (int i = 0; i < ht->size; i++) {
		node* nd = ht->items[i];
		if (nd != NULL) {
			delete_node(nd);
		}
	}
	free(ht->items);
	free(ht->head);
	//free(ht->tail);
	free(ht);
}
//my hash function is MD5
int HThash(char * s) {
	long hash = 0;
	int a = 151; int m = 50;
	const int len_s = strlen(s);
    for (int i = 0; i < len_s; i++) {
        hash += (long)x_to_the_n(a, len_s - (i+1)) * s[i];
        hash = hash % m;
    }
    return (int)hash;
}

void HTinsert(node nd, int key, HT* ht) {
	char * s;
	sprintf(s,"%d", key);
	int hashed = HThash(s);//perna to key apo HThash
	ht->count++;
	memcpy((*(ht->items))->next, &nd, sizeof(node)); //vale to node ekei pou prepei
	(*(ht->items))->next->next = NULL; //next tou last = NULL
	return;
}

node * HTsearch(int key, HT * ht) {  //0 if not exists, !0 if exists
	char * s;
	sprintf(s,"%d", key);
	int hashed = HThash(s);//perna to key apo HThash
	//trace tmp = malloc(sizeof(trace));
	node * temp;// = new_node(hashed,);
	if (ht->head->key == hashed) {
		return ht->head;
	}
	else {
		temp = ht->head->next;
		for (int i = 0; i < ht->count; i++) { //iterate the HT and see if the key exists
			if (temp->key == hashed) {
				return temp;
			}
			else {
				temp = temp->next;
				continue;
			}
			return NULL;
		}
	}
}

HT * HTdelete(int key, HT * ht) {
	char * s;
	sprintf(s,"%d", key);
	if (ht->count == 0 ) {
		fprintf(stderr, "Can't delete from an empty list.\n");
		return ht;
	}
	int hashed = HThash(s);//perna to key apo HThash
	if (HTsearch(hashed,ht) == NULL) { //not exists
		fprintf(stderr, "Can't delete what was never there, continue.\n");
		return ht;
	}
	else {
		free(HTsearch(hashed,ht));
		ht->count--;
		return ht;
	}
}