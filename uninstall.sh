#!/bin/bash

sudo systemctl stop primax_keyboard.service &&\
sudo systemctl disable primax_keyboard.service &&\
sudo rm /etc/systemd/system/primax_keyboard.service &&\
sudo rm /usr/local/bin/primax_keyboard.sh &&\
sudo rm /usr/local/bin/kbdusb