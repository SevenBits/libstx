#include "libstx.h"

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

static int ctest_ok = 1;
#define ctest_assert(x) _ctest_assert(__func__, __LINE__, (x))
#define CTEST_BEGIN (ctest_begin(__func__))
#define CTEST_END (ctest_end())

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
// Begin unit test section
// ----------------------------------------------------------------------------

static char teststr0[] = "";
static char teststr1[] = "c";
static char teststr2[] = "hello world";
static char teststr3[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static char teststr4[] = "\0hello world\r\n\0";
static char teststr5[] = "lxjistriplxjimelxji";
static char teststr6[] = "    strip me      ";

void
test_stxdel()
{
	stx s1;
	s1.mem = malloc(2);
	s1.mem[0] = 1;
	s1.mem[1] = 2;
	s1.len = 1;
	s1.size = 2;

	CTEST_BEGIN;
	stxdel(&s1);
	ctest_assert(NULL == s1.mem);
	ctest_assert(0 == s1.len);
	ctest_assert(0 == s1.size);
	CTEST_END;
}

void
test_stxgrow()
{
	int i;
	int r;
	stx s1;

	memset(&s1, 0, sizeof(s1));
	CTEST_BEGIN;
	for (i = 1; i < 1025; ++i) {
		// 0 as a random value is NOT ok.
		r = 1 + (rand() % 65535);
		ctest_assert(0 == stxgrow(&s1, r));
		ctest_assert(NULL != s1.mem);
		ctest_assert(0 == s1.len);
		ctest_assert(r == s1.size);
		if (!ctest_ok)
			break;
		stxdel(&s1);
	}
	CTEST_END;
}

test_stxensure_size()
{
	int i;
	int r;
	stx s1;

	memset(&s1, 0, sizeof(s1));
	CTEST_BEGIN;
	for (i = 1; i < 1025; ++i) {
		// 0 as a random value is NOT ok.
		r = 1 + (rand() % 65535);
		ctest_assert(0 == stxensure_size(&s1, r));
		ctest_assert(NULL != s1.mem);
		ctest_assert(0 == s1.len);
		ctest_assert(r <= s1.size);
		if (!ctest_ok)
			break;
	}
	stxdel(&s1);
	CTEST_END;
}

void
test_stxnew()
{
	int i;
	int r;
	stx s1;

	CTEST_BEGIN;
	for (i = 1; i < 1025; ++i) {
		// 0 as a random value is NOT ok.
		r = 1 + (rand() % 65535);
		ctest_assert(0 == stxnew(&s1, i));
		ctest_assert(NULL != s1.mem);
		ctest_assert(0 == s1.len);
		ctest_assert(i == s1.size);
		stxdel(&s1);
		if (!ctest_ok)
			break;
		stxdel(&s1);
	}
	CTEST_END;
}

void
test_stxvalid()
{
	stx *s1 = NULL;
	stx s2;

	assert(0 == stxnew(&s2, 10));

	CTEST_BEGIN;
	ctest_assert(false == stxvalid(s1));
	ctest_assert(true == stxvalid(&s2));
	CTEST_END;

	stxdel(&s2);
}

/*
void
test_stxterm()
{
	int i;
	int ret;
	stx s1;
	assert(0 == oystr_init_buf(&s1, 20));
	memset(s1.buf, 1, 20);

	CTEST_BEGIN;
	for (i=0; i<20; ++i) {
		ctest_assert(0 == oystr_setlen(&s1, i));
		ctest_assert(i == s1.len);
		ctest_assert('\0' == s1.buf[i]);
		ctest_assert(20 <= s1.size);
		if (!ctest_ret)
			break;
	}
	CTEST_END;

	oystr_deinit(&s1);
}

void
test_stxcpy()
{
	int i;
	int ret;
	stx s1;
	oystr_init(&s1);
	CTEST_BEGIN;
	for (i=0; i<11; ++i) {
		ctest_assert(0 == oystr_assign(&s1, "hello world", i));
		ctest_assert(0 == strncmp(s1.buf, "hello world", i));
		ctest_assert(i == s1.len);
		ctest_assert(i <= s1.size);
		if (!ctest_ret)
			break;
	}
	CTEST_END;

	oystr_deinit(&s1);
}

void
test_stxapp()
{
	stx s1;
	oystr_init(&s1);
	assert(0 == oystr_assign(&s1, "hello ", 6));
	CTEST_BEGIN;
	ctest_assert(0 == oystr_append(&s1, "world", 5));
	ctest_assert(0 == strcmp(s1.buf, "hello world"));
	ctest_assert(11 == s1.len);
	ctest_assert(11 <=  s1.size);
	CTEST_END;
	oystr_deinit(&s1);
}

void
test_stxins()
{
	const char initstr[] = "pumpkin squash is good dude.";
	stx s1;
	oystr_init(&s1);
	assert(0 == oystr_assign(&s1, initstr, sizeof(initstr) - 1));
	CTEST_BEGIN;
	ctest_assert(0 == oystr_insert(&s1, 15, "beer ", 5));
	ctest_assert(0 == strcmp(s1.buf, "pumpkin squash beer is good dude."));
	ctest_assert(sizeof(initstr) - 1 + 5 == s1.len);
	ctest_assert(sizeof(initstr) - 1 + 5 <= s1.size);
	CTEST_END;
	oystr_deinit(&s1);
}

void
test_oystr_eq()
{
	stx s1;
	stx s2;
	stx s3;
	oystr_init(&s1);
	oystr_init(&s2);
	oystr_init(&s3);
	assert(0 == oystr_assign(&s1, "hello", 5));
	assert(0 == oystr_assign(&s2, "hello", 5));
	assert(0 == oystr_assign(&s3, "wickedpissah", 12));
	CTEST_BEGIN;
	ctest_assert(true == oystr_eq(&s1, &s1));
	ctest_assert(true == oystr_eq(&s1, &s2));
	ctest_assert(false == oystr_eq(&s2, &s3));
	CTEST_END;
	oystr_deinit(&s1);
	oystr_deinit(&s2);
	oystr_deinit(&s3);
}

void
test_oystr_swap()
{
	stx s1;
	stx s2;
	oystr_init(&s1);
	oystr_init(&s2);
	assert(0 == oystr_assign(&s1, "buffer_one", 10));
	assert(0 == oystr_assign(&s2, "buf_2", 5));
	CTEST_BEGIN;
	oystr_swap(&s1, &s2);

	ctest_assert(0 == strcmp(s1.buf, "buf_2"));
	ctest_assert(5 == s1.len);
	ctest_assert(5 <= s1.size);

	ctest_assert(0 == strcmp(s2.buf, "buffer_one"));
	ctest_assert(10 == s2.len);
	ctest_assert(10 <= s2.size);
	CTEST_END;
	oystr_deinit(&s1);
	oystr_deinit(&s2);
}

void
test_oystr_trunc()
{
	const char tstring[] = "truncateme";
	int i;
	stx s1;
	oystr_init(&s1);
	assert(0 == oystr_assign(&s1, tstring, 10));
	CTEST_BEGIN;
	for (i=1; i<=10; ++i) {
		ctest_assert(tstring[10-i] == oystr_trunc(&s1, 1));
		ctest_assert(10 - i == s1.len);
		ctest_assert(11 <= s1.size);
		if (ctest_ret)
			break;
	}
	ctest_assert('\0' == oystr_trunc(&s1, 20));
	CTEST_END;
	oystr_deinit(&s1);
}

void
test_oystr_find()
{
	stx slice;
	stx s1;
	oystr_init(&s1);
	oystr_assign(&s1, "hello world", 11);
	CTEST_BEGIN;
	ctest_assert(true == oystr_find(&slice, &s1, "world", 5));
	ctest_assert(5 == slice.len);
	ctest_assert(5 <= slice.size);
	ctest_assert(0 == strncmp(slice.buf, "world", slice.len));

	oystr_init(&slice);

	ctest_assert(false == oystr_find(&slice, &s1, "hello", 6));
	ctest_assert(0 == slice.len);
	ctest_assert(0 <= slice.size);
	ctest_assert(NULL == slice.buf);

	ctest_assert(false == oystr_find(&slice, &s1, "world ", 6));
	ctest_assert(0 == slice.len);
	ctest_assert(0 <= slice.size);
	ctest_assert(NULL == slice.buf);
	CTEST_END;
	oystr_deinit(&s1);
}

void
test_oystr_rstrip()
{
	const char initstr[] = "mmnnlo??STRIPm>?lnonnmm";
	stx s1;
	oystr_init(&s1);
	assert(0 == oystr_assign(&s1, initstr, sizeof(initstr) - 1));
	CTEST_BEGIN;
	ctest_assert(10 == oystr_rstrip(&s1, "m>?lno", 6));
	ctest_assert(0 == strcmp(s1.buf, "mmnnlo??STRIP"));
	ctest_assert(13 == s1.len);
	CTEST_END;
	oystr_deinit(&s1);
}

void
test_oystr_lstrip()
{
	const char initstr[] = "zzannlo~>STRIPmozzannmmaz";
	stx s1;
	oystr_init(&s1);
	assert(0 == oystr_assign(&s1, initstr, sizeof(initstr) - 1));
	CTEST_BEGIN;
	ctest_assert(9 == oystr_lstrip(&s1, "zanlo~>", 7));
	ctest_assert(0 == strcmp(s1.buf, "STRIPmozzannmmaz"));
	ctest_assert(16 == s1.len);
	CTEST_END;
	oystr_deinit(&s1);
}

void
test_oystr_strip()
{
	const char initstr[] = "zzanloSTRIPm&?lnonnmm";
	stx s1;
	oystr_init(&s1);
	assert(0 == oystr_assign(&s1, initstr, sizeof(initstr) - 1));
	CTEST_BEGIN;
	ctest_assert(16 == oystr_strip(&s1, "zanlom&?", 8));
	ctest_assert(0 == strcmp(s1.buf, "STRIP"));
	ctest_assert(5 == s1.len);
	CTEST_END;
	oystr_deinit(&s1);
}

void
test_oystr_slice()
{
	stx slice;
	stx s1;
	oystr_init(&s1);
	assert(0 == oystr_assign(&s1, "slicemeplease", 13));
	CTEST_BEGIN;
	oystr_slice(&slice, &s1, 0, 5);
	ctest_assert(5 == slice.len);
	ctest_assert(5 <= slice.size);
	ctest_assert(0 == strncmp(slice.buf, "slice", slice.len));
	CTEST_END;
	oystr_deinit(&s1);
}

void
test_oystr_utf8_from_utf32()
{
	int i;
	stx s1;
	char bytes[4];

	oystr_init_buf(&s1, 12);
	CTEST_BEGIN;
	// Simple case for ASCII
	for (i=0; i<127; ++i) {
		ctest_assert(1 == oystr_utf8_from_utf32(bytes, i));
		ctest_assert(bytes[0] == i);
		if (!ctest_ret)
			goto failure;
	}
	// 2-byte utf8
	for (i=161; i<0x800; ++i) {
		ctest_assert(2 == oystr_utf8_from_utf32(bytes, i));
		if (!ctest_ret)
			goto failure;
	}

	// 3-byte utf8
	for (;;) {
		ctest_assert(4 == oystr_utf8_from_utf32(bytes, i));
		if (!ctest_ret)
			goto failure;
	}
	// 4-byte utf8
	for (;;) {
		ctest_assert(4 == oystr_utf8_from_utf32(bytes, i));
		if (!ctest_ret)
			goto failure;
	}
failure:
	CTEST_END;
	oystr_deinit(&s1);
}
*/

int
main()
{
	ctest_intro();

	test_stxdel();
	test_stxgrow();
	test_stxensure_size();
	test_stxnew();
	test_stxvalid();
	/*
	test_stxterm();

	test_stxcpy();
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
