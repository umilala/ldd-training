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

#include "cdata.h"

#define DEV_MAJOR	121
#define DEV_NAME	"cdata"

#define BUFFER_SIZE	50*240*4

#define MSG(m...) printk(KERN_INFO "CDATA: " m "\n");

struct cdata_t {
	unsigned long *fb;
	char* pixels;
	int index;

};

static int cdata_open(struct inode *inode, struct file *filp)
{
	int minor;
	struct cdata_t* cdata;

	printk(KERN_INFO "CDATA: a in open\n");
	minor = MINOR(inode->i_rdev);
	printk(KERN_INFO "CDATA: minor = %d\n", minor);

	cdata = (struct cdata_t*)kmalloc(sizeof(struct cdata_t), GFP_KERNEL);
//	 cdata= kmalloc(sizeof(struct cdata_t), GFP_KERNEL);

	cdata->fb= ioremap(0x33f00000, 320*240*4);
	cdata->pixels = 0;
	cdata->index = 0;

	filp->private_data =  (void*)cdata;

	return 0;
}

static int cdata_close(struct inode *inode, struct file *filp)
{

	struct cdata_t* cdata;
	MSG("cdata_close");

	cdata = (struct cdata_t*)filp->private_data;
	if (cdata) {
		flush_pixels(cdata);

		if (cdata->pixels)
			kfree(cdata->pixels);
		kfree(cdata);
	}


	return 0;
}

static int cdata_read(struct file *filp, char *buffer, size_t size, loff_t *offset)
{
	MSG("cdata_read");
	return 0;
}

static int cdata_ioctl(struct inode* inode, struct file* filp, unsigned int cmd, unsigned long arg) {
	unsigned long *fb;
	int i;
	int n;

	struct cdata_t* cdata = (struct cdata_t*)filp->private_data;

	MSG("cdata_ioctl ...");

	switch(cmd) {
		case CDATA_CLEAR:

			printk(KERN_INFO "CDATA CDATA_CLEAR\n");
			//Lock
			fb= cdata->fb;
			//UnLock
			for (i=0; i< 320*240; i++) {
				writel(0x00ffffff, fb++);
			}	
		break;
		case CDATA_RED:
			n = *((int*)arg);
			printk(KERN_INFO "CDATA CDATA_RED: %d\n", n);
			//Lock
			fb= cdata->fb;
			//UnLock
			for (i=0; i< 320*240; i++) {
				writel(0x00ff0000, fb++);
			}	
		break;
		case CDATA_GREEN:
			printk(KERN_INFO "CDATA CDATA_CLEAR\n");
			//Lock
			fb= cdata->fb;
			//UnLock
			for (i=0; i< 320*240; i++) {
				writel(0x0000ff00, fb++);
			}	
		default:
			return -1;
	}
	return 0;
}

static void flush_pixels(void* priv) {
	unsigned char* pixels;
	unsigned char* fb;
	int i;
	struct cdata_t* cdata = (struct cdata_t *)priv;

	MSG("flush_pixels");
	//Lock
	fb = (unsigned char*)cdata->fb;
	//UnLock
	pixels = cdata->pixels;

	if (pixels==0) {
		MSG("flush_pixels ERROR, NULL pointer");
		return;
	}

	for (i=0; i< /*320*240*/cdata->index; i++) {
		writeb(pixels[i], fb++);
	}	

	//kfree(data);

}

static int cdata_write(struct file *filp, const char *buffer, size_t size, loff_t *offset)
{
	int i;
	struct cdata_t* cdata = (struct cdata_t*)filp->private_data;
	unsigned long* fb;
	char* pixels = cdata->pixels;
	int copy_size = size;

	MSG("cdata_write(BEGIN)");
	if (!pixels) {
		printk(KERN_INFO "cdata_write, allocate buf\n");
		pixels= (char*)kmalloc(sizeof(char)*BUFFER_SIZE, GFP_KERNEL);
		cdata->pixels = pixels;
		cdata->index = 0;
	} else {
		pixels+=cdata->index;
		if ((cdata->index + size) > BUFFER_SIZE) {
			printk(KERN_INFO "cdata_write, adjust copy_size: %d\n", copy_size);
			copy_size = BUFFER_SIZE - cdata->index;
		}
	}

	if(!pixels) {
		printk(KERN_INFO "pixels is NULL\n");
		return 0;
	}

	if (copy_size){
		copy_from_user(pixels, buffer, copy_size);
		cdata->index += size;
		printk(KERN_INFO "cdata_write, still waitting buf, index(%d)\n", cdata->index);
	}

	if (cdata->index >= BUFFER_SIZE) {
		flush_pixels(cdata);
		cdata->index = 0;
	}


	/*for (i=0; i<50000000; i++)while(1) {
		current->state = TASK_INTERRUPTIBLE;
		//current->state = TASK_UNINTERRUPTIBLE:
		schedule();
	}*/






	MSG("cdata_write(END)");
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
	/*fd = ioremap(0x33f00000, 320*240*4);
	for (i=0; i< 320*240; i++) {
		writel(0x00ff0000, fd++);	//
	}*/


	if (register_chrdev(121, "cdata", &cdata_fops) < 0) {
		printk(KERN_INFO "CDATA: can't register driver\n");
		return -1;
	}

	printk(KERN_INFO "CDATA, cdata_init_module\n");

	return 0;
}

static void cdata_cleanup_module(void)
{
	unregister_chrdev(DEV_MAJOR, DEV_NAME);
}

module_init(cdata_init_module);
module_exit(cdata_cleanup_module);

MODULE_LICENSE("GPL");
