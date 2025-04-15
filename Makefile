CC = gcc
CFLAGS = -Wall -g
PFLAGS = -fPIC
LDFLAGS = -shared -Wl,-soname,libclaves.so

all :: app-cliente-1 app-cliente-2 app-cliente-3 servidor-rpc libclaves.so

servidor-rpc: servidor-rpc.o claves.o claves-rpc_svc.o claves-rpc.o claves-rpc_xdr.o
	$(CC) $(CFLAGS) $^ -o $@

proxy-rpc.o: proxy-rpc.c
	$(CC) $(CFLAGS) $(PFLAGS) -c proxy-rpc.c -o proxy-rpc.o

libclaves.so: proxy-rpc.o claves-rpc.o
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
	rm -f app-cliente-1 app-cliente-2 app-cliente-3 servidor-rpc libclaves.so *.o
