#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
int main(void){
	alarm(50);
	int ret;
	sleep(30);
	ret=alarm(10);
	printf("%d\n",ret);
	pause();
	return 0;
}
