#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "query_ioctl.h"

int main() {
    int fd;
    Address address;
    WriteByte writeByte;
    unsigned char *byteValue;

    // Allocate a byte-size memory on the heap.
    byteValue = (unsigned char *)malloc(sizeof(unsigned char));

    if (byteValue == NULL) {
        perror("malloc");
        return 1;
    }

    // Assign the value “6” to the memory.
    *byteValue = 6;

    // Print the virtual address and the value of the allocated memory.
    printf("Virtual Address: %p\nValue at that Address: %d\n\n", (void *)byteValue, *byteValue);

    // Make an ioctl call to get the physical address of the allocated memory. 
    // Print the physical address.
    fd = open("/dev/query", O_RDWR);
    if (fd == -1) {
        perror("test_query_ioctl OPEN");
        return 2;
    }

    address.virt_addr = (unsigned long)byteValue;
    if (ioctl(fd, IOCTL_TRANSLATE_ADDR, &address) == -1) {
        perror("test_query_ioctl IOCTL_TRANSLATE_ADDR");
    } else {
        printf("Physical Address of the above Virtual Address: %lx\n\n", address.phys_addr);
    }

    // Step 5: Make another ioctl call to change the value of the memory to “5” using a physical memory address.
    writeByte.phys_addr = address.phys_addr;
    writeByte.value = 5;

    if (ioctl(fd, IOCTL_WRITE_BYTE, &writeByte) == -1) {
        perror("test_query_ioctl IOCTL_WRITE_BYTE");
    }

    // Step 6: Verify the modified value by printing the content of the allocated memory.
    printf("Modified Value at Virtual Address %lx and Physical Address %lx : %d\n", (void *)byteValue, address.phys_addr, *byteValue);

    // Cleanup
    free(byteValue);
    close(fd);

    return 0;
}
