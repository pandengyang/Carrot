#include <stdio.h>

typedef int queue_item;

struct queue {
	int front;
	int tail;
	queue_item items[MAX_N_ITEMS];
};
