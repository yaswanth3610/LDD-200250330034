#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/circ_buf.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/wait.h>
#include "myioctl.h"
#define SIZE 32
#define MAJORN 42
#define NMINORS 4

MODULE_LICENSE("GPL");

static dev_t devnum; //device number
struct mydevice
{
	struct cdev _cdev; //chardev structure
	struct circ_buf cbuf;  //circular buffer
	wait_queue_head_t _wqr;//wait queue for read
	wait_queue_head_t _wqw;//wait queue for read
}_devs[NMINORS];

int simple_open (struct inode *inodep,struct file *filep)
{
	struct mydevice *_devp=container_of(inodep->i_cdev,struct mydevice,_cdev);
	filep->private_data=_devp;
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
	struct mydevice *_devp=filep->private_data;

	wait_event_interruptible(_devp->_wqr,CIRC_CNT(_devp->cbuf.head,_devp->cbuf.tail,SIZE)>0);

	mini = min (cnt,(size_t)CIRC_CNT(_devp->cbuf.head,_devp->cbuf.tail,SIZE));

	printk("size from user %d\n",(int)cnt);
	for(i=0;i<mini;i++)
	{
		ret = copy_to_user(ubuff+i,_devp->cbuf.buf+_devp->cbuf.tail,1);
		if(ret)
		{
			printk("error copied to user \n");
			return -EFAULT;
		}
		printk("copied %c to user\n",_devp->cbuf.buf[_devp->cbuf.tail]);
		_devp->cbuf.tail=(_devp->cbuf.tail+1) & (SIZE-1);
	}
	wake_up(&_devp->_wqw);
	return i;
}

static ssize_t simple_write(struct file *filep,const char __user * ubuff,size_t cnt,loff_t *offset)
{
	int i,ret,mini;
	struct mydevice *_devp=filep->private_data;
	wait_event_interruptible(_devp->_wqw,CIRC_CNT(_devp->cbuf.head,_devp->cbuf.tail,SIZE)<(int)cnt);
	mini = max (cnt,(size_t)CIRC_CNT(_devp->cbuf.head,_devp->cbuf.tail,SIZE));

        printk("userspace size %d\n",(int)cnt);
	for(i=0;i<mini;i++)
	{
		ret=copy_from_user(_devp->cbuf.buf+_devp->cbuf.head,ubuff+i,1);
		if(ret)
		{
			printk("error copying\n");
			return -EFAULT;
		}
		printk("copied from User %c \n",_devp->cbuf.buf[_devp->cbuf.head]);
		_devp->cbuf.head=(_devp->cbuf.head+1) & (SIZE-1);
	}
	wake_up(&_devp->_wqr);
        return i;
}
long simple_ioctl(struct file *filep,unsigned int cmd,unsigned long arg)
{
	switch(cmd)
	{
		case CMD_1:
			printk("This is ioctl CMD_1\n");
			break;
		case CMD_2:
			printk("This is ioctl CMD_2\n");
                        break;
		default:
			printk("not supported\n");
			return -ENOTTY;
	}
	return 0;
}
struct file_operations fops={
        .open    = simple_open,
        .release = simple_close,
	.read = simple_read,
	.write = simple_write,
	.unlocked_ioctl = simple_ioctl
};

static int __init chardev_init(void)
{
	int ret,i,j;
	dev_t tempdev;
	devnum=MKDEV(42,0);
	ret=register_chrdev_region(devnum,NMINORS,"first_dev");
	if(ret)
	{
		printk("kernel rejects to give device number\n");
		goto ret_out;
	}
	for(i=0;i<NMINORS;i++)
	{
		cdev_init(&_devs[i]._cdev,&fops);//cdev structure and file operations
		_devs[i].cbuf.buf=kmalloc(SIZE,GFP_KERNEL); //memory allocation
		if(!_devs[i].cbuf.buf)
		{
			printk("memory not allocated\n");
			goto cbuf_out;
			return -1;
		}
		init_waitqueue_head (&_devs[i]._wqr);
		init_waitqueue_head (&_devs[i]._wqw);
	}
	for(j=0;j<NMINORS;j++)
	{
		tempdev=MKDEV(MAJOR(devnum),MINOR(devnum)+j);
		ret = cdev_add(&_devs[j]._cdev,tempdev,1);
		if(ret)
		{
			printk("unable to add cdev\n");
			goto cdev_out;
			return ret;
		}
	}
	printk("Inside init Block\n");	
	return 0;

cdev_out:
        for(--j;j>=0;j--)
                cdev_del(&_devs[j]._cdev);

cbuf_out:
	for(--i;i>=0;i--)
		kfree(_devs[i].cbuf.buf);

ret_out:
	unregister_chrdev_region(devnum,NMINORS);
	return -1;
}

static void __exit chardev_exit(void)
{
	int i;
	for(i=0;i<NMINORS;i++)
	{
		cdev_del(&_devs[i]._cdev);
		kfree(_devs[i].cbuf.buf);
	}
	unregister_chrdev_region(devnum,1);
	printk("Exiting from module\n");
}        

module_init(chardev_init);
module_exit(chardev_exit);
