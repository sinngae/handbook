#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;

pthread_t thread;
pthread_cond_t cond;
pthread_mutex_t mutex;
bool running = true;


void *run(void *arg)
{
	struct timeval now;
	struct timespec outtime;

	printf("RUN_THREAD start ... \n");
	pthread_mutex_lock(&mutex);
	while (running)
	{
		printf("do actions \n");

		// 替换sleep(5), 可被随时唤醒
		gettimeofday(&now, NULL);
		outtime.tv_sec = now.tv_sec + 5;
		//outtime.tv_nsec = now.tv_usec * 1000;
		pthread_cond_timewait(&cond, &mutex, &outtime);
	}
	pthread_mutex_unlock(&mutex);
	printf("RUN_THREAD exit ... \n");
}

int main()
{
	int ret = 0;

	printf(" MAIN start ... \n");

	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&cond, NULL);
	ret = pthread_create(&thread, NULL, run, NULL);
	if (ret)
	{
		printf("pthread create error\n");
		exit(-1);
	}

	char c = getchar();
	flag = false;
	pthread_mutex_lock(&mutex);
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);

	// wait for thread to exit
	pthread_join(thread, NULL);

	printf(" MAIN exit ... \n");
	return 0;
}
