#include <stdio.h>
#include <unistd.h>

int main()
{
	int fds[2];
	char str[] = "who are you?";
	char buf[20];

	pid_t pid;

	pipe(fds);		// 부모와 자식프로세스가 접근할 수 있는 파이프 생성
	pid = fork();
	if(pid == 0) write(fds[1], str, sizeof(str));
	else{
		read(fds[0], buf, sizeof(buf));
		puts(buf);
	}

	return 0;
}
