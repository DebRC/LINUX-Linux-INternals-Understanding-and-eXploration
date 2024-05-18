#include <linux/module.h>
#include <asm/io.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/mmap_lock.h>
#include <linux/init.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DebRC");
MODULE_DESCRIPTION("Get Process Size Kernel Module");

// Module Parameter for Process PID
// Default PID = 1 (INIT Process)
static int pid = 1;
// Give Read Access - S_IRUGO
module_param(pid, int, S_IRUGO);

// Structures for Task, Memory, VMA
struct task_struct *task;
struct mm_struct *mm;
struct vm_area_struct *vma;

static int __init get_VA_Size_init(void) {

    unsigned long virt_size,phys_size;

    pr_info("Get Process VA Size Module Loaded\n");

    // Find the process with the specified PID
    rcu_read_lock();
    task = pid_task(find_get_pid(pid), PIDTYPE_PID);
    rcu_read_unlock();

    // If not found print on Log
    if (!task) {
        pr_err("Process with PID %d not found.\n", pid);
        return 0;
    }

    // Get the memory management structure of the task
    mm=task->mm;

    if(!mm){
        pr_err("Process with PID %d Memory Space Not Accessible\n",pid);
        return 0;
    }

    // Perform Lock on mm
    mmap_read_lock(mm);
    
    // Get the VM Size
    virt_size=(mm->total_vm)<<PAGE_SHIFT;

    // Get the VMRss Size
    phys_size=get_mm_rss(mm)<<PAGE_SHIFT;

    // Unlock mm
    mmap_read_unlock(mm);


    pr_info("Total Virtual Address Space Size: %lu\n",virt_size);
    pr_info("Total Mapped Physical Address Space Size: %lu",phys_size);

    return 0;
}

static void __exit get_VA_Size_exit(void) {
    pr_info("Module Unloaded\n");
}

module_init(get_VA_Size_init);
module_exit(get_VA_Size_exit);
