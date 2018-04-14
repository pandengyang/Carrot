#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

struct queue;

typedef struct threadpool_job {
	void *(*run)(void *arg);
	void *arg;
} threadpool_job_t;

typedef struct threadpool {
	struct queue *queue;
	int thread_max;
	pthread_t *threads;
} threadpool_t;

int threadpool_init(threadpool_t *pool, int thread_max);
int threadpool_destroy(threadpool_t *pool);

int threadpool_in(threadpool_t *pool, threadpool_job_t *job);

#endif
