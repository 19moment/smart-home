
#KERNELDIR :=/lib/modules/$(shell uname -r)/build
KERNELDIR :=/home/fengjunhui/kernel/linux-3.14
PWD  :=$(shell pwd)

CROSS_COMPILE=arm-linux-
#CROSS_COMPILE=
CC=$(CROSS_COMPILE)gcc
CP=cp

ARM_DRVDIR=~/source/rootfs/drv/
NFS_BOA_WWWDIR=~/source/rootfs/boa/www/
NFS_BOA_CGIDIR=~/source/rootfs/boa/cgi-bin/

obj-m +=demo.o 

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	$(CC) -o cgi_led.cgi -static cgi_led.c
	mv *.o *.mod.c *.order *.symvers obj

install:
	sudo cp demo.ko  $(ARM_DRVDIR)
	sudo cp *.mp3 *.jpg led.html $(NFS_BOA_WWWDIR)
	sudo cp cgi_led.cgi  $(NFS_BOA_CGIDIR)

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -rf cgi_led.cgi


