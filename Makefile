EXEC = ext0
OBJS = ext0.o disk.o layout.o

CC = gcc
CFLAGS = -g -Og -Wall `pkg-config fuse --cflags`
LIBS = `pkg-config fuse --libs`

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $(EXEC) $(OBJS) $(LIBS)

%.o: %.c ext0.h
	$(CC) -c -o $@ $< $(CFLAGS)

test: test_disk.o test_layout.o disk.o layout.o
	$(CC) -o test_disk test_disk.o disk.o
	$(CC) -o test_layout test_layout.o layout.o
	rm -f test_disk.img
	./test_disk
	./test_layout

.PHONY: all clean test

clean:
	rm -f $(EXEC) test_disk test_layout *.o test_disk.img
