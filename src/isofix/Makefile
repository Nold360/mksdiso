CC = gcc

PROG = isofix
OBJS = isofix13f.o
HDRS = isofix.h

$(PROG) : $(OBJS)
	$(CC) $(OBJS) -o $(PROG)

isofix.o : isofix13f.c $(HDRS)
	$(CC) -c $<

install:
	cp $(PROG) /usr/local/bin/

clean:
	rm -f $(OBJS) $(PROG) *~
