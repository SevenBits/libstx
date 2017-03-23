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
	ctest_assert(stxdel(&s1) == &s1);
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
	ctest_assert(stxgrow(&s1, 10) == &s1);
	ctest_assert(NULL != s1.mem);
	ctest_assert(0 == s1.len);
	ctest_assert(10 == s1.size);

	ctest_assert(stxgrow(&s1, 20) == &s1);
	ctest_assert(NULL != s1.mem);
	ctest_assert(0 == s1.len);
	ctest_assert(30 == s1.size);
	CTEST_END;
	stxdel(&s1);
}

void
test_stxnew(void)
{
	stx s1;
	CTEST_BEGIN;
	ctest_assert(stxnew(&s1, 1) == &s1);
	ctest_assert(NULL != s1.mem);
	ctest_assert(0 == s1.len);
	ctest_assert(1 == s1.size);
	CTEST_END;
	stxdel(&s1);
}

void
test_stxensure_size(void)
{
	stx s1;
	CTEST_BEGIN;
	memset(&s1, 0, sizeof(s1));
	ctest_assert(stxensure_size(&s1, 65535) == &s1);
	ctest_assert(NULL != s1.mem);
	ctest_assert(0 == s1.len);
	ctest_assert(65535 == s1.size);

	ctest_assert(stxensure_size(&s1, 64) == &s1);
	ctest_assert(NULL != s1.mem);
	ctest_assert(0 == s1.len);
	ctest_assert(65535 == s1.size);
	CTEST_END;
	stxdel(&s1);
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
	ctest_assert(NULL == stxvalid(&s1));
	ctest_assert(NULL == stxvalid(&s2));
	ctest_assert(&s3 == stxvalid(&s3));
	stxdel(&s1);
	stxdel(&s2);
	stxdel(&s3);
	CTEST_END;
}

void
test_stxcpy(void)
{
	stx s1 = {0};
	stxgrow(&s1, sizeof(teststr2));
	CTEST_BEGIN;
	ctest_assert(stxcpy(&s1, teststr2, sizeof(teststr2)) == &s1);
	ctest_assert(0 == memcmp(s1.mem, teststr2, sizeof(teststr2)));
	CTEST_END;
	stxdel(&s1);
}

void
test_stxapp(void)
{
	stx s1 = {0};
	stxgrow(&s1, sizeof(teststr2) - 1);
	CTEST_BEGIN;
	ctest_assert(stxapp(&s1, teststr2, sizeof(teststr2) - 1) == &s1);
	ctest_assert(sizeof(teststr2) - 1 == s1.size);
	ctest_assert(sizeof(teststr2) - 1 == s1.len);
	ctest_assert(0 == memcmp(s1.mem, teststr2, sizeof(teststr2) - 1));

	stxgrow(&s1, sizeof("end"));
	ctest_assert(stxapp(&s1, "end", sizeof("end")) == &s1);
	ctest_assert(sizeof(teststr2) - 1 + sizeof("end") == s1.size);
	ctest_assert(sizeof(teststr2) - 1 + sizeof("end") == s1.len);
	ctest_assert(0 == memcmp(s1.mem, teststr2, sizeof(teststr2) - 1));
	ctest_assert(0 == memcmp(s1.mem + sizeof(teststr2) - 1,
				"end", sizeof("end")));
	CTEST_END;
	stxdel(&s1);
}

void
test_stxins(void)
{
	stx s1 = {0};
	stxgrow(&s1, sizeof(teststr3) + sizeof(teststr2) - 1);
	stxcpy(&s1, teststr3, sizeof(teststr3));
	CTEST_BEGIN;
	ctest_assert(&s1 == stxins(&s1, 10, teststr2, sizeof(teststr2) - 1));
	ctest_assert(0 == memcmp(s1.mem + 10, teststr2, sizeof(teststr2) - 1));
	ctest_assert(0 == memcmp(s1.mem, teststr3, 10));
	//ctest_assert(0 == memcmp(s1.mem + 10 + sizeof(teststr2) - 1, teststr3 + 10 + sizeof(teststr2), sizeof(teststr3) - 10 ));
	CTEST_END;
	stxdel(&s1);
}

void
test_stxeq()
{
	stx s1, s2, s3;
	stxcpy(stxnew(&s1, sizeof(teststr4)), teststr4, sizeof(teststr4));
	stxcpy(stxnew(&s2, sizeof(teststr4)), teststr4, sizeof(teststr4));
	stxcpy(stxnew(&s3, sizeof(teststr4)), teststr2, sizeof(teststr2));
	CTEST_BEGIN;
	ctest_assert(true == stxeq(&s1, &s2));
	ctest_assert(false == stxeq(&s1, &s3));
	CTEST_END;
	stxdel(&s1);
	stxdel(&s2);
	stxdel(&s3);
}

void
test_stxtrunc()
{
	stx s1;
	stxcpy(stxnew(&s1, 4), "wall", 4);
	CTEST_BEGIN;
	ctest_assert(&s1 == stxtrunc(&s1, 2));
	ctest_assert(2 == s1.len);
	ctest_assert(4 == s1.size);
	ctest_assert(0 == strncmp(s1.mem, "wa", 2));
	CTEST_END;
	stxdel(&s1);
}

void
test_stxslice()
{
	stx s1, slice;
	stxcpy(stxnew(&s1, sizeof(teststr3)), teststr3, sizeof(teststr3));
	CTEST_BEGIN;
	ctest_assert(&slice == stxslice(&slice, &s1, 4, 6));
	ctest_assert(2 == slice.len);
	ctest_assert(sizeof(teststr3) - 4 == slice.size);
	ctest_assert(s1.mem + 4 == slice.mem);
	CTEST_END;
	stxdel(&s1);
}

void
test_stxfind()
{
	stx s1, slice;
	stxcpy(stxnew(&s1, sizeof(teststr4)), teststr4, sizeof(teststr4));
	CTEST_BEGIN;
	ctest_assert(&slice == stxfind(&slice, &s1, "world", 5));
	ctest_assert(5 == slice.len);
	ctest_assert(s1.size >= slice.size);
	ctest_assert(0 == strncmp(slice.mem, "world", 5));
	CTEST_END;
	stxdel(&s1);
}

void
test_compose()
{
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

	test_stxcpy();
	test_stxapp();
	test_stxins();

	test_stxeq();
	test_stxtrunc();
	test_stxslice();
	test_stxfind();

	/*
	test_stxrstrip();
	test_stxlstrip();
	test_stxstrip();


	test_stxunif32();
	test_compose();
	*/

	ctest_summary();
	return !(ctest_passed == ctest_total);
}
