#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/circ_buf.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/wait.h>

#define SIZE 32

MODULE_LICENSE("GPL");

static dev_t devnum; //device number

static struct cdev _cdev; //chardev structure

struct circ_buf cbuf;  //circular buffer

static wait_queue_head_t _wqr;//wait queue for read
static wait_queue_head_t _wqw;//wait queue for read

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
	int ret,i,mini;

	wait_event_interruptible(_wqr,CIRC_CNT(cbuf.head,cbuf.tail,SIZE)>0);

	mini = min (cnt,(size_t)CIRC_CNT(cbuf.head,cbuf.tail,SIZE));

	printk("size from user %d\n",(int)cnt);
	for(i=0;i<mini;i++)
	{
		ret = copy_to_user(ubuff+i,cbuf.buf+cbuf.tail,1);
		if(ret)
		{
			printk("error copied to user \n");
			return -EFAULT;
		}
		printk("copied %c to user\n",ubuff[i]);
		cbuf.tail=(cbuf.tail+1) & (SIZE-1);
	}
	wake_up(&_wqw);
	return i;
}

static ssize_t simple_write(struct file *filep,const char __user * ubuff,size_t cnt,loff_t *offset)
{
	int i,ret,mini;
	wait_event_interruptible(_wqw,CIRC_CNT(cbuf.head,cbuf.tail,SIZE)<(int)cnt);
	mini = max (cnt,(size_t)CIRC_CNT(cbuf.head,cbuf.tail,SIZE));

        printk("userspace size %d\n",(int)cnt);
	for(i=0;i<mini;i++)
	{
		ret=copy_from_user(cbuf.buf+cbuf.head,ubuff+i,1);
		if(ret)
		{
			printk("error copying\n");
			return -EFAULT;
		}
		printk("copied from User %c \n",cbuf.buf[cbuf.head]);
		cbuf.head=(cbuf.head+1) & (SIZE-1);
	}
	wake_up(&_wqr);
        return i;
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
	devnum=MKDEV(42,0);
	ret=register_chrdev_region(devnum,1,"first_dev");
	if(ret)
	{
		printk("kernel rejects to give device number\n");
		return ret;
	}
	cdev_init(&_cdev,&fops);//cdev structure and file operations
	
	cbuf.buf=kmalloc(SIZE,GFP_KERNEL); //memory allocation
	if(!cbuf.buf)
	{
		printk("memory not allocated\n");
		kfree(cbuf.buf);
		unregister_chrdev_region(devnum,1);
		return -1;
	}

	init_waitqueue_head (&_wqr);
	init_waitqueue_head (&_wqw);
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
	kfree(cbuf.buf);
	unregister_chrdev_region(devnum,1);
	printk("Exiting from module\n");
}        

module_init(chardev_init);
module_exit(chardev_exit);
