CFLAGS = -DPORT=$(PORT) -g -Wall -std=gnu99
DEPENDENCIES = Computations.h tcpserver.h

all: tcpclient tcpserver

tcpclient: tcpclient.o
	gcc ${CFLAGS} -o $@ $^

tcpserver: tcpserver.o Computations.o
	gcc ${CFLAGS} -o $@ $^ -lm

%.o: %.c ${DEPENDENCIES}
	gcc ${CFLAGS} -c $<

clean:
	rm *.o tcpclient tcpserver
