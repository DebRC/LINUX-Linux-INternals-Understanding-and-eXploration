#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>

// Include header or define the IOCTL call interface and devide name
#include <sys/ioctl.h>
#include "module/query_ioctl_2.h"

//**************************************************

int open_driver(const char* driver_name) {

    int fd_driver = open(driver_name, O_RDWR);
    if (fd_driver == -1) {
        perror("ERROR: could not open driver");
    }

	return fd_driver;
}

void close_driver(const char* driver_name, int fd_driver) {

    int result = close(fd_driver);
    if (result == -1) {
        perror("ERROR: could not close driver");
    }
}


int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Usage: %s <parent_pid>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t parent_pid = atoi(argv[1]);

    printf("\nSoldier Started\n");

    printf("\nSoldier PID :: %ju\n", (uintmax_t)getpid());

    // Open ioctl driver
    int fd_driver = open_driver("/dev/query"); 

    // Call ioctl with parent pid as an argument to change the parent
    if (ioctl(fd_driver, IOCTL_CHANGE_PARENT, &parent_pid) == -1) {
        perror("ERROR: ioctl");
        close_driver("/dev/query", fd_driver);
        exit(EXIT_FAILURE);
    }

    // Close ioctl driver
    close_driver("/dev/query", fd_driver);
    
    printf("\nSoldier Died\n");

	return EXIT_SUCCESS;
}


