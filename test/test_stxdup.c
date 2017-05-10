#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../libstx.h"
#include "test.h"

TEST_DEFINE(stxdup_mem_zero)
{
	stx s1;

	TEST_ASSERT(0 == stxdup_mem(&s1, NULL, 0));
	TEST_ASSERT(NULL == s1.mem);
	TEST_ASSERT(0 == s1.len);
	TEST_ASSERT(0 == s1.size);

	stxfree(&s1);

	TEST_END;
}

TEST_DEFINE(stxdup_mem_once)
{
	stx s1;
	char b1[test_rand(1, 512)];

	test_rand_bytes(b1, sizeof(b1));

	TEST_ASSERT(0 == stxdup_mem(&s1, b1, sizeof(b1)));
	TEST_ASSERT(NULL != s1.mem);
	TEST_ASSERT(0 == memcmp(s1.mem, b1, sizeof(b1)));
	TEST_ASSERT(sizeof(b1) == s1.len);
	TEST_ASSERT(sizeof(b1) == s1.size);

	stxfree(&s1);

	TEST_END;
}

TEST_DEFINE(stxdup_mem_twice) {
	char *p;
	stx s1;
	char b1[test_rand(1, 512)];
	char b2[test_rand(1, 512)];

	test_rand_bytes(b1, sizeof(b1));
	test_rand_bytes(b2, sizeof(b2));

	TEST_ASSERT(0 == stxdup_mem(&s1, b1, sizeof(b1)));
	TEST_ASSERT(NULL != s1.mem);
	TEST_ASSERT(0 == memcmp(s1.mem, b1, sizeof(b1)));
	TEST_ASSERT(sizeof(b1) == s1.len);
	TEST_ASSERT(sizeof(b1) == s1.size);

	p = s1.mem;

	TEST_ASSERT(0 == stxdup_mem(&s1, b2, sizeof(b2)));
	TEST_ASSERT(NULL != s1.mem);
	TEST_ASSERT(0 == memcmp(s1.mem, b2, sizeof(b2)));
	TEST_ASSERT(sizeof(b2) == s1.len);
	TEST_ASSERT(sizeof(b2) == s1.size);

	stxfree(&s1);
	free(p);

	TEST_END;
}

TEST_DEFINE(stxdup_str_zero)
{
	stx s1 = {0};

	TEST_ASSERT(0 == stxdup_str(&s1, NULL));
	TEST_ASSERT(NULL == s1.mem);
	TEST_ASSERT(0 == s1.len);
	TEST_ASSERT(0 == s1.size);

	stxfree(&s1);

	TEST_END;
}

TEST_DEFINE(stxdup_str_once)
{
	stx s1;
	char b1[test_rand(1, 512)];

	test_rand_str(b1, sizeof(b1));

	TEST_ASSERT(0 == stxdup_str(&s1, b1));
	TEST_ASSERT(NULL != s1.mem);
	TEST_ASSERT(0 == memcmp(s1.mem, b1, strlen(b1)));
	TEST_ASSERT(strlen(b1) == s1.len);
	TEST_ASSERT(strlen(b1) == s1.size);

	stxfree(&s1);

	TEST_END;
}

TEST_DEFINE(stxdup_str_twice) {
	char *p;
	stx s1;
	char b1[test_rand(1, 512)];
	char b2[test_rand(1, 512)];

	test_rand_str(b1, sizeof(b1));
	test_rand_str(b2, sizeof(b2));

	TEST_ASSERT(0 == stxdup_str(&s1, b1));
	TEST_ASSERT(NULL != s1.mem);
	TEST_ASSERT(0 == memcmp(s1.mem, b1, strlen(b1)));
	TEST_ASSERT(strlen(b1) == s1.len);
	TEST_ASSERT(strlen(b1) == s1.size);

	p = s1.mem;

	TEST_ASSERT(0 == stxdup_str(&s1, b2));
	TEST_ASSERT(NULL != s1.mem);
	TEST_ASSERT(0 == memcmp(s1.mem, b2, strlen(b2)));
	TEST_ASSERT(strlen(b2) == s1.len);
	TEST_ASSERT(strlen(b2) == s1.size);

	stxfree(&s1);
	free(p);

	TEST_END;
}

int
main(void)
{
	srand(time(NULL));
	TEST_INIT(ts);
	TEST_RUN(ts, stxdup_mem_zero);
	TEST_RUN(ts, stxdup_mem_once);
	TEST_RUN(ts, stxdup_mem_twice);
	TEST_RUN(ts, stxdup_str_zero);
	TEST_RUN(ts, stxdup_str_once);
	TEST_RUN(ts, stxdup_str_twice);
	TEST_PRINT(ts);
}
