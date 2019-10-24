#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;

void *run(void *arg)
{
	printf("hi, pthread\n");
}

int main()
{
	pthread_t thread;
	int ret = 0;
	ret = pthread_create(&thread, NULL, run, NULL);
	if (ret)
	{
		printf("pthread create error\n");
		exit(-1);
	}

	pthread_join(thread, NULL);

	return 0;
}
