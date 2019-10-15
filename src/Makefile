# build all

DIRS = cdirip scramble makeip isofix binhack

all:
	for i in $(DIRS); do $(MAKE) -k -C $$i; done

clean:
	for i in $(DIRS); do $(MAKE) -k -C $$i clean; done

install:
	for i in $(DIRS); do $(MAKE) -k -C $$i install; done
