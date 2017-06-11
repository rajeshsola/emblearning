# Expert Steps for building the packages and first boot with generated outcome

## Prepare the pre built toolchain

### For 64 bit systems:-

 extract gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf.tar.xz instead of gcc-linaro-6.2.1-2016.11-i686_arm-linux-gnueabihf.tar.xz
 and rename to gcc-linaro-6.2.1

### Toolchain location
  You can place the toolchain in /opt instead of ~/eworkdir, in such case /opt/gcc-linraro-6.2.1 to be add to PATH variable,you may ignore this step initially!!

   export PATH=/opt/gcc-linaro-6.2.1/bin:$PATH

    and update to .bashrc or .bash_profile as usual.
    
## Building the kernel

### Making story of KERNEL.tar.gz

### Customizing configuration file

## Preparing rootfs

### Making story of bbrootfs.img

Prepare your own image based the downloaded tarball core-image-minimal-beaglebone.tar.bz2 from https://downloads.yoctoproject.org/releases/yocto/yocto-2.2/machines/beaglebone/ as follows, you may replace v2.2 with desired version

dd if=/dev/zero of=myrootfs.img bs=1M count=32

mkfs.ext4 myrootfs.img                           #sudo

mkdir /mnt/image        #one time                #sudo

mount -o loop,rw,sync myrootfs.img /mnt/image    #sudo

tar -jxvf core-image-minimal-beaglebone.tar.bz2 -C /mnt/image

echo "SUBSYSTEM=="tty", ATTR{uartclk}!="0", KERNEL=="ttyS[0-9]", SYMLINK+="ttyO%n" > /mnt/image/etc/udev/rules.d/60-omap-tty.rules

i.e. add echo "SUBSYSTEM=="tty", ATTR{uartclk}!="0", KERNEL=="ttyS[0-9]", SYMLINK+="ttyO%n" to /etc/udev/rules.d/60-omap-tty.rules 

umount /mnt/image

