#include <linux/module.h>
#include <linux/sched.h>
#include <linux/jiffies.h>
#include <linux/timer.h>

static struct timer_list ktimer;//kernal timer
void timer_callback(struct timer_list *t)
{
	int ret;
	printk("In timer callback\n");
	ret =mod_timer(&ktimer,jiffies+msecs_to_jiffies(3000));
}
static int __init timer_init(void)
{
	int retval;
	printk("current process is %s \n",current->comm);
	timer_setup(&ktimer,timer_callback,0);
	printk("Setup timer to fire 100ms (%ld) \n",jiffies);
	retval=mod_timer(&ktimer,jiffies+msecs_to_jiffies(3000));
	if(retval)
		printk("Timer failed \n");
	return 0;
}

static void __exit timer_exit(void)
{
	int retval;
	retval = del_timer_sync(&ktimer);
	if(retval)
		printk("The timer in use\n");
}

module_init(timer_init);
module_exit(timer_exit);


