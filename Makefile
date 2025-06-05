.POSIX:
.SUFFIXES: .c .o .h .hh

#INCLUDES = -I. -I.. -I/usr/include/gtk-2.0 -I/usr/lib64/gtk-2.0/include -I/usr/include/pango-1.0 -I/usr/include/gdk-pixbuf-2.0 -I/usr/lib64/libffi/include -pthread -I/usr/include/fribidi -I/usr/include/harfbuzz -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/cairo -I/usr/include/libpng16 -I/usr/include/freetype2 -I/usr/include/pixman-1

#INCLUDES = -I. -I.. $(shell pkg-config --cflags gtk+-2.0)

INCLUDES = -I. -I.. $(shell pkg-config --cflags pango gdk-pixbuf-2.0)

XCFLAGS = ${CPPFLAGS} ${CFLAGS} -DGTK_COMPILATION=1 -DGDK_COMPILATION=1 -lm -std=c99 -fPIC -Wall -Wno-pedantic ${INCLUDES}
XLDFLAGS = ${LDFLAGS} -shared -Wl

LIBDIR = /lib64

XLIBDIR = /usr${LIBDIR}

XPKGCONFDIR = ${XLIBDIR}/pkgconfig

TARGET = $(shell pkg-config gtk+-2.0 --variable=target)

ifeq ($(TARGET), x11)
	XCFLAGS += -DX11=1
endif

TOP_SRCDIR = $(shell pwd)

# Private headers
XCFLAGS += -I${TOP_SRCDIR}/include

# Public headers
XCFLAGS += -I${TOP_SRCDIR}/headers

# For debugging only
XCFLAGS += -O0 -ggdb3

MAKE_ARGS = XCFLAGS="${XCFLAGS}" TARGET="${TARGET}"

# For deptracking
INCS = $(shell find include -name '*.h')
HEADERS = $(shell find headers -name '*.h')
ALL_HEADERS = ${INCS} ${HEADERS} headers/gdkconfig.h

GTK_SRC = $(shell find gtk -name '*.c')
GDK_SRC = $(shell find gdk -name '*.c')

ALL_LIBS = gtk/libgtk-3.so.0 gdk/libgdk-3.so.0

all: ${ALL_LIBS}

${ALL_LIBS} : ${ALL_HEADERS}

headers/gdkconfig.h:
	echo '/* gdkconfig from gtk2 */' > headers/gdkconfig.h
	echo '#include <'${XLIBDIR}'/gtk-2.0/include/gdkconfig.h>' >> headers/gdkconfig.h

gtk/libgtk-3.so.0: ${ALL_HEADERS} ${GTK_SRC}
	cd gtk && make gtk ${MAKE_ARGS} XLDFLAGS="${XLDFLAGS},-soname,libgtk-3.so.0"

gdk/libgdk-3.so.0: ${ALL_HEADERS} ${GDK_SRC}
	cd gdk && make gdk ${MAKE_ARGS} XLDFLAGS="${XLDFLAGS},-soname,libgdk-3.so.0"


install: ${ALL_LIBS}
	mkdir -p ${DESTDIR}${XLIBDIR}
	cp -f gtk/libgtk-3.so.0 ${DESTDIR}${XLIBDIR}/libgtk-3.so.0
	cp -f gdk/libgdk-3.so.0 ${DESTDIR}${XLIBDIR}/libgdk-3.so.0
	ln -rsf ${DESTDIR}${XLIBDIR}/libgtk-3.so.0 ${DESTDIR}${XLIBDIR}/libgtk-3.so
	ln -rsf ${DESTDIR}${XLIBDIR}/libgdk-3.so.0 ${DESTDIR}${XLIBDIR}/libgdk-3.so
	mkdir -p ${DESTDIR}${XPKGCONFDIR}

#	LIBDIR should only have one /, at the begining, like /lib or /lib64
	sed 's/@libdir@/\${LIBDIR}/g' pc/pc-${TARGET}/gail-3.0.pc > ${DESTDIR}${XPKGCONFDIR}/gail-3.0.pc
	sed 's/@libdir@/\${LIBDIR}/g' pc/pc-${TARGET}/gdk-3.0.pc > ${DESTDIR}${XPKGCONFDIR}/gdk-3.0.pc
	sed 's/@libdir@/\${LIBDIR}/g' pc/pc-${TARGET}/gtk+-3.0.pc > ${DESTDIR}${XPKGCONFDIR}/gtk+-3.0.pc
	sed 's/@libdir@/\${LIBDIR}/g' pc/pc-${TARGET}/gtk+-unix-print-3.0.pc > ${DESTDIR}${XPKGCONFDIR}/gtk+-unix-print-3.0.pc

	mkdir -p ${DESTDIR}/usr/include/gtk-3.0/gtk
	cp -rf headers/* ${DESTDIR}/usr/include/gtk-3.0

uninstall:
	rm -f ${DESTDIR}${XLIBDIR}/libgtk-3.so.0
	rm -f ${DESTDIR}${XLIBDIR}/libgdk-3.so.0

clean:
	cd gtk && make clean ${MAKE_ARGS}
	cd gdk && make clean ${MAKE_ARGS}
	rm -f headers/gdkconfig.h

.PHONY: all clean install uninstall
