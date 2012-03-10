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

	//pid = fork();

	fd = open("/dev/cdata", O_RDWR);

	mmap(0, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	sleep(60);//for checking the mmap, cat /proc/xxx/maps


	//ioctl(fd, CDATA_CLEAR, &i);
	//pid = fork();
/*
	if (pid ==0) {
		while(1) {
			write(fd, pix_green, 4);
		}
	} else {
		while(1) {
			write(fd, pix_blue, 4);
		}		
	}*/

/*	while(1) {
		write(fd, pix_blue, 4);
	}	*/

	close(fd);
}
