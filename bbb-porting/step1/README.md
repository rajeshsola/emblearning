# Simple Steps to prepare the expected outcome

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
this setting effective for every launch of new shell(~/.bashrc) or every login(~/.bash_profile)

## Building the kernel

## Preparing rootfs

## Your First Boot
