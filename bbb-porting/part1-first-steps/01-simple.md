# Simple Steps for the first boot

Choose a suitable directory for current work, say eworkdir under your home directory, we'll refer `~/eworkdir` as our workdir from now onwards

## Prepare the pre built toolchain

Extract gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux.tar.xz in ~/eworkdir

`tar -xvf gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux.tar.xz`

You may rename to short convenient name

`mv gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux.tar.xz gcc-linaro-4.9`

Update PATH with new binaries in ~/eworkdir/gcc-linaro-4.9/bin as follows

`export PATH=$HOME/eworkdir/gcc-linaro-4.9/bin:$PATH`

The above setting is temporary and applicable to current shell only, add this to .bashrc or .bash_profile in your home directory to take this setting effective for every launch of new shell(.bashrc) or every login(.bash_profile)
eg:-  

`vi ~/.bashrc  #add above command at end of this file`

You can verify the toolchain preparation as follows,preferbly launch a new terminal and test this to avoid any mistakes w.r.t PATH updations

`arm-linux-gnueabihf-gcc -v`      should print suitable version, say 4.9.x

`which arm-linux-gnueabihf-gcc`    if any error like command not found re-verify above steps

These steps are assuming that you are using 32 bit system for Host, follow [expert](02-expert.md) steps for 64 bit Host machine and betetr location,other versions of the toolchain

## Building the kernel

Extract the given tar ball with kernel source in workdir, i.e. ~/eworkdir, follow [expert](02-expert.md) steps for the making story behind KERNEL source

`tar -zxvf KERNEL.tar.gz -C ~/eworkdir`

Let's call the extracted kernel source, i.e. `~/eworkdir/KERNEL` as KSRC from now onwards

Follow these steps for custom building of kernel source for the target

`make ARCH=arm mrproper`

`#copy config-4.9.0-step1 as .config in KSRC` follow [expert](02-expert.md) steps for the making story behind config file

`make ARCH=arm menuconfig`   #for further changes may skip initially

`make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage modules dtbs`

Locate `arch/arm/boot/zImage`,`arch/arm/boot/dts/am335x-boneblack.dtb` w.r.t KSRC and copy them to a temporary directory say `~/eworkdir/deploy`

## Preparing rootfs

You may use myrootfs.img prepared based on yocto rootfs  initially 

follow [expert](02-expert.md) for the making story behind myrootfs.img

## Your First Boot

Install minicom on host computer, and configure it with device port of conencted TTL cable,typically `/dev/ttyUSB0` and `1152008N1` configuration,here are some hints to configure minicom.
`minicom -s`

_Serial port setup_

*hit A to change device file name, hit enter*

*hit E to change other baud rate settings using suitable key strokes, hit Enter*

*Save setup as dfl*

and run `minicom` without -s option now

Conenct TTL cable conenctors with debug port of target with following configuration

1-Black(Ground), 4-Green(Rx), 5-White(Tx), leave red conenctor open which is Vcc

Copy zImage, am335x-boneblack.dtb, bbrootfs.img or myrootfs.img to the 1st partition of SD card from `~/eworkdir/deploy`

Now power on the board and stop the autoboot thru minicom by hitting any key within few seconds to enter u-boot console

`mmcinfo`  To check the presence of SD card and basic information

`mmc dev 0`

`fatls mmc 0:1` For listing of contents in first partition of SD card

`fatload mmc 0:1 0x88080000 myrootfs.img` note down the size of rootfs,33554432 for myrootfs.img,loading rootfs image as initrd

`fatload mmc 0:1 0x82000000 zImage` Loading kernel image from SD card to RAM

`fatload mmc 0:1 0x88000000 am335x-boneblack.dtb` Loading dtb file

`setenv bootargs 'console=ttyO0,115200n8 root=/dev/ram0 rw initrd=0x88080000,33554432'` replace size for other rootfs

`bootz 0x82000000 - 0x88000000`

Please follow some [expert](02-expert.md) steps on choosing these addresses without collision.

Please follow [booting methods](03-booting-methods.md) for alternative methods on booting the board

