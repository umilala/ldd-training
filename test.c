/*
 * Filename: test.c
 */


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>


//#define MSG(m...) printk(KERN_INFO "TEST: " m "\n")

int main(void) {
	int fd;
//	MSG("run mytest (start)....");
	fd = open("/dev/cdata", O_RDWR);
//	MSG("run mytest (end)....");
	close(fd);
}
