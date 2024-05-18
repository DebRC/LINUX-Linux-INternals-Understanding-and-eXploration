#ifndef QUERY_IOCTL_H
#define QUERY_IOCTL_H

#include <linux/ioctl.h>
#include <linux/module.h>

// Define IOCTL Commands
#define IOCTL_CHANGE_PARENT _IOW('q', 1, int)

#endif
