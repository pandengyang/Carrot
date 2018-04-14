#include <stdlib.h>
#include <stdio.h>

#include "threadpool.h"
#include "queue.h"

#define JOB_MAX 10

void *run(void *arg);

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
	threadpool_t *pool;
	threadpool_job_t job;

	if ((pool = malloc(sizeof(threadpool_t))) == NULL) {
		return -1;
	}

	if (threadpool_init(pool, 5) == -1) {
		return -1;
	}

	for (i = 0; i < JOB_MAX; i++) {
		job.run = run;
		job.arg = args[i];

		threadpool_in(pool, &job);
	}

	threadpool_destroy(pool);
	free(pool);
}

void *run(void *arg)
{
	char *str = (char *)arg;

	printf("%s\n", str);
}
