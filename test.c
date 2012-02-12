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
	fd = open("/dev/cdata", O_RDWR);
	if (fd!=0) {
		printf("test: fd ERROR\n");
	}
	close(fd);
	fd3 = open("/dev/cdata3", O_RDWR);
	if (fd3!=0) {
		printf("test: fd ERROR\n");
	}
	close(fd3);
}
