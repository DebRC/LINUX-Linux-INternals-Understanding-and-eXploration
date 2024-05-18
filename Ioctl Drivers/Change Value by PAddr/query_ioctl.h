#ifndef QUERY_IOCTL_H
#define QUERY_IOCTL_H

#include <linux/ioctl.h>
#include <linux/module.h>

// Address Structure
typedef struct {
    unsigned long virt_addr;
    unsigned long phys_addr;
} Address;

// WriteByte Structure
typedef struct {
    unsigned long phys_addr;
    unsigned char value;
} WriteByte;

// Define IOCTL Commands
#define IOCTL_TRANSLATE_ADDR _IOR('q', 1, Address *)
#define IOCTL_WRITE_BYTE _IOW('q', 2, WriteByte *)

#endif
