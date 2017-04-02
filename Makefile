# stx - stxing library
# See LICENSE file for copyright and license details.

include config.mk

CFLAGS := -I .

SRC_DIR = src
FUN =\
	stxapp\
	stxcpy\
	stxdel\
	stxensure_size\
	stxeq\
	stxfind\
	stxgrow\
	stxins\
	stxalloc\
	stxslice\
	stxstrip\
	stxswap\
	stxtrunc\
	stxuni8f32\
	stxvalid\
	stxavail\

SRC = ${FUN:=.c}
OBJ = ${FUN:=.o}
MAN3 = ${FUN:=.3}
MAN7 = ${TARGET:=.7}

HDR = libstx.h src/internal.h
TARGET = libstx.a
TARGETPP = libstxpp.a

DIST = $(basename ${TARGET})-${VERSION}
DIST_FILES = ${SRC_DIR} ${MAN_DIR} ${HDR} Makefile README config.mk test.c

all: ${TARGET}

options: config.mk
	@printf "${TARGET} build options:\n"
	@printf "CFLAGS  = ${CFLAGS}\n"
	@printf "LDFLAGS = ${LDFLAGS}\n"
	@printf "CC      = ${CC}\n"

%.o: ${SRC_DIR}/%.c ${HDR} config.mk
	@printf "CC $<\n"
	@${CC} ${CFLAGS} -c $<

${TARGET}: ${OBJ}
	@printf "Creating library archive ... "
	@ar -cq $@ ${OBJ}
	@printf "done.\n"

${TARGETPP}: ${OBJ}
	@printf "Creating library archive ... "
	@ar -cq $@ ${OBJ}
	@printf "done.\n"

ctest: ctest.c ${TARGET} ${OBJ} ${HDR}
	@printf "CC $<\n"
	@${CC} ${CFLAGS} ${LDFLAGS} -o $@ ctest.c ${OBJ}

check: ctest
	@./ctest

clean:
	@printf "Cleaning ... "
	@rm -f ${TARGET} ${OBJ} ${DIST}.tar.gz ctest
	@printf "done.\n"

vcheck:
	@valgrind ./ctest

dist: clean
	@printf "Creating dist tarball ... "
	@mkdir -p ${DIST}
	@cp -R ${DIST_FILES} ${DIST}
	@tar -cf ${DIST}.tar ${DIST}
	@gzip ${DIST}.tar
	@rm -rf ${DIST}
	@printf "done.\n"

install: all
	@printf "Installing library archive to ${DESTDIR}${LIBPREFIX}.\n"
	@mkdir -p ${DESTDIR}${LIBPREFIX}
	@cp -f ${TARGET} ${DESTDIR}${LIBPREFIX}
	@printf "Installing library header to ${DESTDIR}${INCLUDEPREFIX}.\n"
	@mkdir -p ${DESTDIR}${INCLUDEPREFIX}
	@cp -f ${HDR} ${DESTDIR}${INCLUDEPREFIX}

uninstall:
	@printf "Removing library archive from ${DESTDIR}${LIBPREFIX}.\n"
	@rm -f ${DESTDIR}${LIBPREFIX}/${TARGET}
	@printf "Removing library header from ${DESTDIR}${INCLUDEPREFIX}.\n"
	@rm -f ${DESTDIR}${PREFIX}/include/${HDR}
	@printf "Removing manual page from ${DESTDIR}${MANPREFIX}/man3/${MANPAGE}.\n"
	@rm -f ${DESTDIR}${MANPREFIX}/man3/${MANPAGE}

#man -t $< | ps2pdf - $@.pdf

.PHONY: all options clean dist install uninstall
