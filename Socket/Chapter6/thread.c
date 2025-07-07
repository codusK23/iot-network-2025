#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread_main(void*);

int main()
{
	pthread_t t_id;			// thread id를 저장할 변수
	int thread_arg = 5;

	// thread id, 기본 thread, 실행될 thread 함수, thread 함수의 입력
	if(pthread_create(&t_id, NULL, thread_main, &thread_arg) != 0){
		puts("pthread_creat() error");
		return -1;
	}

	sleep(10);
	puts("end of main");

	return 0;
}

void* thread_main(void* arg){
	int i;
	int cnt = *((int*)arg);
	for(i = 0; i < cnt; i++){
		sleep(1);
		puts("running thread");
	}
}
