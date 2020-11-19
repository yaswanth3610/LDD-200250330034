#include <linux/module.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
int Type=1;
static int a,b;
module_param(a,int,S_IRUGO);
module_param(b,int,S_IRUGO);

static int add(int ,int );

static int __init esyms_init(void)
{
	printk("Sum of two numbers a and b =%d\n",add(a,b));
	return 0;
}

static int add(int a,int b)
{
	return a+b;
}

EXPORT_SYMBOL_GPL(add);

static void __exit esyms_exit(void)
{
	printk("exiting from module\n");
}

module_init(esyms_init);
module_exit(esyms_exit);
module_param(Type,int,S_IRUGO);
