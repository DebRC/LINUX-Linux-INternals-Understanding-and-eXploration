#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/vmstat.h>
#include <linux/seq_file.h>


#define PROC_ENTRY_NAME "get_pgfaults"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DebRC");
MODULE_DESCRIPTION("Get Page Faults Module");

static int get_pgfaults_show(struct seq_file *m, void *v) {
    unsigned long ret[NR_VM_EVENT_ITEMS];
    all_vm_events(ret);
    seq_printf(m, "Total Page Faults: %lu\n", ret[PGFAULT]);
    return 0;
}

static int get_pgfaults_open(struct inode *inode, struct file *file) {
    return single_open(file, get_pgfaults_show, NULL);
}

static const struct proc_ops get_pgfaults_fops = {
    .proc_open    = get_pgfaults_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

static int __init get_pgfaults_init(void) {
    proc_create(PROC_ENTRY_NAME, 0, NULL, &get_pgfaults_fops);
    pr_info("Get Page Faults Module Loaded\n");
    return 0;
}

static void __exit get_pgfaults_exit(void) {
    remove_proc_entry(PROC_ENTRY_NAME, NULL);
    pr_info("Get Page Faults Module Unloaded\n");
}

module_init(get_pgfaults_init);
module_exit(get_pgfaults_exit);