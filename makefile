CC=gcc
CFLAGS=-I. -g -Wall -Wextra `pkg-config --libs --cflags gtk+-2.0`
LDFLAGS=`pkg-config --libs --cflags gtk+-2.0`
DEPS = alarm.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

alarm: alarm.o
	$(CC) -o alarm alarm.o $(LDFLAGS)

.PHONY: clean
clean:
	rm -f *.o
