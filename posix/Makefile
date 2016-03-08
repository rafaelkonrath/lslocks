CC=gcc
CFLAGS=-Wall


all: lslock-test lslock


lslock:
	$(CC) lslock.c  -o lslock $(CFLAGS)
	chmod 755 lslock

lslock-test:
	$(CC) lslock-test.c -o lslock-test $(CFLAGS)
	chmod 755 lslock-test
clean:
	rm lslock-test lslock

