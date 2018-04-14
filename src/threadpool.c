#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "threadpool.h"
#include "queue.h"

void *routine(void *arg);

int threadpool_init(threadpool_t *pool, int thread_max)
{
	int i;
	int j;
	struct queue *queue;
	pthread_t *threads;

	if ((queue = malloc(sizeof(struct queue))) == NULL) {
		return -1;
	}

	if (queue_init(queue) < 0) {
		goto err1;
	}

	pool->queue = queue;;
	pool->thread_max = thread_max;
	if ((threads = malloc(sizeof(pthread_t) * thread_max)) == NULL) {
		goto err2;
	}

	for (i = 0; i < thread_max; i++) {
		if (pthread_create(threads + i, NULL, routine, (void *)pool)
			!= 0) {
			goto err4;
		}
	}
	pool->threads = threads;

	return 0;

err4:
	for (j = 0; j < i; j++) {
		pthread_cancel(threads[j]);
	}

err3:
	free(threads);

err2:
	queue_destroy(queue);

err1:
	free(queue);

	return -1;
}

int threadpool_destroy(threadpool_t *pool)
{
	int i;

	for (i = 0; i < pool->thread_max; i++) {
		pthread_join(pool->threads[i], NULL);
	}
	free(pool->threads);

	queue_destroy(pool->queue);
	free(pool->queue);

	return 0;
}

int threadpool_in(threadpool_t *pool, threadpool_job_t *job)
{
	queue_in(pool->queue, job);
}

void *routine(void *arg)
{
	threadpool_job_t job;
	threadpool_t *pool;

	pool = (threadpool_t *)arg;

	while (1) {
		queue_out(pool->queue, &job);

		job.run(job.arg);
	}
}
