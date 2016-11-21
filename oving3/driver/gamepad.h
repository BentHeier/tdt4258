static int my_open (struct inode *inode, struct file *filp);

static int my_release (struct inode *inode, struct file *filp);

static ssize_t my_read (struct file *filp, char __user *buff, size_t count,
        loff_t *offp);

static ssize_t my_write (struct file *filp, const char __user *buff,
        size_t count, loff_t *offp);

static struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .read = my_read,
    .write = my_write,
    .open = my_open,
    .release = my_release
};

struct cdev my_cdev;
