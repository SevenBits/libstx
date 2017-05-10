#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../libstx.h"
#include "test.h"

#define b1 "--testbytes\0\nbc\rfghij\12\0\0\n"
#define b2 "hello\0world\n9"

TEST_DEFINE(stxcpy_mem_empty)
{
	stx s1;
	stxalloc(&s1, sizeof(b1));

	TEST_ASSERT(&s1 == stxcpy_mem(&s1, b1, sizeof(b1)));
	TEST_ASSERT(s1.mem != NULL);
	TEST_ASSERT(0 == memcmp(s1.mem, b1, sizeof(b1)));
	TEST_ASSERT(sizeof(b1) == s1.len);
	TEST_ASSERT(sizeof(b1) == s1.size);

	stxfree(&s1);

	TEST_END;
}

TEST_DEFINE(stxcpy_mem_twice)
{
	stx s1;

	size_t n = sizeof(b1) > sizeof(b2) ? sizeof(b1) : sizeof(b2);
	stxalloc(&s1, n);

	TEST_ASSERT(&s1 == stxcpy_mem(&s1, b1, sizeof(b1)));
	TEST_ASSERT(s1.mem != NULL);
	TEST_ASSERT(0 == memcmp(s1.mem, b1, sizeof(b1)));
	TEST_ASSERT(sizeof(b1) == s1.len);
	TEST_ASSERT(n == s1.size);

	TEST_ASSERT(&s1 == stxcpy_mem(&s1, b2, sizeof(b2)));
	TEST_ASSERT(s1.mem != NULL);
	TEST_ASSERT(0 == memcmp(s1.mem, b2, sizeof(b2)));
	TEST_ASSERT(sizeof(b2) == s1.len);
	TEST_ASSERT(n == s1.size);

	stxfree(&s1);

	TEST_END;
}

TEST_DEFINE(stxcpy_str_empty)
{
	stx s1;
	stxalloc(&s1, strlen(b1));

	TEST_ASSERT(&s1 == stxcpy_str(&s1, b1));
	TEST_ASSERT(s1.mem != NULL);
	TEST_ASSERT(0 == memcmp(s1.mem, b1, strlen(b1)));
	TEST_ASSERT(strlen(b1) == s1.len);
	TEST_ASSERT(strlen(b1) == s1.size);

	stxfree(&s1);

	TEST_END;
}

TEST_DEFINE(stxcpy_str_twice)
{
	stx s1;

	size_t n = strlen(b1) > strlen(b2) ? strlen(b1) : strlen(b2);
	stxalloc(&s1, n);

	TEST_ASSERT(&s1 == stxcpy_str(&s1, b1));
	TEST_ASSERT(s1.mem != NULL);
	TEST_ASSERT(0 == memcmp(s1.mem, b1, strlen(b2)));
	TEST_ASSERT(strlen(b1) == s1.len);
	TEST_ASSERT(n == s1.size);

	TEST_ASSERT(&s1 == stxcpy_str(&s1, b2));
	TEST_ASSERT(s1.mem != NULL);
	TEST_ASSERT(0 == memcmp(s1.mem, b2, strlen(b2)));
	TEST_ASSERT(strlen(b2) == s1.len);
	TEST_ASSERT(n == s1.size);

	stxfree(&s1);

	TEST_END;
}

int
main(void)
{
	TEST_INIT(ts);
	TEST_RUN(ts, stxcpy_mem_empty);
	TEST_RUN(ts, stxcpy_mem_twice);
	TEST_RUN(ts, stxcpy_str_empty);
	TEST_RUN(ts, stxcpy_str_twice);
	TEST_PRINT(ts);
}
