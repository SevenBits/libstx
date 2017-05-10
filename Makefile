# see license file for copyright and license details.

include config.mk

CFLAGS := -I .

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
	stxtrunc\
	stxuni8f32\
	stxvalid\

SRC = ${FUN:=.c}
OBJ = ${FUN:=.o}
MAN3 = ${FUN:=.3}
MAN7 = ${TARGET:.a=.7}
TEST = $(addprefix ${TEST_DIR}/test_, ${FUN:=.test})

HDR = libstx.h
TARGET = libstx.a

DIST = $(basename ${TARGET})-${VERSION}
DIST_FILES = ${TEST_DIR} ${SRC_DIR} ${MAN_DIR} ${HDR} Makefile README config.mk\

all: ${TARGET}

options: config.mk
	@printf "${TARGET} build options:\n"
	@printf "CFLAGS  = ${CFLAGS}\n"
	@printf "LDFLAGS = ${LDFLAGS}\n"
	@printf "CC      = ${CC}\n"

%.o: ${SRC_DIR}/%.c ${HDR} src/internal.h config.mk
	@printf "CC $<\n"
	@${CC} ${CFLAGS} -c $<

${TARGET}: ${OBJ}
	@printf "Creating library archive ... "
	@ar -cq $@ ${OBJ}
	@printf "done.\n"

${TEST_DIR}/%.test: ${TEST_DIR}/%.c ${TARGET}
	@printf "CC $<\n"
	@${CC} ${CFLAGS} ${LDFLAGS} -o $@ $< ${OBJ}

check: ${TEST}
	@for i in ${TEST_DIR}/${TEST}; do \
		printf -- "----------------------------------------\n./$$i\n"; \
		./"$$i"; \
	done

clean:
	@printf "Cleaning ... "
	@rm -f ${TARGET} ${OBJ} ${TEST_DIR}/${TEST} ${DIST}.tar.gz
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
	@printf "Installing man pages to ${DESTDIR}${MANPREFIX}.\n"
	@mkdir -p ${DESTDIR}${MANPREFIX}/man7
	@cp -f $(addprefix ${DOC_DIR}/, ${MAN7}) ${DESTDIR}${MANPREFIX}/man7  
	@mkdir -p ${DESTDIR}${MANPREFIX}/man3
	@cp -f $(addprefix ${DOC_DIR}/, ${MAN3}) ${DESTDIR}${MANPREFIX}/man3

uninstall:
	@printf "Removing library archive from ${DESTDIR}${LIBPREFIX}.\n"
	@rm -f ${DESTDIR}${LIBPREFIX}/${TARGET}
	@printf "Removing library header from ${DESTDIR}${INCLUDEPREFIX}.\n"
	@rm -f ${DESTDIR}${PREFIX}/include/${HDR}

#man -t $< | ps2pdf - $@.pdf

.PHONY: all options check clean dist install uninstall
