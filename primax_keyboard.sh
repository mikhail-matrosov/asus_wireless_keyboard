#!/bin/bash
modprobe usbmon
#stdbuf -i0 -o0 -e0 grep -h -Po '\d{3}:1.+8 = \K[0-9a-f ]+$' cat /sys/kernel/debug/usb/usbmon/*u > /tmp/kbdlog.txt
stdbuf -i0 -o0 -e0 grep -h -Po '\d{3}:1.+8 = \K[0-9a-f ]+$' /sys/kernel/debug/usb/usbmon/*u | /usr/local/bin/kbdusb