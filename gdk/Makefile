.POSIX:

OBJ = $(shell ls *.c | sed --expression='s/\.c/.o/g')

INCS = $(shell find ../include -name '*.h')
HEADERS = $(shell find ../headers -name '*.h')
ALL_HEADERS = ${INCS} ${HEADERS}

TARGET = x11
OBJ += $(shell ls ${TARGET}/*.c | sed --expression='s/\.c/.o/g')

all:
	cd .. && make

install:
	cd .. && make install

gdk: libgdk-3.so.0

${OBJ}: ${ALL_HEADERS}

.c.o:
	${CC} ${XCFLAGS} -c -o $@ $<

libgdk-3.so.0: ${OBJ}
	${CC} ${XCFLAGS} ${OBJ} -lgdk-${TARGET}-2.0 -o libgdk-3.so.0 ${XLDFLAGS}

clean:
	rm -f ${OBJ}
	rm -f libgdk-3.so.0

.PHONY: all clean install gdk
