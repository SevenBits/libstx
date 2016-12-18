include config.mk

SRC=istrlib.c
OBJ=${SRC:.c=.o}

all: settings tests

settings:
	@echo istrlib build settings:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.mk

#@@@@@@@@@@@@@@@@@@@@@@@#
# TESTING SECTION BELOW #
#########################

TESTS=test_istrlib

tests: ${TESTS}

run_tests: tests
	$(foreach test,${TESTS},./${test})

test_istrlib: test_istrlib.c ${OBJ}
	${CC} -o test_istrlib test_istrlib.c ${OBJ}

#
#
#

clean:
	rm -f ${EXE} ${OBJ}
	rm -f ${TESTS}

.PHONY: all settings clean
