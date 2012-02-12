#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/vmalloc.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/miscdevice.h>
#include <linux/input.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#define DEV_MAJOR	121
#define DEV_NAME	"cdata"

#define MSG(m...) printk(KERN_INFO "CDATA: " m "\n");


/* TEST_MODE 
 * 0: LOOP_ONLY
 * 1: SCHEDULE
 * 2: SCHEDULE_UNINTERRUPTIBLE
 * 3:
 */

#define TEST_MODE o


static int cdata_open(struct inode *inode, struct file *filp)
{
	int i;
	/*for (i=0; i<500000; i++) {
		//do nothing
		//
	}*/

	if (MINOR(inode->i_rdev)!=0) {
		MSG("error minor number");
		return -ENODEV;
	}
	MSG("cdata_open");
	return 0;
}

static int cdata_close(struct inode *inode, struct file *filp)
{
	MSG("cdata_close");
	return 0;
}

static int cdata_read(struct file *filp, char *buffer, size_t size, loff_t *offset)
{
	MSG("cdata_read");
	return 0;
}

static int cdata_write(struct file *filp, const char *buffer, size_t size, loff_t *offset)
{
	MSG("cdata_write");
	return 0;
}


static struct file_operations cdata_fops = {
	owner:		THIS_MODULE,	
	open:		cdata_open,
	release:	cdata_close,
	write:		cdata_write,
	read:		cdata_read,
};

int cdata_init_module(void)
{
	if (register_chrdev(DEV_MAJOR, DEV_NAME, &cdata_fops) < 0) {
		//error
		return -1;
	}
	return 0;
}

void cdata_cleanup_module(void)
{
	unregister_chrdev(DEV_MAJOR, DEV_NAME);
}

module_init(cdata_init_module);
module_exit(cdata_cleanup_module);

MODULE_LICENSE("GPL");
