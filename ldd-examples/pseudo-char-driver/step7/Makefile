obj-m += psample.o
KSRC=/lib/modules/$(shell uname -r)/build	
#replace KSRC if building for other kernel version
all:
	make -C ${KSRC} M=${PWD} modules
clean:
	make -C ${KSRC} M=${PWD} clean
