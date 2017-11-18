# build all

DIRS = cdirip scramble makeip isofix binhack

all:
	for i in $(DIRS); do cd $$i; make ; cd .. || true; done

clean:
	for i in $(DIRS); do make -C $$i clean || true; done

install:
	for i in $(DIRS); do make -C $$i install || true; done
