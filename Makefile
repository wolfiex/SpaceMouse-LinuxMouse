CC = gcc
CFLAGS = -pedantic -Wall -g -I../.. -I/usr/local/include
LDFLAGS = -L../.. -L/usr/local/lib -lspnav -lX11 -lXtst

.PHONY: all
all: simple_x11 simple_af_unix

simple_x11: simple.c
	$(CC) $(CFLAGS) -DBUILD_X11 -o $@ $< $(LDFLAGS)
simple_af_unix: simple.c
	$(CC) $(CFLAGS) -DBUILD_AF_UNIX -o $@ $< $(LDFLAGS)
install:
	sudo apt-get install spacenavd
	sudo apt-get install libxtst-dev

start:
	sudo spaceavd
	
.PHONY: clean
clean:
	rm -f simple_x11 simple_af_unix
