#include <linux/module.h>
#include <linux/sched.h>
#include <linux/jiffies.h>

static int __init jiffi_init(void)
{
	unsigned long f_jiffy;
	f_jiffy =jiffies + HZ*5;
	printk("current vaule is %ld and future vaule is %lu \n",jiffies,f_jiffy);
	while(time_before(jiffies,f_jiffy))
	{
		schedule();
	}
	printk("current vaule is %ld and future vaule is %lu \n",jiffies,f_jiffy);
	return 0;
}

static void __exit jiffi_exit(void)
{
	printk("Exit\n");
}

module_init(jiffi_init);
module_exit(jiffi_exit);


