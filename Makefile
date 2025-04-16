CC = gcc
CFLAGS = -Wall -g -I/usr/include/tirpc
PFLAGS = -fPIC
LDFLAGS = -shared -Wl,-soname,libclaves.so

all :: app-cliente-1 app-cliente-2 app-cliente-3 servidor-rpc libclaves.so

servidor-rpc.o: claves.o claves_rpc_svc.o claves_rpc_xdr.o
	$(CC) $(CFLAGS) $^ -ltirpc -o $@

proxy-rpc.o: proxy-rpc.c
	$(CC) $(CFLAGS) $(PFLAGS) -c proxy-rpc.c -o proxy-rpc.o

claves_rpc_clnt.o: claves_rpc_clnt.c
	$(CC) $(CFLAGS) $(PFLAGS) -c $< -o $@

claves_rpc_xdr.o: claves_rpc_xdr.c
	$(CC) $(CFLAGS) $(PFLAGS) -c $< -o $@

libclaves.so: proxy-rpc.o claves_rpc_clnt.o claves_rpc_xdr.o
	$(CC) $(LDFLAGS) $^ -ltirpc -o $@

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
