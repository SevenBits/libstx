# oystr - oystring library
# See LICENSE file for copyright and license details.

include config.mk

TARGET=liboystr.a
MANPAGE=oystr.3

SRC=oystr.c
HEADER=oystr.h
OBJ=${SRC:.c=.o}

DIST=$(basename ${TARGET})-${VERSION}
DIST_FILES=${SRC} Makefile README config.mk

all: ${TARGET}

options: config.mk
	@printf "${TARGET} build options:\n"
	@printf "CFLAGS  = ${CFLAGS}\n"
	@printf "LDFLAGS = ${LDFLAGS}\n"
	@printf "CC      = ${CC}\n"

.c.o:
	@printf "CC $<\n"
	@${CC} -c ${CFLAGS} $<

${OBJ}: config.mk

${TARGET}: ${OBJ} ${HEADER}
	@printf "Creating library archive ... "
	@ar -cq $@ ${OBJ}
	@printf "done.\n"

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
	@cp -f ${HEADER} ${DESTDIR}${INCLUDEPREFIX}
	@printf "Installing manual page to ${DESTDIR}${MANPREFIX}/man3\n."
	@mkdir -p ${DESTDIR}${MANPREFIX}/man3
	@cp -f ${MANPAGE} ${DESTDIR}${MANPREFIX}/man3
	@chmod 644 ${DESTDIR}${MANPREFIX}/man3/${MANPAGE}

uninstall:
	@printf "Removing library archive from ${DESTDIR}${LIBPREFIX}.\n"
	@rm -f ${DESTDIR}${LIBPREFIX}/${TARGET}
	@printf "Removing library header from ${DESTDIR}${INCLUDEPREFIX}.\n"
	@rm -f ${DESTDIR}${PREFIX}/include/${HEADER}
	@printf "Removing manual page from ${DESTDIR}${MANPREFIX}/man3/${MANPAGE}.\n"
	@rm -f ${DESTDIR}${MANPREFIX}/man3/${MANPAGE}

#------------------------------------------------------------------------------
# Testing section
#------------------------------------------------------------------------------

test: test.c ${OBJ} ${HEADER}
	@printf "CC $<\n"
	@${CC} -o $@ test.c ${OBJ} ${LDFLAGS}

.PHONY: all options clean dist install uninstall
