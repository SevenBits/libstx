#include "libstx.h"

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

static int ctest_ok = 1;
#define ctest_assert(x) _ctest_assert(__func__, __LINE__, (x))
#define CTEST_BEGIN ctest_begin(__func__)
#define CTEST_END ctest_end()

static int ctest_passed = 0;
static int ctest_total = 0;

int
_ctest_assert(const char *func, int line, int x)
{
	ctest_ok = ctest_ok && x;
	if (!ctest_ok)
		fprintf(stderr, "Failed assertion in '%s', line %d.\n", func, line);
	return ctest_ok;
}

void
ctest_intro()
{
	printf("Beginning tests.\n");
}

void
ctest_begin(const char *name)
{
	++ctest_total;
	ctest_ok = 1;
	printf("- Testing %s ... ", name);
}

void
ctest_end()
{
	if (ctest_ok)
		++ctest_passed;
	printf("%s\n", ctest_ok ? "pass" : "fail");
}

void
ctest_summary()
{
	printf("Total tests: %d\n", ctest_total);
	printf("Tests passed: %d\n", ctest_passed);
	printf("Tests failed: %d\n", ctest_total - ctest_passed);
}

// ----------------------------------------------------------------------------
// Tests
// ----------------------------------------------------------------------------
static const char teststr0[] = "";
static const char teststr1[] = "c";
static const char teststr2[] = "hello world";
static const char teststr3[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char teststr4[] = "\0hello world\r\n\0";
static const char teststr5[] = "lxjistriplxjimelxji";
static const char teststr6[] = "    strip me      ";

void
test_stxdel(void)
{
	stx s1;
	CTEST_BEGIN;
	s1.mem = malloc(2);
	s1.mem[0] = 1;
	s1.mem[1] = 2;
	s1.len = 1;
	s1.size = 2;
	stxdel(&s1);
	ctest_assert(NULL == s1.mem);
	ctest_assert(0 == s1.len);
	ctest_assert(0 == s1.size);
	CTEST_END;
}

void
test_stxgrow(void)
{
	stx s1;
	CTEST_BEGIN;
	memset(&s1, 0, sizeof(s1));
	ctest_assert(0 == stxgrow(&s1, 10));
	ctest_assert(NULL != s1.mem);
	ctest_assert(0 == s1.len);
	ctest_assert(10 == s1.size);

	ctest_assert(0 == stxgrow(&s1, 20));
	ctest_assert(NULL != s1.mem);
	ctest_assert(0 == s1.len);
	ctest_assert(30 == s1.size);

	ctest_assert(ESTX_OVERFLOW == stxgrow(&s1, SIZE_MAX));
	ctest_assert(NULL != s1.mem);
	ctest_assert(0 == s1.len);
	ctest_assert(30 == s1.size);
	stxdel(&s1);
	CTEST_END;
}

void
test_stxnew(void)
{
	stx s1;
	ctest_assert(0 == stxnew(&s1, 1));
	ctest_assert(NULL != s1.mem);
	ctest_assert(0 == s1.len);
	ctest_assert(1 == s1.size);
	stxdel(&s1);
}

void
test_stxensure_size(void)
{
	stx s1;
	CTEST_BEGIN;
	memset(&s1, 0, sizeof(s1));
	ctest_assert(0 == stxensure_size(&s1, 65535));
	ctest_assert(NULL != s1.mem);
	ctest_assert(0 == s1.len);
	ctest_assert(65535 == s1.size);

	ctest_assert(0 == stxensure_size(&s1, 64));
	ctest_assert(NULL != s1.mem);
	ctest_assert(0 == s1.len);
	ctest_assert(65535 == s1.size);
	stxdel(&s1);
	CTEST_END;
}

void
test_stxvalid(void)
{
	stx s1, s2, s3;
	CTEST_BEGIN;
	s1.mem = NULL;
	stxnew(&s2, 5);
	s2.len = 10;
	stxnew(&s3, 10);
	ctest_assert(false == stxvalid(&s1));
	ctest_assert(false == stxvalid(&s2));
	ctest_assert(true == stxvalid(&s3));
	stxdel(&s1);
	stxdel(&s2);
	stxdel(&s3);
	CTEST_END;
}

void
test_stxterm(void)
{
	stx s1;
	stxnew(&s1, 6);
	memset(s1.mem, 1, 6);
	CTEST_BEGIN;
	ctest_assert(0 == stxterm(&s1, 5));
	ctest_assert(5 == s1.len);
	ctest_assert(6 == s1.size);
	ctest_assert('\0' == s1.mem[s1.len]);
	CTEST_END;
	stxdel(&s1);
}

void
test_stxcpy(void)
{
	stx s1;
	memset(&s1, 0, sizeof(s1));
	CTEST_BEGIN;
	stxcpy(&s1, teststr2, sizeof(teststr2));
	ctest_assert(0 == strncmp(s1.mem, teststr2, sizeof(teststr2)));
	CTEST_END;
	stxdel(&s1);
}

int
main()
{

	ctest_intro();

	test_stxdel();
	test_stxgrow();
	test_stxnew();
	test_stxensure_size();
	test_stxvalid();
	test_stxterm();

	test_stxcpy();
	/*
	test_stxapp();
	test_stxins();

	test_stxeq();
	test_stxswap();
	test_stxtrunc();
	test_stxfind();

	test_stxrstrip();
	test_stxlstrip();
	test_stxstrip();

	test_stxslice();

	test_stxunif32();
	*/

	ctest_summary();
	return !(ctest_passed == ctest_total);
}
