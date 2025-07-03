#include <stdio.h>
#include <unistd.h>

int main()
{
	int fds[2];
	char str[] = "Who are you?";
	char str2[] = "Thank you for your message.";
	char buf[50];
	pid_t pid;

	pipe(fds);
	pid = fork();

	if(pid == 0){
		write(fds[1], str, sizeof(str));
		sleep(2);
		// sleep()없이 바로 프로세스 종료되면 부모프로세스는 파이프에 접근 못 함
		// -> 파이프 2개 만들어서 해결! -> pipe3.c
		read(fds[0], buf, sizeof(buf));
		printf("child proc output: %s\n", buf);
	} else {
		read(fds[0], buf, sizeof(buf));
		printf("parent proc output: %s\n", buf);
		write(fds[1], str2, sizeof(str2));
		sleep(3);
	}

	return 0;
}
