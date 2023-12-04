#!/bin/bash

sudo ipmctl delete -goal
sudo ipmctl create -goal Reserved=100

new_line_1='GRUB_DEFAULT="Advanced options for Ubuntu>Ubuntu, with Linux 5.15.0-89-generic"'
new_line_2='# GRUB_DEFAULT="Advanced options for Ubuntu>Ubuntu, with Linux 6.1.64"'
sed -i "6s/.*/$new_line_1/" "/etc/default/grub"
sed -i "7s/.*/$new_line_2/" "/etc/default/grub"

sudo update-grub
