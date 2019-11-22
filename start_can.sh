#!/bin/bash

sudo modprobe i2c-dev
sudo modprobe can_dev
sudo modprobe can_raw
sudo modprobe can
sudo ip link set can0 up type can bitrate 1000000
sudo ifconfig can0 up