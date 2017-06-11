# Simple Steps for the first boot

Choose a suitable directory, say eworkdir under your home directory, we'll refer it as ~/eworkdir from now onwards

## Prepare the pre built toolchain

### For 32 bit systems:-

Extract gcc-linaro-6.2.1-2016.11-i686_arm-linux-gnueabihf.tar.xz in ~/eworkdir

tar -xvf gcc-linaro-6.2.1-2016.11-i686_arm-linux-gnueabihf.tar.xz

You may rename to short convenient name

mv gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf gcc-linaro-6.2.1

### For 64 bit systems:-

 extract gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf.tar.xz instead of gcc-linaro-6.2.1-2016.11-i686_arm-linux-gnueabihf.tar.xz
 and rename to gcc-linaro-6.2.1

### Common:-
Update PATH with new binaries in ~/eworkdir/gcc-linaro-6.2.1/bin as follows

export PATH=$HOME/eworkdir/gcc-linaro-6.2.1/bin:$PATH

The above setting is temporary and applicable to current shell only, add this to ~/.bashrc or ~/.bash_profile to take
this setting effective for every launch of new shell(~/.bashrc) or every login(~/.bash_profile

After slight experience you can place the toolchain in /opt instead of ~/eworkdir, in such case /opt/gcc-linraro-6.2.1 to be add to PATH variable,you may ignore this step initially!!

export PATH=/opt/gcc-linaro-6.2.1/bin:$PATH

and update to .bashrc or .bash_profile as usual.

## Building the kernel

Extract the given tar ball with kernel source in workdir, i.e. ~/eworkdir

tar -zxvf KERNEL.tar.gz -C ~/eworkdir

Let's call the extracted kernel source, i.e. ~/eworkdir/KERNEL as KSRC from now onwards

Follow these steps for custom building of kernel source for the target

make ARCH=arm mrproper

#copy config-4.9.0-step1 as .config in KSRC

make ARCH=arm menuconfig   #for further changes may skip initially

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage modules dtbs

Locate arch/arm/boot/zImage,arch/arm/boot/dts/am335x-boneblack.dtb w.r.t KSRC and copy them to a temporary directory say ~/eworkdir/deploy

## Preparing rootfs

You may use bbrootfs.img prepared based on yocto rootfs  initially 

(or)

Prepare your own image based the downloaded tarball core-image-minimal-beaglebone.tar.bz2 from https://downloads.yoctoproject.org/releases/yocto/yocto-2.2/machines/beaglebone/ as follows, you may replace v2.2 with desired version

dd if=/dev/zero of=myrootfs.img bs=1M count=32

mkfs.ext4 myrootfs.img                           #sudo

mkdir /mnt/image        #one time                #sudo

mount -o loop,rw,sync myrootfs.img /mnt/image    #sudo

tar -jxvf core-image-minimal-beaglebone.tar.bz2 -C /mnt/image

echo "SUBSYSTEM=="tty", ATTR{uartclk}!="0", KERNEL=="ttyS[0-9]", SYMLINK+="ttyO%n" > /mnt/image/etc/udev/rules.d/60-omap-tty.rules

i.e. add echo "SUBSYSTEM=="tty", ATTR{uartclk}!="0", KERNEL=="ttyS[0-9]", SYMLINK+="ttyO%n" to /etc/udev/rules.d/60-omap-tty.rules 

umount /mnt/image

## Your First Boot

Install minicom on host computer

Conenct TTL cable conenctors with debug port of target with following configuration

1-Black(Ground), 4-Green(Rx), 5-White(Tx), leave red conenctor open which is Vcc

Copy zImage, am335x-boneblack.dtb, bbrootfs.img or myrootfs.img on 1st part of SD card

Stop the autoboot thru minicom to enter u-boot console

mmcinfo

mmc dev 0

fatls mmc 0:1

fatload mmc 0:1 0x88080000 bbrootfs.img

#note down size

fatload mmc 0:1 0x82000000 zImage

fatload mmc 0:1 0x88000000 am335x-boneblack.dtb

setenv bootargs 'console=ttyO0,115200n8 root=/dev/ram0 rw initrd=0x88080000,<size>'

bootz 0x82000000 - 0x88000000

Let's celebrate your first boot if you are fortunate enough!!

## Let's try these alternate methods....

Custom kernel and rootfs on second part of eMMC

Custom kernel and rootfs on second part of SD card.

Loading kernel image, dtb file, initrd.img via TFTP from Host PC

Auto boot with custom kernel using uEnv command

