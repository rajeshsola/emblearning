# Simple Steps for the first boot

Choose a suitable directory, say eworkdir under your home directory, we'll refer it as ~/eworkdir from now onwards

## Prepare the pre built toolchain

### For 32 bit systems:-

Extract gcc-linaro-6.2.1-2016.11-i686_arm-linux-gnueabihf.tar.xz in ~/eworkdir

tar -xvf gcc-linaro-6.2.1-2016.11-i686_arm-linux-gnueabihf.tar.xz

You may rename to short convenient name

mv gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf gcc-linaro-6.2.1

Update PATH with new binaries in ~/eworkdir/gcc-linaro-6.2.1/bin as follows

export PATH=$HOME/eworkdir/gcc-linaro-6.2.1/bin:$PATH

The above setting is temporary and applicable to current shell only, add this to ~/.bashrc or ~/.bash_profile to take
this setting effective for every launch of new shell(~/.bashrc) or every login(~/.bash_profile

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
