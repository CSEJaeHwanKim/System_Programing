CC = gcc
AR = ar
RM = rm -rf
CFLAGS = -W -Wall
LIBS = -L. -lmy
TARGET = mymain
all : $(TARGET)
$(TARGET) : mymain.o libmy.a
	$(CC) -o $(TARGET) mymain.o $(LIBS)
mymain.o : mymain.c 
	$(CC) $(CFLAGS) -c mymain.c
libmy.a : foo.o boo.o bar.o
	$(AR) crv libmy.a foo.o boo.o bar.o
foo.o : foo.c foo.h
	$(CC) $(CFLAGS) -c foo.c
boo.o : boo.c boo.h
	$(CC) $(CFLAGS) -c boo.c
bar.o : bar.c bar.h
	$(CC) $(CFLAGS) -c bar.c
clean :
	$(RM) *.o *.a mymain
