#include "common.h"

#define MAXNITEMS 100000
#define MAXNTHREADS 100

int nitems;
int buff[MAXNITEMS];

struct {
	pthread_mutex_t mutex;
	int nput;
	int nval;
} shared = {
	PTHREAD_MUTEX_INITIALIZER
};

struct {
	pthread_mutex_t mutex;
	pthread_cont_t cond;
	int nready;
} nready = {
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_COND_INITIALIZER,
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
		pthread_mutex_lock(&put.mutex);

		if (put.nput >= nitems) {
			pthread_mutex_unlock(&put.mutex);

			return (void *) NULL;
		}

		buff[put.nput] = put.nval;
		put.nput++;
		put.nval++;

		pthread_mutex_unlock(&put.mutex);

		pthread_mutex_lock(&nready.mutex);
		if (nready.nready == 0) {
			pthread_cond_signal(&nready.cond);
		}
		nready.nready++;
		pthread_mutex_unlock(&nready.mutex);

		*((int *) arg) += 1;
	}

	return (void *) NULL;
}

void *consume (void *arg)
{
	int i;

	for (i = 0; i < nitems; i++) {
		pthread_mutex_lock(&nready.mutex);
		while (nready.nready == 0) {
			pthread_cond_wait(&nready.cond, &nready.mutex);
		}
		nready.nready--;
		pthread_mutex_unlock(&nready.mutex);

		if (buff[i] != i) {
			printf("buff[%d] = %d\n", i, buff[i]);
		}
	}

	return (void *) NULL;
}
