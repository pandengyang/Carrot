#include "common.h"

#define MAXNITEMS 100000
#define MAXNTHREADS 100

int nitems;
struct {
	pthread_mutex_t mutex;
	int buff[MAXNITEMS];
	int nput;
	int nval;
} shared = {
	PTHREAD_MUTEX_INITIALIZER
};

void *produce (void *arg);
void *consume (void *arg);
void consume_wait (int i);

int main (int argc, char **argv)
{
	int i;
	int nthreads;
	int count[MAXNTHREADS];

	pthread_t tid_produce[MAXNTHREADS];
	pthread_t tid_consume;

	if (3 != argc) {
		printf("%s\n", "Usage: prodcons <#item> <#threads>");
		exit(-1);
	}

	nitems = min(atoi(argv[1]), MAXNITEMS);
	nthreads = min(atoi(argv[2]), MAXNTHREADS);

	for (i = 0; i < nthreads; i++) {
		count[i] = 0;
		pthread_create(&tid_produce[i], NULL, produce, &count[i]);
	}

	pthread_create(&tid_consume, NULL, consume, NULL);

	for (i = 0; i < nthreads; i++) {
		pthread_join(tid_produce[i], NULL);
		printf("count[%d] = %d\n", i, count[i]);
	}

	pthread_join(tid_consume, NULL);

	exit(0);
}

void *produce (void *arg)
{
	for ( ; ; ) {
		pthread_mutex_lock(&shared.mutex);

		if (shared.nput >= nitems) {
			pthread_mutex_unlock(&shared.mutex);

			return (void *) NULL;
		}

		shared.buff[shared.nput] = shared.nval;
		shared.nput++;
		shared.nval++;

		pthread_mutex_unlock(&shared.mutex);
		*((int *) arg) += 1;
	}

	return (void *) NULL;
}

void *consume (void *arg)
{
	int i;

	for (i = 0; i < nitems; i++) {
		consume_wait(i);
		if (shared.buff[i] != i) {
			printf("buff[%d] = %d\n", i, shared.buff[i]);
		}
	}

	return (void *) NULL;
}

void consume_wait (int i)
{
	for ( ; ; ) {
		pthread_mutex_lock(&shared.mutex);

		if (i < shared.nput) {
			pthread_mutex_unlock(&shared.mutex);

			return;
		}

		pthread_mutex_unlock(&shared.mutex);
	}
}
