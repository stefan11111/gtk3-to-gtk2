.POSIX:

OBJ = $(shell ls *.c | sed --expression='s/\.c/.o/g')

all: libgtk-3.so.0

.c.o:
	${CC} ${XCFLAGS} -c -o $@ $<

libgtk-3.so.0: ${OBJ}
	${CC} ${XCFLAGS} ${OBJ} -lgtk-${TARGET}-2.0 -o libgtk-3.so.0 ${XLDFLAGS}

clean:
	rm -f ${OBJ}
	rm -f libgtk-3.so.0

.PHONY: all clean
