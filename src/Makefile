
CC = gcc

CFLAGS = -Wall			 	\
	-DG_DISABLE_DEPRECATED 	 	\
	-DGDK_DISABLE_DEPRECATED 	\
	-DGDK_PIXBUF_DISABLE_DEPRECATED \
	-DGTK_DISABLE_DEPRECATED

main: main.c
	$(CC) main.c -o main.out $(CFLAGS) `pkg-config gtk+-2.0 gthread-2.0 --cflags --libs`

clean:
	rm -f *.o main.out
