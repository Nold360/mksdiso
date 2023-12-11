# Toolchain
CC = g++
STRIP = strip
WINDRES = windres
RM = rm

# flags
# add the -DDEBUG directive to CPPFLAGS to compile in DEBUG mode.
#CPPFLAGS = -O3 -DDEBUG
CPPFLAGS = -O3 -fPIC

# directories
OBJ_DIR = obj/
SRC_DIR = src/
BIN_DIR = bin/

# files
BIN = binhack32
TARGET = $(BIN_DIR)$(BIN)$(OS:Windows_NT=.exe)
LIBS =
OBJS = $(OBJ_DIR)main.o $(OBJ_DIR)binhack.o $(OS:Windows_NT=$(OBJ_DIR)version.o)

# go !
all : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CPPFLAGS) $(LIBS)
	$(STRIP) $(TARGET)
# Linux compiling
linux : all

# Mac OSX compiling, thanks to darcagn
osx : all

# compile source
$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

# include win32 resources
$(OBJ_DIR)%.o: $(SRC_DIR)%.rc
	$(WINDRES) -i $< -o $@

install: linux
	cp bin/binhack32 bin/binhack64 /usr/local/bin/

clean:
	$(RM) $(OBJS)

clean-all:
	$(RM) $(OBJS) $(TARGET)
