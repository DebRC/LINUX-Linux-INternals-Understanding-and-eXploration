#!/bin/bash

## Building the applications
make -s

## Build your ioctl driver and load it here
cd module
make -s
sudo insmod query_ioctl_2.ko
cd ..

###############################################

# Launching the control station
./control_station &
c_pid=$!

# Launching the soldier
sudo ./soldier $c_pid &

sleep 2
kill -9 $c_pid

## Remove the driver here
sudo rmmod query_ioctl_2
cd module
make -s clean
cd ..

make -s clean
