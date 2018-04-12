#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "queue.h"

#define THREAD_MAX 20

struct threadpool_job {
	void (*run)(void *arg);
	void *arg;
} threadpool_job_t;

struct threadpool {
	struct queue queue;
	pthread_t threads[THREAD_MAX];
} threadpool_t;

int threadpool_init(struct threadpool *tp);
int threadpool_destroy(struct threadpool *tp);

int threadpool_in(struct threadpool_job *tp_job);

#endif
