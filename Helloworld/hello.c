#include <linux/module.h>

MODULE_LICENSE("GPL");

static int __init hello_init(void //initalization)
{
	printk("HI,i am in init to kernal module\n");
	return 0;
}

static void __exit hello_exit(void)//exiting
{
	printk("BYE,i am exiting from kernal module\n");
}

module_init(hello_init);
module_exit(hello_exit);
