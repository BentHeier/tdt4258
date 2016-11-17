/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/ioport.h>
#include <linux/io.h>

#include "efm32gg.h"

/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

char current_state;

static ssize_t my_read(struct file *filp, char __user *buff, size_t count,
        loff_t *offp){
	if(count > 1){
		current_state = ioread32(GPIO_PC_DIN); 
		copy_to_user(buff, &current_state, 1);
		return 1;
	} return 0;
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
    .open = NULL,
    .release = NULL
};

dev_t device;
struct cdev my_cdev;
struct class *cl;

static int __init my_init(void)
{
    int error = 0;
	
	printk("Driver version 3\n");
    printk("Loading gamepad driver.\nAllocating chrdev_region...\n");
    if(error = alloc_chrdev_region(&device, 0, 1, "gamepad"))
        return error;

    printk("Initializing cdev struct...\n");
    cdev_init(&my_cdev, &my_fops);

	printk("Requesting IO-memory...\n");
	if(!request_mem_region(GPIO_PC_MODEL, 4, "gamepad"))
		return -1;
	if(!request_mem_region(GPIO_PC_DOUT, 4, "gamepad"))
		return -1;
	if(!request_mem_region(GPIO_PC_DIN, 4, "gamepad"))
		return -1;

	printk("Setting up GPIO...\n");
	// Pull-up resistors
	iowrite32(0xff, GPIO_PC_DOUT);
	// Input mode
	iowrite32(0x22222222, GPIO_PC_MODEL);

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
	 release_mem_region(GPIO_PC_MODEL, 4);
	 release_mem_region(GPIO_PC_DOUT, 4);
	 release_mem_region(GPIO_PC_DIN, 4);
     unregister_chrdev_region(device, 1);
}

module_init(my_init);
module_exit(my_exit);

MODULE_DESCRIPTION("Driver for gamepad buttons");
MODULE_LICENSE("GPL");

