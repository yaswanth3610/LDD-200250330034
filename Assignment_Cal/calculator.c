#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");

static dev_t devnum; //device number

static struct cdev _cdev; //chardev structure

int simple_open (struct inode *inodep,struct file *filep)
{
	printk("Simple open method\n");
	return 0;
}

static int simple_close(struct inode *inodep,struct file *filep)
{
	printk("Simple close method\n");
        return 0;
}

static ssize_t simple_read(struct file *filep,char __user *ubuff,size_t cnt,loff_t *offset)
{
	printk("simple read method\n");
	return 0;
}

static ssize_t simple_write(struct file *filep,const char __user *ubuff,size_t cnt,loff_t *offset)
{
        printk("simple write method\n");
        return 0;
}

struct file_operations fops={
        .open    = simple_open,
        .release = simple_close,
	.read = simple_read,
	.write = simple_write,
};

static int __init chardev_init(void)
{
	int ret;
	printk("Inside init Block\n");
	devnum_add=MKDEV(42,0);
	devnum_sub=MKDEV(42,1);
	devnum_mul=MKDEV(42,2);
	devnum_div=MKDEV(42,3);

	ret=register_chrdev_region(devnum,1,"first_dev");
	if(ret)
	{
		printk("kernel rejects to give device number\n");
		return ret;
	}
	cdev_init(&_cdev,&fops);//cdev structure and file operations

	ret = cdev_add(&_cdev,devnum,1);
	if(ret)
	{
		printk("unable to add cdev\n");
		unregister_chrdev_region(devnum,1);
		return ret;
	}
	return 0;
}

static void __exit chardev_exit(void)
{
	cdev_del(&_cdev);
	unregister_chrdev_region(devnum,1);
	printk("Exiting from module\n");
}        

module_init(chardev_init);
module_exit(chardev_exit);
