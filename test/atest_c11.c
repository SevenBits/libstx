#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include "libstx.h"
#include "test.h"

static const char teststr0[] = "";
static const char teststr1[] = "c";
static const char teststr2[] = "hello world";
static const char teststr3[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char teststr4[] = "\0hello \0world\r\n\0";
static const char teststr5[] = "lxjistriplxjimelxji";
static const char teststr6[] = "    strip me      ";

void
test_stxdup11()
{
	stx s1, s2;

	TEST_BEGIN {
		TEST_ASSERT(0 == stxdup(&s1, teststr3));
		TEST_ASSERT(0 == stxdup(&s2, stxref(&s1)));
		TEST_ASSERT(0 == memcmp(s1.mem, teststr3, s1.len));
		TEST_ASSERT(0 == memcmp(s2.mem, teststr3, s2.len));
	} TEST_END;

	stxdel(&s1);
	stxdel(&s2);
}

int
main(void)
{
	test_stxdup11();
	test_summary();
}
