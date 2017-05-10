#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../libstx.h"
#include "test.h"

TEST_DEFINE(stxalloc_zero)
{
	stx s1;

	TEST_ASSERT(0 == stxalloc(&s1, 0));
	TEST_ASSERT(NULL == s1.mem);
	TEST_ASSERT(0 == s1.len);
	TEST_ASSERT(0 == s1.size);

	stxfree(&s1);

	TEST_END;
}

TEST_DEFINE(stxalloc_max)
{
	stx s1;

	TEST_ASSERT(0 == stxalloc(&s1, 65535));
	TEST_ASSERT(NULL != s1.mem);
	TEST_ASSERT(0 == s1.len);
	TEST_ASSERT(65535 == s1.size);

	stxfree(&s1);

	TEST_END;
}

TEST_DEFINE(stxalloc_twice_rand) {
	stx s1;
	char *p;

	size_t n1 = test_rand(0, 65535);
	size_t n2 = test_rand(0, 65535);

	TEST_ASSERT(0 == stxalloc(&s1, n1));
	TEST_ASSERT(NULL != s1.mem);
	TEST_ASSERT(0 == s1.len);
	TEST_ASSERT(n1 == s1.size);

	p = s1.mem;

	TEST_ASSERT(0 == stxalloc(&s1, n2));
	TEST_ASSERT(p != s1.mem);
	TEST_ASSERT(NULL != s1.mem);
	TEST_ASSERT(0 == s1.len);
	TEST_ASSERT(n2 == s1.size);

	stxfree(&s1);
	free(p);

	TEST_END;
}

int
main(void)
{
	srand(time(NULL));
	TEST_INIT(ts);
	TEST_RUN(ts, stxalloc_zero);
	TEST_RUN(ts, stxalloc_max);
	TEST_RUN(ts, stxalloc_twice_rand);
	TEST_PRINT(ts);
}
