#include <linux/init.h>	
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>

#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/kfifo.h>

#define MAX_BUF_SIZE 1024

dev_t pseudo_dev_id;
int ndevices=1;

char *pbuffer;				//preferably in pvt object
struct kfifo kfifo;			//per device
struct cdev cdev; 			//per device
struct device* pdev;			//per device

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
int pseudo_read(struct file* file,char __user* ubuffer,size_t ucount,loff_t *off)
{
	printk("pseudo::entered read method\n");
	int klen,rcount,ret;
	klen=kfifo_len(&k1);
	if(klen==0)
	{
		printk("device is empty\n");
		return 0;
	}
	rcount=ucount;
	if(klen<rcount) rcount=klen;		  //rcount=min(klen,ucount)
	char *tbuffer=kmalloc(rcount,GFP_KERNEL); //char tbuffer[rcount];
	//print kfifo_len, kfifo_avail, ucount
	ret=kfifo_out(&k1,tbuffer,rcount);
	if(ret)
	{
		//TODO
	}
	//print kfifo_len, kfifo_avail, ucount
	ret=copy_to_user(ubuffer,tbuffer,rcount);
	if(ret)
	{
		printk("copy_to_user failed");
		//TODO:kfree(tbuffer)
		//return -EINVAL;
	}
	kfree(tbuffer);
	printk("read method completed\n");
	return rcount;
}
int pseudo_write(struct file* file,const char __user* ubuffer,size_t ucount,loff_t *off)
{
	printk("pseudo::entered write method\n");
	int wcount,ret,kremain;
	kremain=kfifo_avail(&k1);
	if(kremain==0)
	{
		printk("no space left on device\n");
		return -ENOSPC;
	}
	wcount=ucount;
	if(kremain<wcount) wcount=kremain;	//min(ucount,kremain)
	char *tbuffer=kmalloc(wcount,GFP_KERNEL);
	ret=copy_from_user(tbuffer,ubuffer,wcount);
	if(ret)
	{
		//TODO
	}
	ret=kfifo_in(&kfifo,tbuffer,wcount);
	if(ret)
	{
		//TODO
	}
	kfree(tbuffer);
	return wcount;
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
	//per device initialization starts here
	pbuffer=kmalloc(MAX_BUF_SIZE,GFP_KERNEL);
	if(pbuffer==NULL)
	{
		//TODO
	}
	ret=kfifo_init(&k1,pbuffer,MAX_BUF_SIZE);
	/*ret=kfifo_alloc(&k1,MAX_BUF_SIZE,GFP_KERNEL);
	if(ret)
	{

	}*/
	cdev_init(&cdev, &pseudo_fops);
	kobject_set_name(&cdev.kobj,"pdevice%d",i);
	ret=cdev_add(&cdev,pseudo_dev_id+i,1);
	{
		//TODO
	}
	pdev=device_create(pseudo_cls,NULL,pseudo_dev_id+i,NULL,"psample%d",i);
	if(pdev==NULL)
	{
		//TODO
	}
	printk("pseudo char drivered registered,major=%d\n",MAJOR(pseudo_dev_id));
        return 0;
}
static void __exit psample_exit(void)
{
	//per device cleanup
	kfifo_free(&k1);	//dont deallocate pbuffer again using kfree
	cdev_del(&c1);
	device_destroy(pseudo_cls,pseudo_dev_id+0);
	//common cleanup
 	unregister_chrdev_region(pseudo_dev_id,ndevices);
	class_destroy(pseudo_cls);
	printk("device unregistered\n");
}
module_init(psample_init); 
module_exit(psample_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rajesh Sola");
MODULE_DESCRIPTION("A Pseudo Char Driver Module");


