.POSIX:

OBJ = $(shell ls *.c | sed --expression='s/\.c/.o/g')

all: libgdk-3.so.0

.c.o:
	${CC} ${XCFLAGS} -c -o $@ $<

libgdk-3.so.0: ${OBJ}
	${CC} ${XCFLAGS} ${OBJ} -lgdk-${TARGET}-2.0 -o libgdk-3.so.0 ${XLDFLAGS}

clean:
	rm -f ${OBJ}
	rm -f libgdk-3.so.0

.PHONY: all clean
