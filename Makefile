VERSION=$(shell gawk '/^VERSION/ {print $1}' bin/mksdiso | cut -f2 -d\")
BUILD_DIR=build/mksdiso-$(VERSION)-all

default:
	@echo "Usage: make <package|install|clean>"
	@echo "   package | Build debian Package"
	@echo "   install | install to /usr/local/bin"
	@echo "   clean   | cleanup package build directory"
	exit 0	

package:
	mkdir -p $(BUILD_DIR)/usr/bin $(BUILD_DIR)/etc/mksdiso $(BUILD_DIR)/DEBIAN
	cp debian/control $(BUILD_DIR)/DEBIAN/
	cp bin/* $(BUILD_DIR)/usr/bin/
	cp -r mksdiso/* $(BUILD_DIR)/etc/mksdiso/
	sed -i 's/^DATADIR=.*/DATADIR=\/etc\/mksdiso/' $(BUILD_DIR)/usr/bin/mksdiso
	sed -i 's/^Version:.*/Version:\ $(VERSION)/' $(BUILD_DIR)/DEBIAN/control
	dpkg-deb --build $(BUILD_DIR)

install:
	cp -r bin/* /usr/local/bin/
	cp -r mksdiso ~/.mksdiso

clean:
	rm -r build/
