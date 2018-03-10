#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct foo {
	int f_count;
	pthread_mutex_t f_lock;
	int value;
};

struct foo *foo_alloc (void);
void foo_hold (struct foo * fp);
void foo_rele (struct foo *fp)

int main (int argc, char **argv)
{
	printf("This is test\n");
}

struct foo *foo_alloc (void)
{
	struct foo *fp;

	if (NULL != (fp = malloc(sizeof (struct foo))) {
		fp->f_count = 1;
		if (0 != pthread_mutex_init(&fp->f_lock, NULL)) {
			free(fp);

			return NULL;
		}
	}
}

void foo_hold (struct foo *fp)
{
	pthread_mutex_lock(&fp->f_lock);
	fp->f_count++;
	pthread_mutex_unlock(&fp->f_lock);
}

struct foo_rele (struct foo *fp)
{
	pthread_mutex_lock(&fp->f_lock);
	if (--fp->f_count == 0) {
		pthread_mutex_unlock(&fp->f_lock);
		pthread_mutex_destroy(&fp->f_lock);
		free(fp);
	} else {
		pthrad_mutex_unlock(&fp->f_lock);
	}
}
