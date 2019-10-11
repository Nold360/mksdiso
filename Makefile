VERSION=$(shell gawk '/^VERSION/ {print $1}' bin/mksdiso | cut -f2 -d\")
BUILD_DIR=build/mksdiso-$(VERSION)-all

default: build

help:
	@echo "Usage: make <package|install|clean>"
	@echo "   package | Build debian Package"
	@echo "   install | install to /usr/local/bin"
	@echo "   clean   | cleanup object files and package build directory"
	exit 0

build:
	cd src && $(MAKE)

package: build
	mkdir -p $(BUILD_DIR)/usr/bin $(BUILD_DIR)/usr/share/mksdiso $(BUILD_DIR)/DEBIAN
	cp debian/control $(BUILD_DIR)/DEBIAN/
	cp bin/* $(BUILD_DIR)/usr/bin/
	cp -r mksdiso/* $(BUILD_DIR)/usr/share/mksdiso/
	sed -i 's/^DATADIR=.*/DATADIR=\/usr\/share\/mksdiso/' $(BUILD_DIR)/usr/bin/mksdiso
	sed -i 's/^Version:.*/Version:\ $(VERSION)/' $(BUILD_DIR)/DEBIAN/control
	dpkg-deb --build $(BUILD_DIR)

# Local install, modifes mksdiso-data-path to /usr/local/share/mksdiso
install: build
	$(MAKE) -C src install
	sed -i 's#^DATADIR=.*#DATADIR=/usr/local/share/mksdiso#' /usr/local/bin/mksdiso
	mkdir -p /usr/local/share
	cp -r mksdiso /usr/local/share/
	cp -r bin/* /usr/local/bin/

uninstall:
	rm -r /usr/local/share/mksdiso || true
	rm /usr/local/bin/mksdiso || true
	rm /usr/local/bin/cdirip  || true
	rm /usr/local/bin/isofix  || true
	rm /usr/local/bin/binhack32 || true
	rm /usr/local/bin/burncdi || true
	rm /usr/local/bin/scramble || true

clean-package:
	-rm -r $(BUILD_DIR)

clean:	clean-package
	$(MAKE) -C src clean
