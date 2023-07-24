
SRCFILE=uart_test.c
APP=uart_test

NFSDIR=~/source/rootfs

all:
	arm-linux-gnueabihf-gcc $(SRCFILE) -o $(APP)
install:
		cp $(APP) $(NFSDIR)
clean:
	rm $(APP)
