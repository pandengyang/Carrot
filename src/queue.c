#include <pthread.h>
#include <stdio.h>
#include "queue.h"

static int queue_is_empty(struct queue *q);
static int queue_is_full(struct queue *q);

int queue_init(struct queue *q)
{
	pthread_mutex_init(&q->mutex, NULL);
	pthread_cond_init(&q->cond_readable, NULL);
	pthread_cond_init(&q->cond_writable, NULL);

	q->front = 0;
	q->rear = 0;
	q->count = 0;

	return 0;
}

int queue_in(struct queue *q, elemtype *pe)
{
	pthread_mutex_lock(&q->mutex);

	while (queue_is_full(q)) {
		printf("wait write\n");
		pthread_cond_wait(&q->cond_writable, &q->mutex);
		printf("receive write\n");
	}

	q->buf[q->rear] = *pe;
	q->rear = (q->rear + 1) % QUEUE_SIZE;
	if (0 == q->count) {
		printf("signal read\n");
		pthread_cond_signal(&q->cond_readable);
	}
	q->count++;

	pthread_mutex_unlock(&q->mutex);

	return 0;
}

int queue_out(struct queue *q, elemtype *pe)
{
	pthread_mutex_lock(&q->mutex);

	while (queue_is_empty(q)) {
		printf("wait read\n");
		pthread_cond_wait(&q->cond_readable, &q->mutex);
		printf("receive read\n");
	}

	*pe = q->buf[q->front];
	q->front = (q->front + 1) % QUEUE_SIZE;
	if (QUEUE_SIZE == q->count) {
		printf("signal write\n");
		pthread_cond_signal(&q->cond_writable);
	}
	q->count--;

	pthread_mutex_unlock(&q->mutex);

	return 0;
}

unsigned int queue_size(struct queue *q)
{
	unsigned int count;

	pthread_mutex_lock(&q->mutex);
	count = q->count;
	pthread_mutex_unlock(&q->mutex);

	return count;
}

static int queue_is_empty(struct queue *q)
{
	return 0 == q->count;
}

static int queue_is_full(struct queue *q)
{
	return QUEUE_SIZE == q->count;
}
