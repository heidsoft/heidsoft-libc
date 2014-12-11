#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");

static int hello_init(void)
{ 
	printk(KERN_ALERT "hello_init");
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "hello_exit");
}

//注册
module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("heidsoft");
MODULE_DESCRIPTION("example.hello.module");
MODULE_ALIAS("example");