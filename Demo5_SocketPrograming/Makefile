CC = gcc
app = mymain
all : $(app)
$(app) : Server.o Client.o Important.o
	$(CC) -o Server Server.o Important.o
	$(CC) -o Client Client.o Important.o
Server.o : Server.c
	$(CC) -c Server.c
Client.o : Client.c
	$(CC) -c Client.c
Important.o : Important.c
	$(CC) -c Important.c
clean : 
	rm *.o Server Client
