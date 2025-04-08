CC = gcc
CFLAGS = -Wall -g
PFLAGS = -fPIC
LDFLAGS = -shared -Wl,-soname,libclaves.so

all :: app-cliente-1 app-cliente-2 app-cliente-3 servidor-sock libclaves.so

servidor-sock: servidor-sock.o claves.o comm.o
	$(CC) $(CFLAGS) $^ -o $@

proxy-sock.o: proxy-sock.c
	$(CC) $(CFLAGS) $(PFLAGS) -c proxy-sock.c -o proxy-sock.o

comm.o: comm.c comm.h
	$(CC) $(CFLAGS) $(PFLAGS) -c comm.c -o comm.o

libclaves.so: proxy-sock.o comm.o
	$(CC) $(LDFLAGS) $^ -o $@

app-cliente-1: app-cliente-1.o libclaves.so
	$(CC) $(CFLAGS) $^ -lclaves -L. -o $@ -Wl,-rpath,.

app-cliente-2: app-cliente-2.o libclaves.so
	$(CC) $(CFLAGS) $^ -lclaves -L. -o $@ -Wl,-rpath,.

app-cliente-3: app-cliente-3.o libclaves.so
	$(CC) $(CFLAGS) $^ -lclaves -L. -o $@ -Wl,-rpath,.

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f app-cliente-1 app-cliente-2 app-cliente-3 servidor-sock libclaves.so *.o
