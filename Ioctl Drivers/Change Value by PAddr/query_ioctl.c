#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/mm.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include "query_ioctl.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DebRC");
MODULE_DESCRIPTION("IOCTL Driver");

#define FIRST_MINOR 0
#define MINOR_CNT 1

static dev_t dev;
static struct cdev c_dev;
static struct class *cl;

static int my_open(struct inode *i, struct file *f) {
    return 0;
}

static int my_close(struct inode *i, struct file *f) {
    return 0;
}

static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg) {
    Address address;
    WriteByte writeByte;
    volatile unsigned char *ptr;

    switch (cmd) {
        case IOCTL_TRANSLATE_ADDR:

            // Get Val from User
            if (copy_from_user(&address, (Address *)arg, sizeof(Address))) {
                return -EACCES;
            }

            // Set Physical Address
            address.phys_addr = virt_to_phys((void *)address.virt_addr);

            // Return Val to User
            if (copy_to_user((Address *)arg, &address, sizeof(Address))) {
                return -EACCES;
            }
            break;

        case IOCTL_WRITE_BYTE:
            // Write byte to the specified physical address

            if (copy_from_user(&writeByte, (WriteByte *)arg, sizeof(WriteByte))) {
                return -EACCES;
            }

            // Convert physical address to virtual address
            ptr = phys_to_virt(writeByte.phys_addr);

            // Write the byte value
            *ptr = writeByte.value;
            break;

        default:
            return -EINVAL;
    }

    return 0;
}

static struct file_operations query_fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_close,
    .unlocked_ioctl = my_ioctl,
};

static int __init query_ioctl_init(void) {
    int ret;
    struct device *dev_ret;

    pr_info("IOCTL Module 2 Loaded\n");

    if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "query_ioctl")) < 0) {
        return ret;
    }

    cdev_init(&c_dev, &query_fops);

    if ((ret = cdev_add(&c_dev, dev, MINOR_CNT)) < 0) {
        return ret;
    }

    if (IS_ERR(cl = class_create(THIS_MODULE, "char"))) {
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(cl);
    }

    if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "query"))) {
        class_destroy(cl);
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(dev_ret);
    }

    return 0;
}

static void __exit query_ioctl_exit(void) {
    pr_info("IOCTL Module 2 Unloaded\n");
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
}

module_init(query_ioctl_init);
module_exit(query_ioctl_exit);
