EXEC = ext0
OBJS = ext0.o

CC = gcc
CFLAGS = -g -Og -Wall `pkg-config fuse --cflags`
LIBS = `pkg-config fuse --libs`

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $(EXEC) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: all clean

clean:
	rm -f $(EXEC) $(OBJS)
