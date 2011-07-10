#!/bin/ash

# System Startup Script

echo "###fuser -k /media/usrdata"
cd /
fuser -k /media/usrdata
sleep 1
fuser -k /media/usrdata
sleep 1
fuser -k /media/usrdata
/usr/bin/showbmp /usr/bin/usbconnect.bmp
echo "###insmod /lib/modules/g_file_storage.ko"
insmod /lib/modules/g_file_storage.ko file=/dev/nftl0 removable=1
