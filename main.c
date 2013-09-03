#include <stdio.h>
#include <math.h>
#include "galois.h"

int w = 3;		// log cardinality of field
int m = 3;		// save drives
int n = 3;		// number of drives

unsigned long subspace;

unsigned char* table = 0;
unsigned long first = 0;
int* as = 0;


struct solution {
	unsigned long entry;
	struct solution* next;
} s;
typedef struct solution* item;
item head = 0;
item tail = 0;

void create_table(int w) {
	int i, size;
	if(w <= 3) {
		size = 1;
	} else {
		size = (int)1<<(w-3);
	}
	table = (unsigned char*)malloc(size);
	for(i = 0; i < size; i++) table[i] = 0;
}
void remove_table() {
	free(table);
	table = 0;
}
void flag_table(unsigned long pos) {
	table[pos / 16] |= (1 << (pos % 16));
}
void flag_dependend_table(unsigned long pos) {
	int i, j;
	subspace = 1 >> (n - 1) + 1 >> m - 1;
	do {
		for(i = 0; i < 1 << (m * w); i++) {
			
		}
	} while(nextsubspace());
}
int isflagged_table(unsigned long pos) {
	return (table[pos / 16] & (1 << (pos % 16))) != 0;
}
int first_unflagged_table() {
	while(isflagged_table(first))
		first++;
	return first;
}

void initsubspace() {
	//subspace = 
	return;
}
int nextsubspace() {
	unsigned int t;
	if(subspace & (1 << n)) return 0;
	t = (subspace | (subspace - 1)) + 1;
	subspace = t | ((((t & -t) / (subspace & -subspace)) >> 1) - 1);
	return 1;
}

void remove_solution() {
	while(head != 0) {
		tail = head->next;
		free(head);
		head = tail;
	}
	tail = 0;
}
void add_solution(unsigned long sol) {
	if(head == 0) {
		head = (item)malloc(sizeof(s));
		head->entry = sol;
		tail = head;
		tail->next = 0;
	} else {
		tail->next = (item)malloc(sizeof(s));
		tail->next->entry = sol;
		tail->next->next = 0;
		tail = tail->next;
	}
}

unsigned long vector2long(int* as) {
	unsigned long ret = 0;
	int i;
	for(i = 0; i < w; i++) {
		ret |= (unsigned long)as[i] << (m * i);
	}
	return ret;
}
int long2vectori(unsigned long pos, int component) {
	return (pos & ((1 << (m * (component + 1))) - 1)) >> (m * component);
}

int main(void)
{

	create_table(w * m);
	as = (int*)malloc(w);
	/*//as[0] = 1;
	//as[1] = 5;
	//as[2] = 7;
	//printf("test: %lu\n", vector2long(as));
	//printf("test: %lu\n", long2vectori(vector2long(as), 0));
	//printf("test: %lu\n", long2vectori(vector2long(as), 1));
	//printf("test: %lu\n", long2vectori(vector2long(as), 2));*/

	subspace = 135;
	nextsubspace();
	printf("test: %lu\n", subspace);

	//free((void*)as);
	remove_table();
	remove_solution();
	return 0;
}

