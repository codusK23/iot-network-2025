#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <netinet/in.h>

#define MAX_CLIENTS 100

int main(int argc, char **argv)
{
	int server_fd, client_fd;
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);

	struct pollfd fds[MAX_CLIENTS];	//구조체 배열
	int nfds = 1;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(atoi(argv[1]));
	bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
	listen(server_fd, 5);

	fds[0].fd = server_fd;	// 감시 대상 등록(서버 소켓 등록)
	fds[0].events = POLLIN;	// 감시 이벤트 등록(읽기)

	while (1) {
		// fds 구조체 배열을 감시, 감시할 파일 디스크립터는 1개, 무한대기
		int acticity = poll(fds, nfds, -1);

		// 서버 소켓의 발생한 이벤트가 읽기 이벤트면 클라이언트 소켓을 생성한다
		if(fds[0].revents & POLLIN){
			client_fd = accept(server_fd, (struct sockaddr *)&addr, &addrlen);
			if(nfds < MAX_CLIENTS){
				fds[nfds].fd = client_fd;		// 클라이언트 소켓을 fds에 저장한다.
				fds[nfds].events = POLLIN;
				nfds++;
			}
			printf("connected client: %d\n", client_fd);
		}

		for (int i = 1; i < nfds; i++){		// 클라이언트 소켓에 읽기 이벤트가 발생하면
			if(fds[i].revents & POLLIN){
				char buffer[1024] = {0};
				int read_len = read(fds[i].fd, buffer, sizeof(buffer));
				if(read_len <= 0){
					close(fds[i].fd);
					fds[i] = fds[nfds - 1];
					nfds--;
					i--;
					printf("closed client: %d\n", fds[i].fd);
				} else{
					printf("client[%d] %s", fds[i].fd, buffer);
					write(fds[i].fd, buffer, read_len);
				}
			}
		}
	}
}
