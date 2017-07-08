## Adding System Calls to the custom kernel

Add a new entry to system call table in `arch/x86/syscalls/syscall_32.tbl`

`350 i386 testcall sys_testcall`

Replace 350 with suitable number in sequence according to your kernel source

Add the definition of sys_testcall as a static module as follows

Create a new directory under **KSRC**, say **kernel/stest** and place **mysycall.c** in it with following code
```C
asmlinkage long sys_testcall(void) //SYSCALL_DEFINE0(testcall)
{
        printk("testcall\n");
        return 0;
}
```
and place a Makefile in stest dir 

`obj-y += mysyscall.o`

Link the newly created directory to the **Makefile** of parent directory,i.e. in **kernel/Makefile** add following line

`obj-y += stest`

Optionally add the prototype of your system calls to `include/linux/syscalls.h`

`asmlinkage long sys_testcall(void);`

Rebuild the kernel source to update custom kernel with added system call

`make -j <n>`

`make modules_install`

`make install`

## Testing the presence new system call in updated kernel(after rebooting)

`cat /proc/kallsyms | grep sys_testcall`

`nm vmlinux | grep sys_testcall`

`objdump -d vmlinux | less`

## Invoking system call from userspace

Write a simple user space program as follows
```C
#define __NR_testcall 350
int main()
{
   ret=syscall(__NR_testcall);
   if(ret < 0)
   {
      perror("testcall");
      exit(1);
   }
}
```
