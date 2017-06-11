# Testing the board with Factory image on eMMC

# Updating Debian/Ubuntu image in eMMC

Download latest flasher image from https://rcn-ee.com/rootfs/, 

say https://rcn-ee.com/rootfs/2017-06-08/flasher/BBB-eMMC-flasher-debian-8.8-console-armhf-2017-06-08-2gb.img.xz

Dump the above image in a SD card as follows

Remove any other external storage devices from Host to be on safer side

#identify the presence of SD card attached to Host using

`fdisk -l`                           #locate the active partitions on the SD card in the form of /dev/sdb*

#identify any partitions of SD card are mounted using mount command and unmount each manually(dont use eject or safely remove options)

`mount`

`umount /media/<users>/<label>`       #repeat this for each partitions

`xzcat BBB-eMMC-flasher-debian-8.8-console-armhf-2017-06-08-2gb.img.xz > /dev/sdb`

#Now remove the card using eject or safely remove options and connecte to BBB target

Power on the board and observe the blinky pattern, remove the power and SD card when blink pattern changes

That's it you can boot the board with updated Debian/Ubuntu in eMMC

