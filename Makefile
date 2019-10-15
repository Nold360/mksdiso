VERSION=$(shell gawk '/^VERSION/ {print $1}' bin/mksdiso | cut -f2 -d\")
BUILD_DIR=build/mksdiso-$(VERSION)-all

default: compile

help:
	@echo "Usage: make <package|install|clean>"
	@echo "   package | Build debian Package"
	@echo "   install | install to /usr/local/bin"
	@echo "   clean   | cleanup object files and package build directory"
	exit 0

compile:
	$(MAKE) -C src

package: compile
	mkdir -p $(BUILD_DIR)/usr/bin $(BUILD_DIR)/usr/share/mksdiso $(BUILD_DIR)/DEBIAN
	cp debian/control $(BUILD_DIR)/DEBIAN/
	cp bin/* $(BUILD_DIR)/usr/bin/
	cp src/binhack/bin/binhack $(BUILD_DIR)/usr/bin/
	cp src/cdirip/cdirip $(BUILD_DIR)/usr/bin/
	cp src/isofix/isofix $(BUILD_DIR)/usr/bin/
	cp src/makeip/makeip $(BUILD_DIR)/usr/bin/
	cp src/scramble/scramble $(BUILD_DIR)/usr/bin/
	cp -r mksdiso/* $(BUILD_DIR)/usr/share/mksdiso/
	sed -i 's/^Version:.*/Version:\ $(VERSION)/' $(BUILD_DIR)/DEBIAN/control
	dpkg-deb --build $(BUILD_DIR)

# Local sysadmin install goes to /usr/local/
install: compile
	$(MAKE) -C src install
	mkdir -p /usr/local/share
	cp -r mksdiso /usr/local/share/
	cp -r bin/* /usr/local/bin/

uninstall:
	-rm -r /usr/local/share/mksdiso
	-rm /usr/local/bin/mksdiso
	-rm /usr/local/bin/cdirip 
	-rm /usr/local/bin/isofix 
	-rm /usr/local/bin/binhack
	-rm /usr/local/bin/burncdi 
	-rm /usr/local/bin/scramble

clean-package:
	-rm -r $(BUILD_DIR)

clean:	clean-package
	$(MAKE) -C src clean
