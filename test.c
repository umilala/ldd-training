/*
* Filename: test.c
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "cdata.h"

int main(void)
{
	int fd;
	int i;
	pid_t pid;
	char pix_green[4] = {0x00, 0xff, 0x00, 0xff};
	char pix_blue[4] = {0xff, 0x00, 0x00, 0xff};

	i = 10000;
	fd = open("/dev/cdata", O_RDWR);
	//ioctl(fd, CDATA_CLEAR, &i);

	pid = fork();
	if (pid ==0) {
		while(1) {
			write(fd, pix_green, 4);
		}
	} else {
		while(1) {
			write(fd, pix_blue, 4);
		}		
	}
	close(fd);
}
