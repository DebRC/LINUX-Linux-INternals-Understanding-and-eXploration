#!/bin/bash

# Compile the kernel module
echo "Running Makefile of the Kernel Module ..."
make
echo ""

# Load the kernel module
echo "Loading the Kernel Module ..."
sudo insmod query_ioctl.ko
echo ""

# Compile the user space application
echo "Compiling The User Space Application ..."
gcc -o test_query_ioctl test_query_ioctl.c
echo ""

# Run the user-space application
echo "Running User Space Application ..."
echo "-----------------------------"
echo ""
sudo ./test_query_ioctl
echo ""
echo "-----------------------------"

# Unload the kernel module
echo "Cleaning the Kernel Module ..."
sudo rmmod query_ioctl

# Clean up compiled files
make clean
echo ""