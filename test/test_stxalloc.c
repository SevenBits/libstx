#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

TEST_DEFINE(stxalloc_twice) {
	stx s1;
	char *p;

	TEST_ASSERT(0 == stxalloc(&s1, 65535));
	TEST_ASSERT(NULL != s1.mem);
	TEST_ASSERT(0 == s1.len);
	TEST_ASSERT(65535 == s1.size);

	p = s1.mem;

	stxalloc(&s1, 65535);

	TEST_ASSERT(NULL != s1.mem);
	TEST_ASSERT(0 == s1.len);
	TEST_ASSERT(65535 == s1.size);
	TEST_ASSERT(p != s1.mem);

	stxfree(&s1);
	free(p);

	TEST_END;
}

int
main(void)
{
	TEST_INIT(ts);
	TEST_ADD(ts, stxalloc_zero);
	TEST_ADD(ts, stxalloc_twice);
	TEST_RUN(ts);
}
