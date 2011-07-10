#!/bin/ash

# System Startup Script

echo "###umount /media/usrdata"
umount /media/usrdata

echo "###mount /media/usrdata"
mount -t vfat -rw -o sync,umask=0,utf8,shortname=winnt /dev/nftl0 /media/usrdata

echo "###run /media/usrdata/main"
# check driver and AP in /media/usrdata =====>
if [ -d "/media/usrdata" ]; then
        if [ -e "/media/usrdata/main" ]; then
        cd /media/usrdata
        ./main &
        fi
fi

rmmod /lib/modules/g_file_storage.ko