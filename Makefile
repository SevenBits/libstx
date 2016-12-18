include config.mk

ARCHIVE=libistr.a

HEADER=libistr.h
SRC=libistr.c
OBJ=${SRC:.c=.o}

TEST_SRC=test_libistr.c
TESTS=test_libistr

MANPAGE=libistr.3

all: settings libistr.a

settings:
	@echo libistr build settings:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.mk

${ARCHIVE}: ${OBJ}
	@echo creating library archive
	@ar -cq $@ ${OBJ}

dist: clean
	@echo creating dist tarball
	@mkdir -p libistr-${VERSION}
	@cp -r LICENSE README.md Makefile config.mk ${TEST_SRC} ${SRC} ${HEADER} ${MANPAGE} libistr-${VERSION}
	tar -cvf libistr-${VERSION}.tar libistr-${VERSION}
	@gzip libistr-${VERSION}.tar
	@rm -rf libistr-${VERSION}

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

clean:
	@echo "cleaning..."
	rm -f ${ARCHIVE} 
	rm -f ${OBJ}
	rm -f ${TESTS}
	rm -f libistr-${VERSION}.tar.gz

#
# TESTING SECTION BELOW
#

tests: ${TESTS}

run_tests: tests
	@echo running all tests
	$(foreach test,${TESTS},./${test})

test_libistr: test_libistr.c ${OBJ}
	${CC} -o test_libistr test_libistr.c ${OBJ}

.PHONY: all settings clean run_tests
