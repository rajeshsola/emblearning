#include<linux/init.h>
#include<linux/module.h>
int __init hello_init(void)	//init_module
{
	printk("Hello module::init--welcome\n");
	printk(KERN_EMERG   "Hello module::emergency\n"); //<0>
	printk(KERN_ALERT   "Hello module::alert\n"); //<1>
	printk(KERN_CRIT    "Hello module::critical\n"); //<2>
	printk(KERN_ERR     "Hello module::error\n"); //<3>
	printk(KERN_WARNING "Hello module::warning\n");//<4>
	printk(KERN_NOTICE  "Hello module::notice\n");//<5>
	printk(KERN_INFO    "Hello module::info\n");//<6>
	printk(KERN_DEBUG   "Hello module::debug\n");//<7>
	return 0;
}
void __exit hello_exit(void) 	//cleanup_module
{
	printk("Hello module::exit--bye\n");
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_AUTHOR("Your Name");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A Simple Module");
