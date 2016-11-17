/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/cdev.h>

/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */


static int my_open(struct inode *inode, struct file *filp){

}

static int my_release(struct inode *inode, struct file *filp){

}

static ssize_t my_read(struct file *filp, char __user *buff, size_t count,
        loff_t *offp){
    return count;
}

static ssize_t my_write(struct file *filp, const char __user *buff,
        size_t count, loff_t *offp){
    printk("Du skrev ting til driveren. Driveren vet ikke helt hva den synes om dette\n");
    return count;
}

static struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .read = my_read,
    .write = my_write,
    .open = my_open,
    .release = my_release
};

dev_t device;
struct cdev my_cdev;
struct class *cl;

static int __init my_init(void)
{
    int error = 0;
	
    printk("Loading gamepad driver.\nAllocating chrdev_region...\n");
    if(error = alloc_chrdev_region(&device, 0, 1, "gamepad"))
        return error;

    printk("Initializing cdev struct...\n");
    cdev_init(&my_cdev, &my_fops);

    printk("Adding cdev...\n");
    if(error = cdev_add(&my_cdev, device, 1))
        return error;

    printk("Class/device create...\n");
    cl = class_create(THIS_MODULE, "gamepad");
    device_create(cl, NULL, device, NULL, "gamepad");
	return 0;
}

/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit my_exit(void)
{
	 printk("Unloading gamepad driver...\n");
     unregister_chrdev_region(device, 1);
}

module_init(my_init);
module_exit(my_exit);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

