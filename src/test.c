#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "threadpool.h"
#include "queue.h"

#define JOB_MAX 10

void *run(void *arg);
threadpool_t *pool;

char *args[JOB_MAX] = {
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"10",
};

int main(int argc, char **argv)
{
	int i;
	thread_t tid_producer;

	if ((pool = malloc(sizeof(threadpool_t))) == NULL) {
		return -1;
	}

	if (threadpool_init(pool, 5) == -1) {
		return -1;
	}

	pthread_create(&producer_tid, NULL, producer, NULL);
	pthread_join(producer_tid, NULL);

	threadpool_destroy(pool);
	free(pool);
}

void *producer(void *arg)
{
	int i = 0;
	threadpool_job_t job;

	while (1) {
		job.run = run;
		job.arg = args[i];

		threadpool_in(pool, &job);

		i = ++i % JOB_MAX;

		sleep(1);
	}
}

void *run(void *arg)
{
	char *str = (char *)arg;

	printf("%s\n", str);
}
