include config.mk

ARCHIVE=istrlib.a
SRC=istrlib.c
OBJ=${SRC:.c=.o}
HEADER=istrlib.h
MANPAGE=istrlib.3

all: settings istrlib.a

settings:
	@echo istrlib build settings:
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
	@mkdir -p istrlib-${VERSION}
	@cp -r * istrlib-${VERSION}
	tar -cvf istrlib-${VERSION}.tar istrlib-${VERSION}
	@gzip istrlib-${VERSION}.tar
	@rm -rf istrlib-${VERSION}

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
	@chmod 644 ${DESTDIR}${MANPREFIX}/man3/${MANPAGE}.1

clean:
	@echo "cleaning..."
	rm -f ${ARCHIVE} 
	rm -f ${OBJ}
	rm -f ${TESTS}

#
# TESTING SECTION BELOW
#
TESTS=test_istrlib

tests: ${TESTS}

run_tests: tests
	@echo running all tests
	$(foreach test,${TESTS},./${test})

test_istrlib: test_istrlib.c ${OBJ}
	${CC} -o test_istrlib test_istrlib.c ${OBJ}

.PHONY: all settings clean run_tests
