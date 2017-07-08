# Pseudo Char Driver -- Simple Skeleton

Registering char driver and obtaining major number

Compile the above module using given Makefile and load it

`cat /proc/devices`

`dmesg | grep pseudo`

Compare the major number from above two commands, and create the device node manually as follows

`mknod c /dev/pseudo c 250 0`     #assuming assigned major number is 250

### code

[psample.c](https://github.com/rajeshsola/emblearning/tree/master/ldd-examples/pseudo-char-driver/step1/psample.c)

[Makefile](https://github.com/rajeshsola/emblearning/tree/master/ldd-examples/pseudo-char-driver/step1/Makefile)
