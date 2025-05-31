.POSIX:

OBJ = $(shell ls *.c | sed --expression='s/\.c/.o/g')

INCS = $(shell find ../include -name '*.h')
HEADERS = $(shell find ../headers -name '*.h')
ALL_HEADERS = ${INCS} ${HEADERS}


all:
	cd .. && make

gtk: libgtk-3.so.0

${OBJ}: ${ALL_HEADERS}

.c.o:
	${CC} ${XCFLAGS} -c -o $@ $<

libgtk-3.so.0: ${OBJ}
	${CC} ${XCFLAGS} ${OBJ} -lgtk-${TARGET}-2.0 -o libgtk-3.so.0 ${XLDFLAGS}

clean:
	rm -f ${OBJ}
	rm -f libgtk-3.so.0

.PHONY: all clean gtk
