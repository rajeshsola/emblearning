# Expert Steps for building the packages and first boot with generated outcome

## Prepare the pre built toolchain

### For 64 bit systems:-
As better maintained toolchains are available for 64 bit architecture,Download and extract gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf.tar.xz from https://releases.linaro.org/components/toolchain/binaries/4.9-2017.01/arm-linux-gnueabihf/, follow the same steps to set the PATH as discussed in [First Steps](01-simple.md)
 
### Other versions of toolchain
Even though above versions are recommended initial work, you are free to choose a toolchain based on any gcc version, say v6.2.x available from https://releases.linaro.org/components/toolchain/binaries/ and keep getting updated time to time.Toolchain version used by Robert C Nelson's script can be a better choice!!

### Toolchain location
  You can place the toolchain in /opt instead of ~/eworkdir, in such case /opt/gcc-linraro-4.9 to be add to PATH variable,you may ignore this step initially!!

   export PATH=/opt/gcc-linaro-4.9/bin:$PATH

    and update to .bashrc or .bash_profile as discussed earlier.
    
## Building the kernel

### Making story of KERNEL.tar.gz

`git clone https://github.com/RobertCNelson/bb-kernel`

`cd bb-kernel`

`git checkout am335x-v4.9`   #choose any Stable/LTS version like 4.1.x, 4.4.x 

`#copy system.sh.sample as system.sh and update CC to prevent downloading toolchain again`

`./build_kernel.sh`

This steps require around 1 GB of download limit and takes some time to download entire repo from git.kernel.org,update necessary patches for the target board and build the kernel.

collect zImage, dtb file from deploy dir and especially config file (config-4.x.x-bonex.x) for further use.

`cd KERNEL`

`make clean`

`tar -zcvf KERNEL.tar.gz KERNEL`

Now this KERNEL.tar.gz, configuration file collected in deploy directory can be used independently without this build system.

### Customzing configuration file


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

