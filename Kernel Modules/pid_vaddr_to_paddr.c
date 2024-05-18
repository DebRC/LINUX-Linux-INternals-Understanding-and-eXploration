#include <linux/module.h>
#include <asm/io.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/mmap_lock.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DebRC");
MODULE_DESCRIPTION("VA Translation Kernel Module");

// Module Parameter for Process PID
// Default PID = 1 (INIT Process)
static int pid = 1;
// Give Read Access - S_IRUGO
module_param(pid, int, S_IRUGO);

static unsigned long virtual_addr = 0;
module_param(virtual_addr, ulong, S_IRUGO);

// Structures for Task, Memory, VMA
struct task_struct *task;
struct mm_struct *mm;
struct vm_area_struct *vma;
struct page *pages[1];

static int __init check_VA_init(void) {

    unsigned long page_no;
    unsigned long physical_addr;
    int result;

    pr_info("VA Translation Module Loaded\n");

    // Find the process with the specified PID
    rcu_read_lock();
    task = pid_task(find_get_pid(pid), PIDTYPE_PID);
    rcu_read_unlock();

    pr_info("PID :: %d\n",pid);

    // If not found print on Log
    if (!task) {
        pr_err("Process with PID %d not found.\n", pid);
        return 0;
    }

    // Get the memory management structure of the task
    mm=task->mm;

    // // Perform Lock on mm
    mmap_read_lock(mm);

    result = get_user_pages_remote(mm, virtual_addr, 1, 0, pages, NULL, NULL);

    if (result > 0) {
        // 'pages' now contains an array of 'struct page' pointers corresponding to the virtual address range
        // You can access the page using pages[0]

        // Perform operations on the page if needed
        page_no = page_to_phys(pages[0]);
        physical_addr = page_no+offset_in_page(virtual_addr);

        pr_info("PID : %d, Virtual Address : 0x%lx, Physical Address : 0x%lx",pid,virtual_addr,physical_addr);

        
    } else {
        // Error handling
        printk(KERN_ERR "Failed to Get Physical Address: %d\n", result);
    }

    // // Unlock mm
    mmap_read_unlock(mm);
    return 0;
}

static void __exit check_VA_exit(void) {
    pr_info("Module Unloaded\n");
}

module_init(check_VA_init);
module_exit(check_VA_exit);
