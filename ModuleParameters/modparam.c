#include <linux/module.h>

MODULE_LICENSE("GPL");

static char* cvar="I am Yaswanth";
static int ivar=34;

module_param(cvar,charp,S_IRUGO);
module_param(ivar,int,S_IRUGO);

static int __init param_init(void)
{
	printk("The content in cvar is %s\n",cvar);
	printk("The content in ivar is %d\n",ivar);
	return 0;
}

static void __exit param_exit(void)
{
	printk("exiting from module\n");
}

module_init(param_init);
module_exit(param_exit);


