#include "oystr.h"

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

static int ctest_ret = 0;
#define ctest_assert(x) (ctest_ret = ctest_ret && (x))
#define CTEST_START (ctest_start(__func__))
#define CTEST_END (ctest_end())

static int ctest_passed = 0;
static int ctest_total = 0;

struct ctest {
	int ret;
	int passed;
	int total;
};

void
ctest_intro()
{
	printf("Beginning tests.\n");
}

void
ctest_start(const char *name)
{
	++ctest_total;
	ctest_ret = 1;
	printf("- Testing %s ... ", name);
}

void
ctest_end()
{
	if (ctest_ret)
		++ctest_passed;
	printf("%s\n", ctest_ret ? "pass" : "fail");
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

void
test_oystr_init()
{
	struct oystr s1;

	CTEST_START;
	oystr_init(&s1);
	ctest_assert(NULL == s1.buf);
	ctest_assert(0 == s1.len);
	ctest_assert(0 == s1.size);
	CTEST_END;
}

void
test_oystr_deinit()
{
	struct oystr s1;
	s1.buf = malloc(1);
	s1.buf[0] = 1;

	CTEST_START;
	oystr_deinit(&s1);
	ctest_assert(NULL == s1.buf);
	ctest_assert(0 == s1.len);
	ctest_assert(0 == s1.size);
	CTEST_END;
}

void
test_oystr_init_buf()
{
	int r;
	int i;
	struct oystr s1;

	CTEST_START;
	for (i=0; i<100; ++i) {
		// 0 as a random value should be ok here.
		r = rand() % 65536;
		int ret;
		ret = oystr_init_buf(&s1, r);
		ret = ret && ctest_assert(s1.buf);
		ret = ret && ctest_assert(0 == s1.len);
		ret = ret && ctest_assert(r <= s1.size);
		if (!ret) {
			// Quit upon first failure
			break;
		}
		oystr_deinit(&s1);
	}
	CTEST_END;
}

void
test_oystr_valid()
{
	struct oystr *s1 = NULL;
	struct oystr s2;

	assert(!oystr_init_buf(&s2, 10));

	CTEST_START;
	ctest_assert(0 > oystr_valid(s1));
	ctest_assert(!oystr_valid(&s2));
	CTEST_END;

	oystr_deinit(&s2);
}

int
test_oystr_set_len()
{
	int i;
	int ret;
	struct oystr s1;
	assert(!oystr_init_buf(&s1, 20));
	memset(s1.buf, 1, 20);

	CTEST_START;
	for (i=0; i<20; ++i) {
		ret = ctest_assert(!oystr_set_len(&s1, i));
		ret = ctest_assert(i == s1.len);
		ret = ctest_assert('\0' == s1.buf[i]);
		ret = ctest_assert(20 <= s1.size);
		if (!ret) {
			break;
		}
	}
	CTEST_END;

	oystr_deinit(&s1);
}

int
main()
{
	ctest_intro();

	test_oystr_init();
	test_oystr_deinit();
	test_oystr_init_buf();
	test_oystr_valid();
	test_oystr_set_len();

	ctest_summary();
	return !(ctest_passed == ctest_total);
}
