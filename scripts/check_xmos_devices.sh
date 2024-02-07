
#!/bin/bash

# This script checks:
#  * For presence of XMOS XTAG devices.
#  * That those devices are accessible by the current user

if [ `whoami` == root ]; then
    echo Warning: This script is designed to run without root permissions
fi

# These two arrays must have equal length
xtag_ids=("20b1:f7d4" "20b1:f7d5")
xtag_names=("xtag3"     "xtag4")

for i in ${!xtag_ids[@]};
do
  xtag_id=${xtag_ids[$i]}
  xtag_name=${xtag_names[$i]}

  echo Searching for $xtag_name devices...

  num_devices=`lsusb | grep -c $xtag_id`

  echo $num_devices found

  if ! [ $num_devices == 0 ]
  then
    if ! lsusb -d $xtag_id -v 2>&1 | grep "Couldn't open device" > /dev/null
    then
      echo Success: User `whoami` is able to access all $xtag_name devices
    else
      echo Error: User `whoami` is not permitted to access at least one of the $xtag_name devices
      echo Please run setup_xmos_devices.sh as root and replug all XTAG devices
    fi
  fi
done
