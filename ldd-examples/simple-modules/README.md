## Preparing the things

1.Identifying or obtaining kernel source

a) Identify the kernel source under /usr/src 

b) download the tarball of desired kernel version from kernel.org or a reputed site

c) checkout the kernel source from git.kernel.org or any reputed git repo

2.Obtain the suitable kernel configuration

a) Configuration of running kernel, extract /proc/config.gz

b) Vendor supplied config in /boot dir

c) Expert provided config file

d) Default configs avialble in kernel source

## Building custom kernel

Extract the kernel source tar ball or switch to existing source,let's call it as KSRC from now onwards

`make mrproper`

#copy the obtained configuration file as .config in KSRC

`make oldconfig`    #may skip initially

`make menuconfig`

`make -j <n>`

`make modules_install`

`make install`

Reboot the machine and identify the new entry for custom kernel in boot menu.

## Testing the new kernel

`uname -r`

`ls /boot/vmlinuz*`

`ls /lib/modules/linux*`

`ls -l /lib/modules/$(uname -r)/build`

`cd /lib/modules/$(uname -r)/build`

`pwd`

`ls -lh vmlinux`

`ls -lh /boot/vmlinuz*`

