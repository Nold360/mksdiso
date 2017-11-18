# Toolchain
CC = g++
STRIP = strip
WINDRES = windres
PACKER = res/upx
RM = rm

# flags
# add the -DDEBUG directive to CPPFLAGS to compile in DEBUG mode.
#CPPFLAGS = -O3 -DDEBUG
CPPFLAGS = -O3 -fPIC
PACKEROPT = -9 --compress-resources=0

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
	$(PACKER) $(PACKEROPT) $(TARGET)
# Linux compiling
linux : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CPPFLAGS) $(LIBS)
	$(STRIP) $(TARGET)
	./$(PACKER) $(PACKEROPT) $(TARGET)

# Mac OSX compiling, thanks to darcagn
osx : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CPPFLAGS) $(LIBS)
	$(STRIP) $(TARGET)


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
