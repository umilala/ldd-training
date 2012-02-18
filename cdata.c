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


static int cdata_open(struct inode *inode, struct file *filp)
{
//	int i;
	int minor;

	printk(KERN_INFO "CDATA: a in open\n");

	minor = MINOR(inode->i_rdev);
	printk(KERN_INFO "CDATA: minor = %d\n", minor);

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
	int i;
	MSG("cdata_write(BEGIN)");
	/*for (i=0; i<50000000; i++)*/while(1) {
		current->state = TASK_INTERRUPTIBLE;
		//current->state = TASK_UNINTERRUPTIBLE:
		schedule();
	}

	MSG("cdata_write(END)");
	return 0;
}

static int cdata_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	MSG("cdata_ioctl");
	return 0;
}


static int cdata_flush(struct file * filp) {
	MSG("cdata_flush");
	return 0;
}

static struct file_operations cdata_fops = {
	owner: THIS_MODULE,
	open: cdata_open,
	release: cdata_close,
	read: cdata_read,
	write: cdata_write,
	ioctl: cdata_ioctl,
	flush: cdata_flush,
};

static int cdata_init_module(void)
{

	unsigned long *fd;
	int i;
	fd = ioremap(0x33f00000, 320*240*4);
	for (i=0; i< 320*240; i++) {
		writel(0x00ff0000, fd++);	//
	}


	if (register_chrdev(121, "cdata", &cdata_fops) < 0) {
		printk(KERN_INFO "CDATA: can't register driver\n");
		return -1;
	}

	printk(KERN_INFO "CDATA, cdata_init_module");

	return 0;
}

static void cdata_cleanup_module(void)
{
	unregister_chrdev(DEV_MAJOR, DEV_NAME);
}

module_init(cdata_init_module);
module_exit(cdata_cleanup_module);

MODULE_LICENSE("GPL");
