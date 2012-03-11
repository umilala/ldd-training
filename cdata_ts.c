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
//#include "cdata_ts.h"

static void cdata_bh(unsigned long);
DECLARE_TASKLET(my_tasklet, cdata_bh, 0);


struct cdata_ts {
	struct input_dev ts_input;
	int x;
	int y;	
};




static void cdata_ts_handler(int irqflags, void* priv, struct pt_regs* reg) {
	struct cdata_ts *cdata = (struct cdata_ts *)priv;

	printk(KERN_INFO "cdata_ts_handler\n");

	cdata->x = 100;
	cdata->y = 100;

	my_tasklet.data = (unsigned long)cdata;

	tasklet_schedule(&my_tasklet);
	/* FIXME: read (x, y) from ADC */
	//x = 100;
	//y = 100;
}

static void cdata_bh(unsigned long priv) {
	struct cdata_ts *cdata = (struct cdata_ts *)priv;
	struct input_dev *dev = &cdata->ts_input;

	printk(KERN_INFO "cdata_ts: down...\n");

	input_report_abs(dev, ABS_X, cdata->x);
	input_report_abs(dev, ABS_Y, cdata->y);
}

int ts_input_open(struct input_dev *dev) {



	printk(KERN_INFO "ts_input_open\n");
	return 0;
}

void ts_input_close(struct input_dev* dev) {
	printk(KERN_INFO "ts_input_close\n");
}

static int cdata_ts_open(struct inode *inode, struct file *filp)
{
	struct cdata_ts *cdata;
	cdata = kmalloc(sizeof(struct cdata_ts), GFP_KERNEL);
#if 0	
	u32 reg;
	reg = GPGCON;
	reg |= 0xff000000;
	GPGCON = reg;
	printk(KERN_INFO "GPGCON: %08x\n", GPGCON);
#else
	set_gpio_ctrl(GPIO_YPON);
	set_gpio_ctrl(GPIO_YMON);
	set_gpio_ctrl(GPIO_XPON);
	set_gpio_ctrl(GPIO_XMON);
#endif


	ADCTSC = DOWN_INT | XP_PULL_UP_EN | \
		XP_AIN | XM_HIZ | YP_AIN | YM_GND | \
		XP_PST(WAIT_INT_MODE);

	/* Request touch panel IRQ */
	if (request_irq(IRQ_TC, cdata_ts_handler, 0, "cdata-ts", (void *)cdata)) {
		printk(KERN_ALERT "cdata: request irq failed.\n");
		return -1;
	}

	 /** handling input device ***/
	cdata->ts_input.name = "ts-input";
	cdata->ts_input.open = ts_input_open;
	cdata->ts_input.close = ts_input_close;
	cdata->ts_input.absbit[0] = BIT(ABS_X) | BIT(ABS_Y);
	cdata->ts_input.evbit[0] = BIT(KEY_ENTER) | BIT(KEY_F5);

	cdata->x =0;
	cdata->y =0;

	input_register_device(&cdata->ts_input);

	filp->private_data = (void *)cdata;

	printk(KERN_INFO "cdata_ts_open\n");
	return 0;
}

static ssize_t cdata_ts_read(struct file *filp, char *buf, size_t size, loff_t *off)
{
	return 0;
}


static ssize_t cdata_ts_write(struct file *filp, const char *buf, size_t size, loff_t *off)
{
	return 0;
}

static int cdata_ts_close(struct inode *inode, struct file *filp)
{
	struct cdata_ts *cdata = (struct cdata_ts *)filp->private_data;
	kfree(cdata);
	return 0;
}

static int cdata_ts_ioctl(struct inode *inode, struct file *filp,
unsigned int cmd, unsigned long arg)
{
	return -ENOTTY;
}

static struct file_operations cdata_ts_fops = {
	owner: THIS_MODULE,
	open: cdata_ts_open,
	release: cdata_ts_close,
	read: cdata_ts_read,
	write: cdata_ts_write,
	ioctl: cdata_ts_ioctl,
};


static struct miscdevice cdata_ts_misc = {
	minor: 50,
	name: "cdata-ts",
	fops:	&cdata_ts_fops,
	
};



int cdata_ts_init_module(void)
{

	if (misc_register(&cdata_ts_misc) < 0) {
		printk(KERN_INFO "CDATA-TS: can't register driver\n");
		return -1;
	}
	printk(KERN_INFO "CDATA-TS: cdata_ts_init_module\n");
	return 0;
}

void cdata_ts_cleanup_module(void)
{
	misc_deregister(&cdata_ts_misc);

}

module_init(cdata_ts_init_module);
module_exit(cdata_ts_cleanup_module);

MODULE_LICENSE("GPL");





