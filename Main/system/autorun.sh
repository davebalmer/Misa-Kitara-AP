#!/bin/ash

# System Startup Script

#insmod /lib/modules/nftl.ko partition=6
mount -t vfat -rw -o sync,umask=0,utf8,shortname=winnt /dev/nftl0 /media/usrdata

# check driver and AP in /media/usrdata =====>
if [ -d "/media/usrdata" ]; then
	if [ -e "/media/usrdata/driver/nastech_ts.ko" ]; then
	insmod /media/usrdata/driver/nastech_ts.ko
	tsmajor=`cat /proc/devices | grep nastech | cut -c2-3`
	mknod /dev/nastech_ts c $tsmajor 0
	fi
	if [ -e "/media/usrdata/driver/keyboard144.ko" ]; then
	insmod /media/usrdata/driver/keyboard144.ko
	kbmajor=`cat /proc/devices | grep keyboard144 | cut -c2-3`
	mknod /dev/keyboard144 c $kbmajor 0
	fi
	if [ -e "/media/usrdata/driver/guitar_key.ko" ]; then
	insmod /media/usrdata/driver/guitar_key.ko
	sleep 1
	cd /usr/bin
	./power_key &
	fi

	if [ -e "/media/usrdata/driver/sam3716.ko" ]; then
	mknod /dev/samdev c 100 0
	insmod /media/usrdata/driver/sam3716.ko
	cd /usr/bin
	./samprom fw /media/usrdata/3716-FW_MISA.bin
	fi

	if [ -e "/media/usrdata/driver/sgtl5000.ko" ]; then
	mknod /dev/sgtldev c 62 0
	insmod /media/usrdata/driver/sgtl5000.ko
	fi
	
	if [ -e "/media/usrdata/main" ]; then
	cd /media/usrdata
	./main &
	fi
fi
# <====
#insmod /lib/modules/ssludc.ko
#insmod /lib/modules/g_file_storage.ko file=/dev/nftl0 removable=1

cd /usr/bin
./otgd &
