#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/mm.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/signal.h>
#include <linux/spinlock.h>
#include "query_ioctl_2.h"

DEFINE_SPINLOCK(children_list_lock);

#define FIRST_MINOR 0
#define MINOR_CNT 1

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DebRC");
MODULE_DESCRIPTION("IOCTL Driver 2");

static dev_t dev;
static struct cdev c_dev;
static struct class *cl;

// Function to add a child to the parent's children list
void add_child(struct task_struct *parent_task, struct task_struct *child_task) {

    spin_lock(&children_list_lock);

    // Add the child to the end of the children list
    list_add_tail(&child_task->sibling, &parent_task->children);

    spin_unlock(&children_list_lock);

    pr_info("PID: %d Added to Children List of PID :%d\n", child_task->pid,parent_task->pid);
}

// Function to add a parent to the child's parent attr
void add_parent(struct task_struct *parent_task, struct task_struct *child_task){
    rcu_read_lock();
    child_task->parent = parent_task;
    child_task->real_parent = parent_task;
    rcu_read_unlock();
}

static int my_open(struct inode *i, struct file *f) {
    return 0;
}

static int my_close(struct inode *i, struct file *f) {
    return 0;
}

static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg) {
    int parent_pid = 0;
    struct task_struct *parent_task;
    struct task_struct *current_task = current;

    switch (cmd) {
        case IOCTL_CHANGE_PARENT:
            // Get target pid from user
            if (copy_from_user(&parent_pid, (int *)arg, sizeof(int))) {
                return -EACCES;
            }
            printk(KERN_INFO "PARENT PID RECEIVED :: %d\n",parent_pid);
            // Find the parent process with the specified PID
            rcu_read_lock();
            parent_task = pid_task(find_get_pid(parent_pid), PIDTYPE_PID);
            rcu_read_unlock();
            if (parent_task != NULL) {
                pr_info("CURRENT PROCESS PID :: %d\n",current_task->pid);
                
                add_parent(parent_task,current_task);
                
                add_child(parent_task,current_task);

                printk(KERN_INFO "CHANGED CURRENT PROCESS PARENT PID TO :: %d\n", current_task->parent->pid);
            } else {
                printk(KERN_ERR "INVALID PARENT PID: %d\n", parent_pid);
            }
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
