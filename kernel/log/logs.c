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



/******************************************************************************
 *
 *执行dump_stack就可打印stack信息.
 *
 ******************************************************************************/
asmlinkage void __div0(void)
{
	printk("Division by zero in kernel.\n");
	dump_stack(); // dump current task stack
}


/******************************************************************************
 *print_string代替printk朝当前终端打消息
 *"/n" = "/012"  n 是特殊转义字符，代表"Next Line" 或Line Feed 。 /012是8进制数字表示方式
 *012 = 0x0A = 10 
 * /015 = 0x0D = 13 是回车 Carrige Return 的意思，对应的特殊转义字符是 '/r'。
 *
 *至于"/015/012"代表"/r/n" 就是先回车、再换行。Windows是用这个组合作为换行符。而/n 是Unix用来表示的换行。
 *
 *换行好理解，回车的意思实际上是指把当前坐标转到行首。是用来控制打印机针头的。。。 
 ******************************************************************************/
 static void print_string(char *str)
{
	        struct tty_struct *my_tty;
	        my_tty = current->signal->tty;
	        if (my_tty != NULL) {
		                ((my_tty->driver->ops)->write) (my_tty,str, strlen(str));
		                ((my_tty->driver->ops)->write) (my_tty,"/015/012", 2);
		        }
}
 
 


/******************************************************************************
 *
 *print device path
 *
 ******************************************************************************/
void print_devicepath(void)
{
path = kobject_get_path(&dev->dev.kobj, GFP_KERNEL);  
printk(KERN_INFO "input: %s as %s\n",  
dev->name ? dev->name : "Unspecified device", path ? path:"N/A");  
kfree(path); 

}



/******************************************************************************
 *
 *获得当前的CPUID
 *There was a comment in current.h saying
 *that current_thread was obsolete, so this patch turns all instances of
 *current_thread into current_thread_info()
 *
 ******************************************************************************/
void print_current_cpu_id(void)
{
	current_thread_info()->cpu
}

static inline struct thread_info *current_thread_info(void)
{
        register unsigned long sp asm ("sp");
        return (struct thread_info *)(sp & ~(THREAD_SIZE - 1));
}

void main()
{
}
