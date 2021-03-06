# see license file for copyright and license details.

include config.mk

#CFLAGS := -I .

SRC_DIR = src
DOC_DIR = doc
TEST_DIR = test

FUN =\
	stxalloc\
	stxapp\
	stxavail\
	stxcmp\
	stxcpy\
	stxdup\
	stxensuresize\
	stxfind\
	stxfree\
	stxgrow\
	stxins\
	stxref\
	stxslice\
	stxstrip\
	stxswap\
	stxterm\
	stxtok\
	stxtrunc\
	stxutf\
	stxvalid\

MAN3 = ${FUN:=.3}
MAN7 = ${TARGET:.a=.7}

SRC = $(addprefix ${SRC_DIR}/, ${FUN:=.c})
OBJ = $(addprefix ${SRC_DIR}/, ${FUN:=.o})

TEST_SRC = $(addprefix ${TEST_DIR}/test_, ${FUN:=.c})
TEST = $(addprefix ${TEST_DIR}/test_, ${FUN:=.test})

TARGET = libstx.a

DIST = $(basename ${TARGET})-${VERSION}
DIST_FILES = ${TEST_DIR} ${SRC_DIR} ${MAN_DIR} libstx.h Makefile README config.mk

all: ${TARGET}

options: config.mk
	@printf "${TARGET} build options:\n"
	@printf "CFLAGS  = ${CFLAGS}\n"
	@printf "LDFLAGS = ${LDFLAGS}\n"
	@printf "CC      = ${CC}\n"

%.o: %.c config.mk
	@printf "CC $<\n"
	@${CC} ${CFLAGS} -c -o $@ $<

%.test: %.c ${TARGET} config.mk
	@printf "CC $<\n"
	@${CC} ${CFLAGS} -o $@ $< ${TARGET} ${LDFLAGS}

${OBJ}: libstx.h ${SRC_DIR}/internal.h

${TARGET}: ${OBJ}
	@printf "Creating library archive ... "
	@ar -cq $@ ${OBJ}
	@printf "done.\n"

${TEST}: libstx.h ${TEST_DIR}/test.h

test: ${TEST}

check: test
	./test/run_all.sh

clean:
	@printf "Cleaning ... "
	@rm -f ${OBJ} ${TARGET} ${TEST} ${DIST}.tar.gz
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
	@cp -f libstx.h ${DESTDIR}${INCLUDEPREFIX}
	@printf "Installing man pages to ${DESTDIR}${MANPREFIX}.\n"
	@mkdir -p ${DESTDIR}${MANPREFIX}/man7
	@cp -f $(addprefix ${DOC_DIR}/, ${MAN7}) ${DESTDIR}${MANPREFIX}/man7  
	@mkdir -p ${DESTDIR}${MANPREFIX}/man3
	@cp -f $(addprefix ${DOC_DIR}/, ${MAN3}) ${DESTDIR}${MANPREFIX}/man3

uninstall:
	@printf "Removing library archive from ${DESTDIR}${LIBPREFIX}.\n"
	@rm -f ${DESTDIR}${LIBPREFIX}/${TARGET}
	@printf "Removing library header from ${DESTDIR}${INCLUDEPREFIX}.\n"
	@rm -f ${DESTDIR}${PREFIX}/include/libstx.h

#man -t $< | ps2pdf - $@.pdf

.PHONY: all options check clean dist install uninstall
