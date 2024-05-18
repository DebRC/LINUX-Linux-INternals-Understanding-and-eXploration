#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DebRC");
MODULE_DESCRIPTION("List Child Processes Kernel Module");

// Module Parameter for Parent Process PID
// Default PID = 1 (INIT Process)
static int parent_pid = 1;
// Give Read Access - S_IRUGO
module_param(parent_pid, int, S_IRUGO);


// Structure for parent and child task
struct task_struct *parent_task, *child_task;

static int __init list_child_processes_init(void) {

    pr_info("List Child Processes Kernel Module Loaded\n");

    // Find the parent process with the specified PID
    rcu_read_lock();
    parent_task = pid_task(find_get_pid(parent_pid), PIDTYPE_PID);
    rcu_read_unlock();

    // If not found print on Log
    if (!parent_task) {
        pr_err("Parent process with PID %d not found.\n", parent_pid);
        return -EINVAL;
    }

    pr_info("Listing child processes of PID %d:\n", parent_pid);

    // Iterate over child processes
    list_for_each_entry_rcu(child_task, &parent_task->children, sibling) {
        pr_info("Child PID: %d, State: %u\n", child_task->pid, child_task->__state);
    }
    return 0;
}

static void __exit list_child_processes_exit(void) {
    pr_info("Module Unloaded\n");
}

module_init(list_child_processes_init);
module_exit(list_child_processes_exit);
