#!/bin/bash

if [ `whoami` != root ]; then
    echo Please run this script as root or using sudo
    exit
fi

dest_dir=/etc/udev/rules.d

echo Copying XTAG rules into $dest_dir...

if cp 99-xmos.rules $dest_dir > /dev/null
then
   echo Success
else
   echo Failure
fi

echo Restarting udev service...

if udevadm control --reload && udevadm trigger > /dev/null
then
   echo Success
else
   echo Failure
fi

echo Please reconnect all XTAG devices to enable changes
