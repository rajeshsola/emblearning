#  Pseudo Character Driver :: Step By Step
## [step1](step1)
Simple skeleton,registering device driver, checking major number and unregistering
## [step2](step2)
Adding file interface,with dummy operations to the registered driver.
## [step3](step3)
Creating class, device file nodes automatically(without the need of mknod)
## [step4](step4)
Adding global buffer as device data store.
## [step5a](step5a)
Using kfifo instead of global buffer.
## [step5b](step5b)
Using access_ok to avoid local copy.
## [step6](step6)
Using Private object approach to encapsulate per device declarations.
## [step7](step7)
Extending the driver for multiple devices and using list operations.
## [step8](step8)
Adding synchronization support,blocking the file operations when the buffer is full or empty!!
