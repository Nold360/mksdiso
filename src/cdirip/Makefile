#####################################
#                                   
#  Custom-written for making Debian package for CDIRip
#                                   
#####################################
 
CC?=gcc

# Output filename
OUTPUT=cdirip

DESTDIR?=/usr/local

# Source files
SRCS=cdirip.c buffer.c cdi.c common.c audio.c
# Output object files (*.o)
OBJECTS = ${SRCS:.c=.o}
# Headers (*.h)
HEADERS= ${SRCS:.c=.h}

RM = rm -f
MV = mv -f
CP = cp -f

.c.o:
	${CC} -c ${CFLAGS} $<

${OUTPUT}: ${OBJECTS}
	${CC} ${LDFLAGS} -o $@ ${OBJECTS} -lm

all: ${OUTPUT}

test: all
	./${OUTPUT}

clean:
	-${RM} ${OUTPUT} ${OBJECTS}

mrproper: clean

install: all
	${CP} ${OUTPUT} ${DESTDIR}/bin
