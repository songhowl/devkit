#include <stdio.h>

/******************************************************************************
 *
 * Just print once
 *
 ******************************************************************************/
void print_once()
{ 
	WARN_ONCE(!irqs_disabled(),
			"Interrupts enabled after %pF\n", ops->resume);
}

/******************************************************************************
 *
 * calculate the time
 *
 ******************************************************************************/
void print_time(void)
{
	printk(KERN_ERR "calling  %pF @ %i\n", fn, task_pid_nr(current));
	calltime = ktime_get();
	ret = fn();
	rettime = ktime_get();
	delta = ktime_sub(rettime, calltime);
	duration = (unsigned long long) ktime_to_ns(delta) >> 10;
	printk(KERN_ERR "initcall %pF returned %d after %lld usecs\n", fn,
			ret, duration)
}




/******************************************************************************
 *
 * calculate the time
 *
 ******************************************************************************/
void print_name(void)
{
//	• 设备名：
	dev_name(dev)
	
//	• 总线名:
	bus->name
	
//	• 驱动名：
	drv->name
	
//	• 当前cpuid
	current_thread_info()->cpu
}




void main()
{
}
