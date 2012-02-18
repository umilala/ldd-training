/*
 * Filename: test.c
 */


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "cdata.h"

int main(void) {
	int fd, fd3;
	char buffer[256];
	int num =3;

	char pix[4] = {0x00, 0xff, 0x00, 0x00};


	fd = open("/dev/cdata", O_RDWR);
	printf("test: fd %d\n", fd);

	ioctl(fd, CDATA_CLEAR);


	for (num=0; num<(50*240); num++) {		/*   */
		write(fd, &pix, 4);
	}
	/*sleep(5);

	ioctl(fd, CDATA_RED, &num);*/

	close(fd);
/*	fd3 = open("/dev/cdata3", O_RDWR);
	printf("test: fd3 %d\n", fd3);
	write(fd3, buffer, 0);
	close(fd3);*/
}
