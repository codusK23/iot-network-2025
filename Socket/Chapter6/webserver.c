#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main(int argc, char** argv)
{
	int serv_sock, clnt_sock;
	int read_cnt;
	char buf[BUF_SIZE];

	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_sz;

	FILE* fp;

	char webpage[] = "HTTP/1.1 200 OK\r\n"
					"Server:Linux Web Server\r\n"
					"Content-Type: text/html; charset=UTF-8\r\n\r\n"
					"<!DOCTYPE html>\r\n"
					"<html><head><title> My Web Page </title>\r\n"
					"<style>body {background-color: #FFFACD}</style></head>\r\n"
					"<body><center><h1>Hello World!</h1><br>\r\n"
					"<img src=\"yeppi.jpg\" width=\"500\"></center></body></html>\r\n";

	if(argc != 2){
		printf("%s <port>\n", argv[0]);
		exit(1);
	}

	fp =  fopen("yeppi.jpg", "rb");
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
		perror("bind() error");
		close(serv_sock);
		exit(1);
	}

	if(listen(serv_sock, 5) == -1){
		perror("listen() error");
		close(serv_sock);
		exit(1);
	}

	clnt_addr_sz = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);

	while(1){
		if(strstr(buf, "GET /yeppi.jpg") != NULL){
			char img_header[] = "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\n\r\n";
			write(clnt_sock, img_header, sizeof(img_header));

			while((read_cnt = fread(buf, 1, BUF_SIZE, fp)) > 0)
				write(clnt_sock, buf, read_cnt);
			fclose(fp);
		} else{
			write(clnt_sock, webpage, sizeof(webpage));
		}

		close(clnt_sock);
	}

	close(serv_sock);
	return 0;
}
