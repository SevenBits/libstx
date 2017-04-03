#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include "libstx.h"
#include "test.h"

/**
 * This test suite is for most of the libstx library. Anything that is C99
 * compatible should be tested in this test suite.
 *
 * C11 specific code should be placed in "test11.c".
 */

static const char teststr0[] = "";
static const char teststr1[] = "c";
static const char teststr2[] = "hello world";
static const char teststr3[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char teststr4[] = "\0hello world\r\n\0";
static const char teststr5[] = "lxjistriplxjimelxji";
static const char teststr6[] = "    strip me      ";

void
assert_non_null(stx *s1, int len, int size) {
	test_assert(NULL != s1->mem);
	test_assert(len == s1->len);
	test_assert(size == s1->size);
}

void
test_stxgrow(void)
{
	stx s1;
	test_BEGIN;
	memset(&s1, 0, sizeof(s1));
	test_assert(0 == stxgrow(&s1, 10));
	assert_non_null(&s1, 0, 10);

	test_assert(0 == stxgrow(&s1, 20));
	assert_non_null(&s1, 0, 30);
	test_END;
	stxdel(&s1);
}

void
test_stxalloc(void)
{
	stx s1;
	test_BEGIN;
	test_assert(0 == stxalloc(&s1, 1));
	assert_non_null(&s1, 0, 1);
	test_END;
	stxdel(&s1);
}

void
test_stxensuresize(void)
{
	stx s1;
	test_BEGIN;
	memset(&s1, 0, sizeof(s1));
	test_assert(0 == stxensuresize(&s1, 65535));
	assert_non_null(&s1, 0, 65535);

	test_assert(0 == stxensuresize(&s1, 64));
	assert_non_null(&s1, 0, 65535);
	test_END;
	stxdel(&s1);
}

void
test_stxvalid(void)
{
	stx s1, s2, s3;
	test_BEGIN;
	s1.mem = NULL;
	stxalloc(&s2, 5);
	s2.len = 10;
	stxalloc(&s3, 10);
	test_assert(-1 == stxvalid(&s1));
	test_assert(-1 == stxvalid(&s2));
	test_assert(0 == stxvalid(&s3));
	stxdel(&s1);
	stxdel(&s2);
	stxdel(&s3);
	test_END;
}

void
test_stxcpy_mem(void)
{
	stx s1;
	stxalloc(&s1, sizeof(teststr2));
	test_BEGIN;
	test_assert(stxcpy_mem(&s1, teststr2, sizeof(teststr2)) == &s1);
	test_assert(sizeof(teststr2) == s1.len);
	test_assert(0 == memcmp(s1.mem, teststr2, sizeof(teststr2)));
	test_END;
	stxdel(&s1);
}

void
test_stxcpy_str(void)
{
	stx s1;
	stxalloc(&s1, sizeof(teststr2) - 1);
	test_BEGIN;
	test_assert(stxcpy_str(&s1, teststr2) == &s1);
	test_assert(sizeof(teststr2) - 1 == s1.len);
	test_assert(0 == memcmp(s1.mem, teststr2, sizeof(teststr2) - 1));
	test_END;
	stxdel(&s1);
}

void
test_stxapp_mem(void)
{
	stx s1 = {0};
	stxgrow(&s1, sizeof(teststr2) - 1);
	test_BEGIN;
	test_assert(stxapp_mem(&s1, teststr2, sizeof(teststr2) - 1) == &s1);
	test_assert(sizeof(teststr2) - 1 == s1.size);
	test_assert(sizeof(teststr2) - 1 == s1.len);
	test_assert(0 == memcmp(s1.mem, teststr2, sizeof(teststr2) - 1));

	stxgrow(&s1, sizeof("end"));
	test_assert(stxapp_mem(&s1, "end", sizeof("end")) == &s1);
	test_assert(sizeof(teststr2) - 1 + sizeof("end") == s1.size);
	test_assert(sizeof(teststr2) - 1 + sizeof("end") == s1.len);
	test_assert(0 == memcmp(s1.mem, teststr2, sizeof(teststr2) - 1));
	test_assert(0 == memcmp(s1.mem + sizeof(teststr2) - 1,
				"end", sizeof("end")));
	test_END;
	stxdel(&s1);
}

void
test_stxins_mem(void)
{
	stx s1 = {0};
	stxgrow(&s1, sizeof(teststr3) + sizeof(teststr2) - 1);
	stxcpy_mem(&s1, teststr3, sizeof(teststr3));
	test_BEGIN;
	test_assert(&s1 == stxins_mem(&s1, 10, teststr2, sizeof(teststr2) - 1));
	test_assert(0 == memcmp(s1.mem + 10, teststr2, sizeof(teststr2) - 1));
	test_assert(0 == memcmp(s1.mem, teststr3, 10));
	//test_assert(0 == memcmp(s1.mem + 10 + sizeof(teststr2) - 1, teststr3 + 10 + sizeof(teststr2), sizeof(teststr3) - 10 ));
	test_END;
	stxdel(&s1);
}

void
test_stxeq()
{
	stx s1, s2, s3;
	stxalloc(&s1, sizeof(teststr4));
	stxalloc(&s2, sizeof(teststr4));
	stxalloc(&s3, sizeof(teststr2));
	stxcpy_mem(&s1, teststr4, sizeof(teststr4));
	stxcpy_mem(&s2, teststr4, sizeof(teststr4));
	stxcpy_mem(&s3, teststr2, sizeof(teststr2));
	test_BEGIN;
	test_assert(true == stxeq(&s1, &s2));
	test_assert(false == stxeq(&s1, &s3));
	test_END;
	stxdel(&s1);
	stxdel(&s2);
	stxdel(&s3);
}

void
test_stxtrunc()
{
	stx s1;
	stxalloc(&s1, 4);
	stxcpy_mem(&s1, "wall", 4);
	test_BEGIN;
	test_assert(&s1 == stxtrunc(&s1, 2));
	test_assert(2 == s1.len);
	test_assert(4 == s1.size);
	test_assert(0 == strncmp(s1.mem, "wa", 2));
	test_END;
	stxdel(&s1);
}

void
test_stxslice()
{
	stx s1, slice;
	stxalloc(&s1, sizeof(teststr3));
	stxcpy_mem(&s1, teststr3, sizeof(teststr3));
	test_BEGIN;
	test_assert(&slice == stxslice(&slice, &s1, 4, 6));
	test_assert(2 == slice.len);
	test_assert(sizeof(teststr3) - 4 == slice.size);
	test_assert(s1.mem + 4 == slice.mem);
	test_END;
	stxdel(&s1);
}

void
test_stxfind_mem()
{
	stx s1, slice;
	stxalloc(&s1, sizeof(teststr4));
	stxcpy_mem(&s1, teststr4, sizeof(teststr4));
	test_BEGIN;
	test_assert(&slice == stxfind_mem(&slice, &s1, "world", 5));
	test_assert(5 == slice.len);
	test_assert(s1.size >= slice.size);
	test_assert(0 == strncmp(slice.mem, "world", 5));
	test_END;
	stxdel(&s1);
}

void
test_stxrstrip()
{
	stx s1;
	stxalloc(&s1, sizeof(teststr5));
	stxcpy_mem(&s1, teststr5, sizeof(teststr5));
	test_BEGIN;
	test_assert(&s1 == stxrstrip(&s1, "lxji", 5));
	test_assert(0 == strncmp(s1.mem, teststr5, sizeof(teststr5) - 5));
	test_assert(sizeof(teststr5) - 5 == s1.len);
	test_assert(sizeof(teststr5) == s1.size);
	test_END;
	stxdel(&s1);
}

void
test_stxlstrip()
{
	stx s1;
	stxalloc(&s1, sizeof(teststr5));
	stxcpy_mem(&s1, teststr5, sizeof(teststr5));
	test_BEGIN;
	test_assert(&s1 == stxlstrip(&s1, "lxji", 4));
	test_assert(0 == strncmp(s1.mem, teststr5 + 4, sizeof(teststr5) - 4));
	test_assert(sizeof(teststr5) - 4 == s1.len);
	test_assert(sizeof(teststr5) == s1.size);
	test_END;
	stxdel(&s1);
}

int
main(void)
{
	test_intro();

	test_stxgrow();
	test_stxalloc();
	test_stxensuresize();
	test_stxvalid();

	test_stxcpy_mem();
	test_stxcpy_str();
	test_stxapp_mem();
	test_stxins_mem();

	test_stxeq();
	test_stxtrunc();
	test_stxslice();
	test_stxfind_mem();

	test_stxrstrip();
	test_stxlstrip();
	/*
	test_stxunif32();
	test_compose();
	*/

	test_summary();
	return !(test_passed == test_total);
}
