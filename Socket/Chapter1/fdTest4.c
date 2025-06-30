#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void main()
{
	int fd;
	char buf[20];
	ssize_t read_sz;

	fd = open("hello.txt", O_RDONLY);

	if (fd < 0) {
		perror("Could not open\n");
		exit(1);
	}

	read_sz = read(fd, buf, sizeof(buf));
	if (read_sz < 0){
		perror("Failed to read from file\n");
		exit(1);
	}
	printf("Read from file: %s\n", buf);

	close(fd);
}

/*

#include <unistd.h>
ssize_t read(int fd, void* buf, size_t count);

*/
