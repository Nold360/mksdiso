CC = gcc

PROG = isofix
OBJS = isofix13f.o
HDRS = isofix.h

$(PROG) : $(OBJS)
	$(CC) $(OBJS) -o $(PROG) 

isofix.o : isofix13f.c $(HDRS)  
	$(CC) -c $<

clean:
	rm -f $(OBJS) $(PROG) *~
