#ifndef __QUEUE_H__
#define __QUEUE_H__

#define QUEUE_SIZE 10
typedef struct thread_pool_job elemtype;

/*
 * mutex 用于同步 front rear count
 * cond_readable:
 *	0 == count
 * cond_writable
 *	QUEUE_SIZE == count
 */
struct queue {
	pthread_mutex_t mutex;
	pthread_cond_t cond_readable;
	pthread_cond_t cond_writable;
	int front; // 指向下一个可读的位置
	int rear; // 指向下一个可写的位置
	int count; // 队列中的元素个数
	elemtype buf[QUEUE_SIZE];
};

int queue_init(struct queue *q);
int queue_in(struct queue *q, elemtype *pe);
int queue_out(struct queue *q, elemtype *pe);
unsigned int queue_size(struct queue *q);

#endif
