#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");

static int add(int ,int );
static int __init add_init(void)
{
	printk("\ninside Addition module\n");
	return 0;
}

static int add(int a,int b)
{
	return (a+b);
}

EXPORT_SYMBOL_GPL(add);

static void __exit add_exit(void)
{
	printk("Bye\n");
}

module_init(add_init);
module_exit(add_exit);
