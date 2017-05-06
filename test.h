#ifndef test_H
#define test_H

#include <stdio.h>

static int test_ok = 1;
#define TEST_ASSERT(x) test_assert(__func__, __LINE__, (x))
#define TEST_BEGIN test_begin(__func__); do 
#define TEST_END while(0); test_end()

static int test_passed = 0;
static int test_total = 0;

static int
test_assert(const char *func, int line, int x)
{
	test_ok = test_ok && x;
	if (!test_ok)
		fprintf(stderr, "Failed assertion in '%s', line %d.\n", func, line);
	return test_ok;
}

static void
test_intro()
{
	printf("Beginning tests.\n");
}

static void
test_begin(const char *name)
{
	++test_total;
	test_ok = 1;
	printf("- Testing %s ... ", name);
}

static void
test_end()
{
	if (test_ok)
		++test_passed;
	printf("%s\n", test_ok ? "pass" : "fail");
}

static void
test_summary()
{
	printf("Total tests: %d\n", test_total);
	printf("Tests passed: %d\n", test_passed);
	printf("Tests failed: %d\n", test_total - test_passed);
}

#endif
