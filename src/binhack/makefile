# Toolchain
CC = g++
STRIP = strip
WINDRES = windres
PACKER = upx
RM = rm

# flags
# add the -DDEBUG directive to CPPFLAGS to compile in DEBUG mode.
#CPPFLAGS = -O3 -DDEBUG
CPPFLAGS += -O3 -fPIC
PACKEROPT = -qq -9 --compress-resources=0

# directories
OBJ_DIR = obj/
SRC_DIR = src/
BIN_DIR = bin/

# files
BIN = binhack
TARGET = $(BIN_DIR)$(BIN)$(OS:Windows_NT=.exe)
LIBS =
OBJS = $(OBJ_DIR)main.o $(OBJ_DIR)binhack.o $(OS:Windows_NT=$(OBJ_DIR)version.o)

# go !
all : $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CPPFLAGS) $(LIBS)
	$(STRIP) $(TARGET)
	-$(PACKER) $(PACKEROPT) $(TARGET)

# Linux compiling (same as all)
linux : all

# Mac OS compiling (same as all)
osx : all

# compile source
$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

# include win32 resources
$(OBJ_DIR)%.o: $(SRC_DIR)%.rc
	$(WINDRES) -i $< -o $@

install: all
	mkdir -p /usr/local/bin/
	cp $(TARGET) /usr/local/bin/

clean:
	-$(RM) $(OBJS)

clean-all: clean
	-$(RM) $(OBJS) $(TARGET)

test: all
	cd bin; ./test_linux; ./clean_linux
