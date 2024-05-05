.POSIX:

XCFLAGS = ${CPPFLAGS} ${CFLAGS} -nostdlib -std=c99 -fPIC -Wall -Wno-pedantic
XLDFLAGS = ${LDFLAGS} -shared -Wl

TARGET = x11

all: libgtk-3.so.0 libgdk-3.so.0

.c.o:
	${CC} ${XCFLAGS} -c -o $@ $<

libgtk-3.so.0:
	${CC} ${XCFLAGS} libgtk-3.c -lgtk-${TARGET}-2.0 -o libgtk-3.so.0 ${XLDFLAGS},-soname,libgtk-3.so.0

libgdk-3.so.0:
	${CC} ${XCFLAGS} libgdk-3.c -lgdk-${TARGET}-2.0 -o libgdk-3.so.0 ${XLDFLAGS},-soname,libgdk-3.so.0

install: libgtk-3.so.0 libgdk-3.so.0
	mkdir -p ${DESTDIR}/usr/lib64
	cp -f *.so.0 ${DESTDIR}/usr/lib64
	ln -rsf ${DESTDIR}/usr/lib64/libgtk-3.so.0 ${DESTDIR}/usr/lib64/libgtk-3.so
	ln -rsf ${DESTDIR}/usr/lib64/libgdk-3.so.0 ${DESTDIR}/usr/lib64/libgdk-3.so
	mkdir -p ${DESTDIR}/usr/lib64/pkgconfig
	cp -f pc/* ${DESTDIR}/usr/lib64/pkgconfig
	mkdir -p ${DESTDIR}/usr/include/gtk-3.0/gtk
	cp -rf headers/* ${DESTDIR}/usr/include/gtk-3.0
uninstall:
	rm -f ${DESTDIR}/usr/lib64/libgtk-3.so.0 ${DESTDIR}/usr/lib64/libgdk-3.so.0

clean:
	rm -f *.so.0

.PHONY: all clean install uninstall
