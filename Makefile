CC := musl-gcc

default : master udp uart ping-pong

master : master.c
	 $(CC) -o $@ $?
udp : udp.c
	$(CC) -o $@ $?
uart : uart.c
	$(CC) -o $@ $?
ping-pong : ping-pong.c
	$(CC) -o $@ $?
clean :
	rm -f master udp uart ping-pong
