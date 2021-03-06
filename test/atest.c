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
static const char teststr4[] = "\0hello \0world\r\n\0";
static const char teststr5[] = "lxjistriplxjimelxji";
static const char teststr6[] = "    strip me      ";

void
assert_non_null(stx *s1, int len, int size) {
	TEST_ASSERT(NULL != s1->mem);
	TEST_ASSERT(len == s1->len);
	TEST_ASSERT(size == s1->size);
}

void
test_stxgrow(void)
{
	stx s1 = {0};
	TEST_BEGIN {
		TEST_ASSERT(0 == stxgrow(&s1, 10));
		assert_non_null(&s1, 0, 10);

		TEST_ASSERT(0 == stxgrow(&s1, 20));
		assert_non_null(&s1, 0, 30);
	} TEST_END;
	stxdel(&s1);
}

void
test_stxalloc(void)
{
	stx s1;
	TEST_BEGIN {
		TEST_ASSERT(0 == stxalloc(&s1, 1));
		assert_non_null(&s1, 0, 1);
	} TEST_END;
	stxdel(&s1);
}

void
test_stxensuresize(void)
{
	stx s1 = {0};
	TEST_BEGIN {
		TEST_ASSERT(0 == stxensuresize(&s1, 65535));
		assert_non_null(&s1, 0, 65535);

		TEST_ASSERT(0 == stxensuresize(&s1, 64));
		assert_non_null(&s1, 0, 65535);
	} TEST_END;
	stxdel(&s1);
}

void
test_stxvalid(void)
{
	stx s1, s2, s3;
	s1.mem = NULL;
	stxalloc(&s2, 5);
	s2.len = 10;
	stxalloc(&s3, 10);

	TEST_BEGIN {
		TEST_ASSERT(false == stxvalid(&s1));
		TEST_ASSERT(false == stxvalid(&s2));
		TEST_ASSERT(true == stxvalid(&s3));
		stxdel(&s1);
		stxdel(&s2);
		stxdel(&s3);
	} TEST_END;
}

void
test_stxcpy_mem(void)
{
	stx s1;
	stxalloc(&s1, sizeof(teststr2));
	TEST_BEGIN {
		TEST_ASSERT(stxcpy_mem(&s1, teststr2, sizeof(teststr2)) == &s1);
		TEST_ASSERT(sizeof(teststr2) == s1.len);
		TEST_ASSERT(0 == memcmp(s1.mem, teststr2, sizeof(teststr2)));
	} TEST_END;
	stxdel(&s1);
}

void
test_stxcpy_str(void)
{
	stx s1;
	stxalloc(&s1, sizeof(teststr2) - 1);
	TEST_BEGIN {
		TEST_ASSERT(stxcpy_str(&s1, teststr2) == &s1);
		TEST_ASSERT(sizeof(teststr2) - 1 == s1.len);
		TEST_ASSERT(0 == memcmp(s1.mem, teststr2, sizeof(teststr2) - 1));
	} TEST_END;
	stxdel(&s1);
}

void
test_stxdup(void)
{
	stx s1, s2, s3;

	TEST_BEGIN {
		TEST_ASSERT(0 == stxdup_mem(&s1, teststr4, sizeof(teststr4)));
		TEST_ASSERT(0 == stxdup_str(&s2, teststr3));
		TEST_ASSERT(0 == stxdup_spx(&s3, stxref(&s1)));
		TEST_ASSERT(0 == memcmp(s1.mem, teststr4, s1.len));
		TEST_ASSERT(0 == memcmp(s2.mem, teststr3, s2.len));
		TEST_ASSERT(0 == memcmp(s3.mem, teststr4, s3.len));
	} TEST_END;

	stxdel(&s1);
	stxdel(&s2);
	stxdel(&s3);
}

void
test_stxapp_mem(void)
{
	stx s1 = {0};
	stxgrow(&s1, sizeof(teststr2) - 1);
	TEST_BEGIN {
		TEST_ASSERT(stxapp_mem(&s1, teststr2, sizeof(teststr2) - 1) == &s1);
		TEST_ASSERT(sizeof(teststr2) - 1 == s1.size);
		TEST_ASSERT(sizeof(teststr2) - 1 == s1.len);
		TEST_ASSERT(0 == memcmp(s1.mem, teststr2, sizeof(teststr2) - 1));

		stxgrow(&s1, sizeof("end"));
		TEST_ASSERT(stxapp_mem(&s1, "end", sizeof("end")) == &s1);
		TEST_ASSERT(sizeof(teststr2) - 1 + sizeof("end") == s1.size);
		TEST_ASSERT(sizeof(teststr2) - 1 + sizeof("end") == s1.len);
		TEST_ASSERT(0 == memcmp(s1.mem, teststr2, sizeof(teststr2) - 1));
		TEST_ASSERT(0 == memcmp(s1.mem + sizeof(teststr2) - 1,
					"end", sizeof("end")));
	} TEST_END;
	stxdel(&s1);
}

void
test_stxins_mem(void)
{
	stx s1 = {0};
	stxgrow(&s1, sizeof(teststr3) + sizeof(teststr2) - 1);
	stxcpy_mem(&s1, teststr3, sizeof(teststr3));
	TEST_BEGIN {
		TEST_ASSERT(&s1 == stxins_mem(&s1, 10, teststr2, sizeof(teststr2) - 1));
		TEST_ASSERT(0 == memcmp(s1.mem + 10, teststr2, sizeof(teststr2) - 1));
		TEST_ASSERT(0 == memcmp(s1.mem, teststr3, 10));
		//TEST_ASSERT(0 == memcmp(s1.mem + 10 + sizeof(teststr2) - 1, teststr3 + 10 + sizeof(teststr2), sizeof(teststr3) - 10 ));
	} TEST_END;
	stxdel(&s1);
}

void
test_stxeq()
{
	stx s1, s2, s3;

	stxdup_str(&s1, teststr4);
	stxdup_spx(&s2, stxref(&s1));
	stxdup_str(&s3, teststr2);

	TEST_BEGIN {
		TEST_ASSERT(true == stxeq(stxref(&s1), stxref(&s2)));
		TEST_ASSERT(false == stxeq(stxref(&s1), stxref(&s3)));
	} TEST_END;
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
	TEST_BEGIN {
		TEST_ASSERT(&s1 == stxtrunc(&s1, 2));
		TEST_ASSERT(2 == s1.len);
		TEST_ASSERT(4 == s1.size);
		TEST_ASSERT(0 == strncmp(s1.mem, "wa", 2));
	} TEST_END;
	stxdel(&s1);
}

void
test_stxslice()
{
	stx s1, slice;
	stxalloc(&s1, sizeof(teststr3));
	stxcpy_mem(&s1, teststr3, sizeof(teststr3));
	TEST_BEGIN {
		const spx sp1 = stxslice(stxref(&s1), 4, 6);
		TEST_ASSERT(NULL != sp1.mem);
		TEST_ASSERT(2 == sp1.len);
		TEST_ASSERT(s1.mem + 4 == sp1.mem);
	} TEST_END;
	stxdel(&s1);
}

void
test_stxfind_mem()
{
	stx s1;
	stxalloc(&s1, sizeof(teststr4));
	stxcpy_mem(&s1, teststr4, sizeof(teststr4));
	TEST_BEGIN {
		const spx sp1 = stxfind_mem(stxref(&s1), "world", 5);
		TEST_ASSERT(NULL != sp1.mem);
		TEST_ASSERT(5 == sp1.len);
		TEST_ASSERT(0 == strncmp(sp1.mem, "world", 5));
	} TEST_END;
	stxdel(&s1);
}

void
test_stxrstrip()
{
	stx s1;
	stxalloc(&s1, sizeof(teststr5));
	stxcpy_mem(&s1, teststr5, sizeof(teststr5));
	TEST_BEGIN {
		TEST_ASSERT(&s1 == stxrstrip(&s1, "lxji", 5));
		TEST_ASSERT(0 == strncmp(s1.mem, teststr5, sizeof(teststr5) - 5));
		TEST_ASSERT(sizeof(teststr5) - 5 == s1.len);
		TEST_ASSERT(sizeof(teststr5) == s1.size);
	} TEST_END;
	stxdel(&s1);
}

void
test_stxlstrip()
{
	stx s1;
	stxalloc(&s1, sizeof(teststr5));
	stxcpy_mem(&s1, teststr5, sizeof(teststr5));
	TEST_BEGIN {
		TEST_ASSERT(&s1 == stxlstrip(&s1, "lxji", 4));
		TEST_ASSERT(0 == strncmp(s1.mem, teststr5 + 4, sizeof(teststr5) - 4));
		TEST_ASSERT(sizeof(teststr5) - 4 == s1.len);
		TEST_ASSERT(sizeof(teststr5) == s1.size);
	} TEST_END;
	stxdel(&s1);
}

int
main(void)
{
	test_stxgrow();
	test_stxalloc();
	test_stxensuresize();
	test_stxvalid();

	test_stxcpy_mem();
	test_stxcpy_str();
	test_stxdup();
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
