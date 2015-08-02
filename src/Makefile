CC = gcc

CFLAGS = -Wall			 	\
	-DG_DISABLE_DEPRECATED 	 	\
	-DGDK_DISABLE_DEPRECATED 	\
	-DGDK_PIXBUF_DISABLE_DEPRECATED \
	-DGTK_DISABLE_DEPRECATED

objects = main.o items.o Queue.o PriorityQueue.o

GTKFLAGS = $(CFLAGS) `pkg-config gtk+-2.0 gthread-2.0 --cflags --libs`

.PHONY: all
all: $(objects)
	$(CC) -o main.out $(objects) $(GTKFLAGS)

main.o: main.c
	$(CC) -c main.c $(GTKFLAGS)

items.o: items.h
	$(CC) -c items.c $(GTKFLAGS)

Queue.o: Queue.h
PriorityQueue.o: PriorityQueue.h

.PHONY: clean
clean:
	-rm *.o main.out
