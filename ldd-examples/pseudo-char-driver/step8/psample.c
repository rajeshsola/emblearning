/** Changelog:- synchronization support **/
/** Caution - Testing is pending */
#include <linux/init.h>	
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>

#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/kfifo.h>
#include<linux/wait.h>

#define MAX_BUF_SIZE 1024

dev_t pseudo_dev_id;
int ndevices=5;
module_param(ndevices,int,S_IRUGO);

typedef struct priv_obj1{
	struct list_head lentry;
	void *pbuffer;				
	struct kfifo kfifo;			
	struct cdev cdev; 			
	struct device* pnode;			
	wait_queue_head_t rd_queue;
	wait_queue_head_t wr_queue;
}C_DEV;
LIST_HEAD(dev_list);

int pseudo_open(struct inode* inode,struct file* file)
{
	printk("pseudo::open...\n");
	struct C_DEV *pobj=container_of(inode->i_cdev,C_DEV,cdev);
	file->private_data=pobj;
	return 0;
}
int pseudo_close(struct inode* inode,struct file* file)
{
	printk("pseudo::close...\n");
	return 0;
}
int pseudo_read(struct file* file,char __user* ubuffer,
		size_t ucount,loff_t *off)
{
	C_DEV* pobj=file->private_data;
	printk("entered pseudo read method\n");
	int klen,rcount,ret,nbytes;
	klen=kfifo_len(&pobj->kfifo);
	if(klen==0) //TODO, block until some data is added
	{
		if(file->f_flags & O_NONBLOCK)
			return -EAGAIN;
		else
			wait_event_interruptible(pdev->rd_queue,(kfifo_len(&pdev->kfifo)>0));
	}
	rcount=ucount;
	if(klen<rcount) rcount=klen;	//rcount=min(klen,ucount)
	if(access_ok(VERIFY_WRITE,(void __user*)ubuffer,(unsigned long)rcount))
	{
		nbytes=kfifo_out(&pobj->kfifo,ubuffer,rcount);
	}
	else
		return -EFAULT;
	wake_up_interruptible(&pdev->wr_queue);
	printk("pseudo::ead method completed\n");
	return rcount;
}
int pseudo_write(struct file* file,const char __user* ubuffer,
		size_t ucount,loff_t *off)
{
	C_DEV* pobj=file->private_data;
	printk("entered pseudo write method\n");
	int wcount,ret,kremain;
	kremain=kfifo_avail(&pobj->kfifo);
	if(kremain==0)	//block until some free space comes
	{
		if(file->f_flags & O_NONBLOCK)
			return -EAGAIN;
		else
			wait_event_interruptible(pdev->wr_queue,(kfifo_free(&pdev->kfifo)>0));
	
	}
	wcount=ucount;
	if(kremain<wcount) wcount=kremain;	//min(ucount,kremain)
	if(access_ok(VERIFY_READ,(void __user*)ubuffer,(unsigned long)wcount))
	{
		kfifo_in(&pobj->kfifo,ubuffer,wcount);
	}
	else
		return -EFAULT;
	wake_up_interruptible(&pdev->rd_queue);
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
	C_DEV *pobj;
	pseudo_cls=class_create(THIS_MODULE,"pseudo_class");
	ret=alloc_chrdev_region(&pseudo_dev_id,0,ndevices,"pseudo_char_driver");
	if(ret)
	{
		printk("error in alloc....\n");
		return -EINVAL; //suitable err value
	}
	//per device initialization starts here
	for(i=0;i<ndevices;i++)
	{
		pobj=kmalloc(sizeof(C_DEV),GFP_KERNEL);
		if(pobj==NULL)
		{
			//TODO
		}
		ret=kfifo_alloc(&pobj->kfifo,MAX_BUF_SIZE,GFP_KERNEL);
		if(ret)
		{
			//TODO
		}
		cdev_init(&pobj->cdev, &pseudo_fops);
		kobject_set_name(&pobj->cdev.kobj,"pdevice%d",i);
		ret=cdev_add(&pobj->cdev,pseudo_dev_id+i,1);
		if(ret)
		{
			//TODO
		}
		pobj->pnode=device_create(pseudo_cls,NULL,pseudo_dev_id+i,NULL,"psample%d",i);
		init_waitqueue_head(&pobj->rd_queue);
		init_waitqueue_head(&pobj->wr_queue);
		list_add_tail(&pobj->lentry,&dev_list);	
	}
	printk("pseudo char drivered registered,major=%d\n",MAJOR(pseudo_dev_id));
        return 0;
}
static void __exit psample_exit(void)
{
	//per device cleanup
	C_DEV *pobj,*psafe;
	int i=0;
	list_for_each_entry_safe(pobj,psafe,&dev_list,lentry)
	{
		kfifo_free(&pobj->kfifo);	//dont deallocate pbuffer again using kfree
		cdev_del(&pobj->cdev);
		device_destroy(pseudo_cls,pseudo_dev_id+i);
		kfree(pobj);
		i++;
	}
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
