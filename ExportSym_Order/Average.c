#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include "Header.h"

int a_test=10,b_test=20;
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("yaswanth");

static int __init hello_init(void)
{
	int average;
	printk(KERN_ALERT "\nIn Init Module...The Module parameters is %d,%d\n",a_test,b_test);
	average=add(a_test,b_test)/2;
	printk(KERN_ALERT "\nAverage is %d\n",average);
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_ALERT "GoodBye\n");
}

module_init(hello_init);
module_exit(hello_exit);
module_param(a_test,int,S_IRUGO);
module_param(b_test,int,S_IRUGO);
