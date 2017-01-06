CC = g++
RM = rm -rf
CFLAGS = -c -Wall -I. -D_DEBUG
server: main.o filedao.o oracledao.o logqueue.o \
	logthread.o clientthread.o storethread.o \
	serversocket.o server.o
	$(CC) $^ -lpthread -o $@
main.o: main.cpp
	$(CC) $(CFLAGS) $^
filedao.o: filedao.cpp
	$(CC) $(CFLAGS) $^
oracledao.o: oracledao.cpp
	$(CC) $(CFLAGS) $^
logqueue.o: logqueue.cpp
	$(CC) $(CFLAGS) $^
logthread.o: logthread.cpp
	$(CC) $(CFLAGS) $^
clientthread.o: clientthread.cpp
	$(CC) $(CFLAGS) $^
storethread.o: storethread.cpp
	$(CC) $(CFLAGS) $^
serversocket.o: serversocket.cpp
	$(CC) $(CFLAGS) $^
server.o: server.cpp
	$(CC) $(CFLAGS) $^
clean:
	$(RM) server *.o
