#include <linux/init.h>	
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>

dev_t pseudo_dev_id;

static int __init psample_init(void)	
{
	int ret;
	ret=alloc_chrdev_region(&pseudo_dev_id,0,ndevices,"pseudo_char_driver");
	if(ret)
	{
		printk("error in alloc....\n");
		return -EINVAL; //TODO:suitable err value
	}
	printk("pseudo::char drivered registered successfully\n");
	printk("pseudo::assigned major number=%d\n",MAJOR(pseudo_dev_id));	//compare this with /proc/devices
        return 0;
}
static void __exit psample_exit(void)
{
 	unregister_chrdev_region(pseudo_dev_id,ndevices);
	printk("device unregistered\n");
}
module_init(psample_init); 
module_exit(psample_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rajesh Sola");
MODULE_DESCRIPTION("A Pseudo Char Driver Module");


