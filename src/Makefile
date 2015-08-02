CC = gcc

CFLAGS = -Wall			 	\
	-DG_DISABLE_DEPRECATED 	 	\
	-DGDK_DISABLE_DEPRECATED 	\
	-DGDK_PIXBUF_DISABLE_DEPRECATED \
	-DGTK_DISABLE_DEPRECATED

objects = main.o items.o Queue.o PriorityQueue.o

GTKFLAGS = `pkg-config gtk+-2.0 gthread-2.0 --cflags --libs`

.PHONY: all
all: main $(objects)

main: $(objects)
	$(CC) -o main.out $(objects) $(CFLAGS) $(GTKFLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS) $(GTKFLAGS)

items.o: items.c items.h
	$(CC) -c items.c $(CFLAGS) $(GTKFLAGS)

Queue.o: Queue.c Queue.h
PriorityQueue.o: PriorityQueue.c PriorityQueue.h

.PHONY: clean
clean:
	-rm *.o main.out
