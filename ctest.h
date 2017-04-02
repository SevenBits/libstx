#ifndef CTEST_H
#define CTEST_H

#include <stdio.h>

static int ctest_ok = 1;
#define ctest_assert(x) _ctest_assert(__func__, __LINE__, (x))
#define CTEST_BEGIN ctest_begin(__func__)
#define CTEST_END ctest_end()

static int ctest_passed = 0;
static int ctest_total = 0;

static int
_ctest_assert(const char *func, int line, int x)
{
	ctest_ok = ctest_ok && x;
	if (!ctest_ok)
		fprintf(stderr, "Failed assertion in '%s', line %d.\n", func, line);
	return ctest_ok;
}

static void
ctest_intro()
{
	printf("Beginning tests.\n");
}

static void
ctest_begin(const char *name)
{
	++ctest_total;
	ctest_ok = 1;
	printf("- Testing %s ... ", name);
}

static void
ctest_end()
{
	if (ctest_ok)
		++ctest_passed;
	printf("%s\n", ctest_ok ? "pass" : "fail");
}

static void
ctest_summary()
{
	printf("Total tests: %d\n", ctest_total);
	printf("Tests passed: %d\n", ctest_passed);
	printf("Tests failed: %d\n", ctest_total - ctest_passed);
}

#endif
