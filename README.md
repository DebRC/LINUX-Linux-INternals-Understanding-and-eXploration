# Hands-On-with-Linux-Internals
This repository hosts the first assignment for the CS695 course, Spring 2023-24, 
aimed at diving deep into Linux internals. The assignment is divided into three main sections: 
creating and exploring Linux Kernel Modules (LKMs), implementing ioctl system calls, and working with the /proc filesystem.

## Setting Up the Environment
The assignment requires working on Linux machines running kernel version 6.1.0. To facilitate this, VM images for x86-64 and arm64 architectures are provided. Instructions for setting up VirtualBox or UTM (for Apple Silicon) are included, ensuring a consistent development environment.

## Kernel Modules
This section introduces the creation and manipulation of Linux Kernel Modules. Tasks include: 
- Creating LKMs to list running processes, inspect child processes, and analyze memory mappings.

## Ioctl Drivers
Focusing on the ioctl system call, this section involves:
- Writing an ioctl driver to manage memory mappings and direct memory manipulation.
- Creating a user-space program to interact with the ioctl driver.
- Modifying process parent-child relationships using ioctl to send SIGCHLD signals on process exit.

## Procfs Files
Exploring the /proc filesystem, tasks include:
- Creating a proc file entry to display a "Hello World!" message (hello_procfs.c).
- Developing a module to track and display system page faults (get_pgfaults.c).
