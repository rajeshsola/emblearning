/** Changelog:- using per device attributes in the form of private object **/
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

typedef struct priv_obj1{
	void *pbuffer;				
	struct kfifo kfifo;			
	struct cdev cdev; 			
	struct device* pnode;			
}C_DEV;
C_DEV* my_dev;

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
	C_DEV* pobj=my_dev;
	printk("entered pseudo read method\n");
	int klen,rcount,ret,nbytes;
	klen=kfifo_len(&pobj->kfifo);
	if(klen==0)
	{
		printk("device is empty\n");
		return 0;
	}
	rcount=ucount;
	if(klen<rcount) rcount=klen;	//rcount=min(klen,ucount)
	if(access_ok(VERIFY_WRITE,(void __user*)ubuffer,(unsigned long)rcount))
	{
		nbytes=kfifo_out(&pobj->kfifo,ubuffer,rcount);
	}
	else
		return -EFAULT;
	printk("read method completed\n");
	return rcount;
}
int pseudo_write(struct file* file,const char __user* ubuffer,size_t ucount,loff_t *off)
{
	C_DEV* pobj=my_dev;
	printk("entered pseudo write method\n");
	int wcount,ret,kremain;
	kremain=kfifo_avail(&pobj->kfifo);
	if(kremain==0)
	{
		printk("no space left on device\n");
		return -ENOSPC;
	}
	wcount=ucount;
	if(kremain<wcount) wcount=kremain;	//min(ucount,kremain)
	if(access_ok(VERIFY_READ,(void __user*)ubuffer,(unsigned long)wcount))
	{
		kfifo_in(&pobj->kfifo,ubuffer,wcount);
	}
	else
		return -EFAULT;
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
	my_dev=kmalloc(sizeof(C_DEV),GFP_KERNEL);
	if(my_dev==NULL)
	{
		//TODO
	}
	/*my_dev->pbuffer=kmalloc(MAX_BUF_SIZE,GFP_KERNEL);
	if(my_dev->pbuffer==NULL)
	{
		//TODO
	}
	ret=kfifo_init(&my_dev->kfifo,my_dev->pbuffer,MAX_BUF_SIZE);
	if(ret)
	{
		//TODO
	}*/
	ret=kfifo_alloc(&my_dev->kfifo,MAX_BUF_SIZE,GFP_KERNEL);
	if(ret)
	{

	}
	cdev_init(&my_dev->cdev, &pseudo_fops);
	kobject_set_name(&my_dev->cdev.kobj,"pdevice%d",i);
	ret=cdev_add(&my_dev->cdev,pseudo_dev_id+i,1);
	if(ret)
	{
		//TODO
	}	
	my_dev->pnode=device_create(pseudo_cls,NULL,pseudo_dev_id+i,NULL,"psample%d",i);
	if(my_dev->pnode==NULL)
	{
		//TODO
	}
	printk("pseudo char drivered registered,major=%d\n",MAJOR(pseudo_dev_id));
        return 0;
}
static void __exit psample_exit(void)
{
	//per device cleanup
	C_DEV* pdev=my_dev;
	kfifo_free(&pdev->kfifo);	//dont deallocate pbuffer again using kfree
	cdev_del(&pdev->cdev);
	device_destroy(pseudo_cls,pseudo_dev_id+0);
	kfree(pdev);
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


