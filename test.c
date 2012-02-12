/*
 * Filename: test.c
 */


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>


int main(void) {
	int fd, fd3;
	char buffer[256];
	fd = open("/dev/cdata", O_RDWR);
	printf("test: fd %d\n", fd);
	close(fd);
	fd3 = open("/dev/cdata3", O_RDWR);
	printf("test: fd3 %d\n", fd3);
	write(fd3, buffer, 0);
	close(fd3);
}
