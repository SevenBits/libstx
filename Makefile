include config.mk

ARCHIVE=libustr.a

HEADER=libustr.h
SRC=libustr.c
OBJ=${SRC:.c=.o}

MANPAGE=libustr.3

all: settings run_test libustr.a

settings:
	@echo libustr build settings:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.mk

${ARCHIVE}: ${OBJ}
	@echo creating library archive
	@ar -cq $@ ${OBJ}

# Tests below

TEST=test

${TEST}: ${TEST}.c ${OBJ}
	${CC} -o ${TEST} ${TEST}.c ${OBJ}

run_test: ${TEST}
	./${TEST}

clean:
	@echo "cleaning..."
	rm -f ${ARCHIVE} 
	rm -f ${OBJ}
	rm -f ${TESTS}
	rm -f libustr-${VERSION}.tar.gz

dist: clean
	@echo creating dist tarball
	@mkdir -p libustr-${VERSION}
	@cp -r LICENSE README.md Makefile config.mk ${TEST_SRC} ${SRC} ${HEADER} ${MANPAGE} libustr-${VERSION}
	tar -cvf libustr-${VERSION}.tar libistr-${VERSION}
	@gzip libustr-${VERSION}.tar
	@rm -rf libustr-${VERSION}

install: all
	@echo installing library archive to ${DESTDIR}${PREFIX}/lib/
	@mkdir -p ${DESTDIR}${PREFIX}/lib/
	@cp -f ${ARCHIVE} ${DESTDIR}${PREFIX}/lib/
	@echo installing library header to ${DESTDIR}${PREFIX}/include/
	@mkdir -p ${DESTDIR}${PREFIX}/include/
	@cp -f ${HEADER} ${DESTDIR}${PREFIX}/include/
	@echo installing manual page to ${DESTDIR}${MANPREFIX}/man3/
	@mkdir -p ${DESTDIR}${MANPREFIX}/man3/
	@sed "s/VERSION/${VERSION}/g" < ${MANPAGE} > ${DESTDIR}${MANPREFIX}/man3/${MANPAGE}
	@chmod 644 ${DESTDIR}${MANPREFIX}/man3/${MANPAGE}

uninstall:
	@echo removing library archive from ${DESTDIR}${PREFIX}/lib/
	@rm -f ${DESTDIR}${PREFIX}/lib/${ARCHIVE}
	@echo removing library header from ${DESTDIR}${PREFIX}/include/
	@rm -f ${DESTDIR}${PREFIX}/include/${HEADER}
	@echo removing manual page from ${DESTDIR}${MANPREFIX}/man3/${MANPAGE}
	@rm -f ${DESTDIR}${MANPREFIX}/man3/${MANPAGE}

.PHONY: all settings clean run_tests
