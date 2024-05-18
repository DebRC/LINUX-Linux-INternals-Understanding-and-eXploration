#include <linux/module.h>
#include <linux/sched/signal.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DebRC");
MODULE_DESCRIPTION("List Processes Kernel Module");

static int __init list_processes_init(void) {
    struct task_struct *tsk;
    //
    pr_info("LISTING ALL RUNNABLE PROCESSES:\n");
    /*    for_each_process() MACRO for iterating through 
    each task in the os located in linux\sched\signal.h    */
    for_each_process(tsk) {
        // 0 - Runnable
        if (tsk->__state == 0) {
            pr_info("PID: %d\t STATE: %d", tsk->pid, tsk->__state);
        }
    }
    return 0;
}

static void __exit list_processes_exit(void) {
    pr_info("Module Unloaded\n");
}

module_init(list_processes_init);
module_exit(list_processes_exit);