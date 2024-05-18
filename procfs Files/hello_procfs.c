#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define PROC_ENTRY_NAME "hello_procfs"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DebRC");
MODULE_DESCRIPTION("Hello Procfs Module");


static int hello_procfs_show(struct seq_file *m, void *v) {
    seq_printf(m, "Hello World!\n");
    return 0;
}

static int hello_procfs_open(struct inode *inode, struct file *file) {
    return single_open(file, hello_procfs_show, NULL);
}

static const struct proc_ops hello_procfs_ops = {
    .proc_open    = hello_procfs_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

static int __init hello_procfs_init(void) {
    proc_create(PROC_ENTRY_NAME, 0, NULL, &hello_procfs_ops);
    pr_info("Hello Procfs Module Loaded\n");
    return 0;
}

static void __exit hello_procfs_exit(void) {
    remove_proc_entry(PROC_ENTRY_NAME, NULL);
    pr_info("Hello Procfs Module Unloaded\n");
}

module_init(hello_procfs_init);
module_exit(hello_procfs_exit);