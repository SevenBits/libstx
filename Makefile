# oystr - oystring library
# See LICENSE file for copyright and license details.

include config.mk

CFLAGS := -I .

SRC_DIR = src
FUN =\
	oystr_append\
	oystr_assign\
	oystr_deinit\
	oystr_ensure_size\
	oystr_eq\
	oystr_find\
	oystr_init\
	oystr_insert\
	oystr_printf\
	oystr_setlen\
	oystr_slice\
	oystr_strip\
	oystr_swap\
	oystr_trunc\
	oystr_utf8_from_utf32\
	oystr_valid\
	oystr_write

FUN_INLINE =\
	oystr_avail

SRC = ${FUN:=.c}
OBJ = ${FUN:=.o}
MAN3 = ${FUN:=.3} ${FUN_INLINE:=.3}
MAN7 = ${TARGET:=.7}

HDR = oystr.h src/internal.h
TARGET = liboystr.a

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

test: test.c ${TARGET}
	@printf "CC $<\n"
	@${CC} ${CFLAGS} ${LDFLAGS} -o $@ test.c ${TARGET}

check: test
	@./test

clean:
	@printf "Cleaning ... "
	@rm -f ${TARGET} ${OBJ} ${DIST}.tar.gz test
	@printf "done.\n"

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
	@printf "Installing manual page to ${DESTDIR}${MANPREFIX}/man3\n."
	@mkdir -p ${DESTDIR}${MANPREFIX}/man3
	@cp -f ${MANPAGE} ${DESTDIR}${MANPREFIX}/man3
	@chmod 644 ${DESTDIR}${MANPREFIX}/man3/${MANPAGE}

uninstall:
	@printf "Removing library archive from ${DESTDIR}${LIBPREFIX}.\n"
	@rm -f ${DESTDIR}${LIBPREFIX}/${TARGET}
	@printf "Removing library header from ${DESTDIR}${INCLUDEPREFIX}.\n"
	@rm -f ${DESTDIR}${PREFIX}/include/${HDR}
	@printf "Removing manual page from ${DESTDIR}${MANPREFIX}/man3/${MANPAGE}.\n"
	@rm -f ${DESTDIR}${MANPREFIX}/man3/${MANPAGE}

#man -t $< | ps2pdf - $@.pdf

.PHONY: all options clean dist install uninstall
