#include<linux/init.h>
#include<linux/module.h>
int __init hello_init(void)	//init_module
{
	pid_t pid;
	pid_t ppid;
	printk("Hello module::init--welcome\n");
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
