#include <linux/module.h>
#include <linux/sched.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/interrupt.h>

void tasklet_fun(unsigned long data)
{
	printk("Called from tasklet function is %lu\n",data);
}
struct tasklet_struct demo_tasklet;

static int __init task_init(void)
{
	tasklet_init(&demo_tasklet,tasklet_fun,1);
	tasklet_disable(&demo_tasklet);
	tasklet_schedule(&demo_tasklet);
	return 0;
}

static void __exit task_exit(void)
{
	tasklet_enable(&demo_tasklet);
	tasklet_kill(&demo_tasklet);
}

module_init(task_init);
module_exit(task_exit);


