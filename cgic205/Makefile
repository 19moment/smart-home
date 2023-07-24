CC=arm-linux-gnueabihf-gcc -static
AFLAGS=-Wall -g
LDFLAGS= -L ./lib
OBJS= capture.cgi m0_beep.cgi m0_fan.cgi m0_led.cgi \
	m0_seg.cgi env1.cgi setEnv.cgi

run:$(OBJS)

$(OBJS):%.cgi:%.c
	$(CC) $(AFLAGS) $< cgic.c -o $@ $(LDFLAGS)
	
install:
	$(CC) $(AFLAGS) login.c -o login.cgi

.PHONY:clean
clean:
	rm *.cgi

