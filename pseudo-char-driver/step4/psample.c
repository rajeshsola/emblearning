#include <linux/init.h>	
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/cdev.h>

#define MAX_BUF_SIZE 1024

dev_t pseudo_dev_id;

struct cdev cdev; 			//per device
struct device* pdev;			//per device
char* pbuffer;

int pseudo_open(struct inode* inode,struct file* file)
{
	printk("pseudo open...\n");
	return 0;
}
int pseudo_close(struct inode* inode,struct file* file)
{
	printk("pseudo close...\n");
	return 0;
}
int pseudo_read(struct file* file,char __user* ubuf,size_t ucount,loff_t *off)
{
	printk("pseudo read..\n");
	return 0;
}
int pseudo_write(struct file* file,const char __user* ubuf,size_t ucount,loff_t *off)
{
	printk("pseudo write..\n");
	return -ENOSPC;
}
struct file_operations pseudo_fops =
{
	.open 		= pseudo_open,
	.release 	= pseudo_close,
	.read		= pseudo_read,
	.write		= pseudo_write
};
struct class *pseudo_cls;
static int __init psample_init(void)	
{
	int ret,i=0;
	pseudo_cls=class_create(THIS_MODULE,"pseudo_class");
	ret=alloc_chrdev_region(&pseudo_dev_id,0,ndevices,"pseudo_char_driver");
	if(ret)
	{
		printk("error in alloc....\n");
		return -EINVAL; //suitable err value
	}
	pbuffer=kmalloc(MAX_BUF_SIZE, GFP_KERNEL);
	if(pbuffer==NULL)
	{
		//TODO
	}
	cdev_init(&cdev, &pseudo_fops);
	kobject_set_name(&cdev.kobj,"pdevice%d",i);
	ret=cdev_add(&cdev,pseudo_dev_id+i,1);
	if(ret)
	{
		//TODO
	}
	ret=device_create(pseudo_cls,NULL,pseudo_dev_id+i,NULL,"psample%d",i);
	if(pdev==NULL)
	{
		//TODO
	}		
	printk("pseudo::char drivered registered,major=%d\n",MAJOR(pseudo_dev_id));
        return 0;
}
static void __exit psample_exit(void)
{
	kfree(pbuffer);
	cdev_del(&cdev);
	device_destroy(pseudo_cls,pseudo_dev_id+0);

 	unregister_chrdev_region(pseudo_dev_id,ndevices);
	class_destroy(pseudo_cls);
	printk("device unregistered\n");
}
module_init(psample_init); 
module_exit(psample_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your name");
MODULE_DESCRIPTION("A Pseudo Char Driver Module");


