#include <linux/module.h>
#include "Hello.h"

MODULE_LICENSE("GPL");

static int a,b;
module_param(a,int,S_IRUGO);
module_param(b,int,S_IRUGO);

static int __init avg_init(void)
{
	int average;
	average = add(a,b)/2;
	printk("Average = %d\n",average);
	return 0;
}

static void __exit avg_exit(void)
{
	printk("exinting from module\n");
}

module_init(avg_init);
module_exit(avg_exit);

