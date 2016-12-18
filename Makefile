include config.mk

ARCHIVE=istrlib.a
SRC=istrlib.c
OBJ=${SRC:.c=.o}
INCLUDE=istrlib.h
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

install: all
	@echo installing library archive to ${DESTDIR}${PREFIX}/lib/
	@mkdir -p ${DESTDIR}${PREFIX}/lib/
	@cp -f ${ARCHIVE} ${DESTDIR}${PREFIX}/lib/
	@echo installing manual page to ${DESTDIR}${MANPREFIX}/man1/
	@mkdir -p ${DESTDIR}${MANPREFIX}/man3/
	@cp ${MANPAGE} ${DESTDIR}${MANPREFIX}/man3/
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
	$(foreach test,${TESTS},./${test})

test_istrlib: test_istrlib.c ${OBJ}
	${CC} -o test_istrlib test_istrlib.c ${OBJ}

.PHONY: all settings clean run_tests
