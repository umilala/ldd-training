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
	int i, j;
	pid_t pid;
	char pix_green[4] = {0x00, 0xff, 0x00, 0xff};
	char pix_blue[4] = {0xff, 0x00, 0x00, 0xff};

	char *fb, *temp;	//framebuffer pointer

	i = 10000;

	//pid = fork();

	fd = open("/dev/cdata", O_RDWR);

	fb = (char*)mmap(0, 320*240*4, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	temp = fb;
	for (i=0; i< 320*240; i++) {
		for (j=0; j<4; j++) {
			*temp++ = pix_blue[j];
		}
		j=0;
	}

	sleep(10);//for checking the mmap, cat /proc/xxx/maps

	temp = fb;
	for (i=0; i< 320*240; i++) {
		for (j=0; j<4; j++) {
			*temp++ = pix_green[j];
		}
		j=0;
	}



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
