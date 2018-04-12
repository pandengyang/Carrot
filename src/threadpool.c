int threadpool_init(threadpool_t *pool)
{
	int i;

	queue_init(pool->queue);
	for (i = 0; i < THREAD_MAX; i++) {
		pthread_createa(pool->threads + i, NULL, routine, pool);
	}
}

void *routine(void *arg)
{
	threadpool_job_t job;
	threadpool_t *pool;

	pool = (threadpool_t *)arg;

	while (1) {
		queue_out(poll->queue, &job);

		job.run(job.arg);
	}
}

int threadpool_in(threadpool_t *pool, threadpool_job_t *job)
{
	queue_in(&pool->queue, job);
}
