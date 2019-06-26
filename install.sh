#!/bin/bash

gcc -o kbdusb kbdusb.c &&\
sudo cp kbdusb /usr/local/bin/. &&\
sudo cp primax_keyboard.sh /usr/local/bin/. &&\
sudo cp primax_keyboard.service /etc/systemd/system/. &&\
sudo systemctl enable primax_keyboard.service &&\
sudo systemctl start primax_keyboard.service