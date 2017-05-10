#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../libstx.h"
#include "test.h"

TEST_DEFINE(stxapp_mem_zero)
{
	stx s1 = {0};

	TEST_ASSERT(&s1 == stxapp_mem(&s1, NULL, 0));
	TEST_ASSERT(s1.mem == NULL);
	TEST_ASSERT(0 == s1.len);
	TEST_ASSERT(0 == s1.size);

	TEST_END;
}

TEST_DEFINE(stxapp_mem_once)
{
	stx s1;
	char b1[test_rand(1, 512)];

	test_rand_bytes(b1, sizeof(b1));
	stxalloc(&s1, sizeof(b1));

	TEST_ASSERT(&s1 == stxapp_mem(&s1, b1, sizeof(b1)));
	TEST_ASSERT(s1.mem != NULL);
	TEST_ASSERT(0 == memcmp(s1.mem, b1, sizeof(b1)));
	TEST_ASSERT(sizeof(b1) == s1.len);
	TEST_ASSERT(sizeof(b1) == s1.size);

	stxfree(&s1);

	TEST_END;
}

TEST_DEFINE(stxapp_mem_bytes_seperately)
{
	stx s1;
	char b1[test_rand(1, 512)];
	char b2[test_rand(1, 512)];

	test_rand_bytes(b1, sizeof(b1));
	test_rand_bytes(b2, sizeof(b2));

	stxalloc(&s1, sizeof(b1));

	for (size_t i=0; i<sizeof(b1); ++i) {
		TEST_ASSERT(&s1 == stxapp_mem(&s1, &b1[i], 1));
		TEST_ASSERT(s1.mem != NULL);
		TEST_ASSERT(0 == memcmp(s1.mem, b1, i + 1));
		TEST_ASSERT(i + 1 == s1.len);
		TEST_ASSERT(sizeof(b1) == s1.size);
	}

	stxfree(&s1);

	TEST_END;
}

TEST_DEFINE(stxapp_mem_twice)
{
	stx s1;
	char b1[test_rand(1, 512)];
	char b2[test_rand(1, 512)];
	char tstr[sizeof(b1) + sizeof(b2)];

	test_rand_bytes(b1, sizeof(b1));
	test_rand_bytes(b2, sizeof(b2));
	memcpy(tstr, b1, sizeof(b1));
	memcpy(tstr + sizeof(b1), b2, sizeof(b2));
	const size_t tlen = sizeof(b1) + sizeof(b2);

	stxalloc(&s1, sizeof(b1) + sizeof(b2));

	TEST_ASSERT(&s1 == stxapp_mem(&s1, b1, sizeof(b1)));
	TEST_ASSERT(sizeof(b1) == s1.len);
	TEST_ASSERT(tlen == s1.size);

	TEST_ASSERT(&s1 == stxapp_mem(&s1, b2, sizeof(b2)));
	TEST_ASSERT(s1.mem != NULL);
	TEST_ASSERT(0 == memcmp(s1.mem, tstr, tlen));
	TEST_ASSERT(tlen == s1.len);
	TEST_ASSERT(tlen == s1.size);

	stxfree(&s1);

	TEST_END;
}

TEST_DEFINE(stxapp_str_zero)
{
	stx s1 = {0};

	TEST_ASSERT(&s1 == stxapp_str(&s1, NULL));
	TEST_ASSERT(s1.mem == NULL);
	TEST_ASSERT(0 == s1.len);
	TEST_ASSERT(0 == s1.size);

	TEST_END;
}

TEST_DEFINE(stxapp_str_once)
{
	stx s1;
	char b1[test_rand(1, 512)];

	test_rand_str(b1, sizeof(b1));
	stxalloc(&s1, strlen(b1));

	TEST_ASSERT(&s1 == stxapp_str(&s1, b1));
	TEST_ASSERT(s1.mem != NULL);
	TEST_ASSERT(0 == memcmp(s1.mem, b1, strlen(b1)));
	TEST_ASSERT(strlen(b1) == s1.len);
	TEST_ASSERT(strlen(b1) == s1.size);

	stxfree(&s1);

	TEST_END;
}

TEST_DEFINE(stxapp_str_twice)
{
	stx s1;
	char b1[test_rand(1, 512)];
	char b2[test_rand(1, 512)];
	char tstr[sizeof(b1) + sizeof(b2)];

	test_rand_str(b1, sizeof(b1));
	test_rand_str(b2, sizeof(b2));

	strcpy(tstr, b1);
	strcpy(tstr + strlen(b1), b2);
	const size_t tlen = strlen(tstr);
	stxalloc(&s1, strlen(b1) + strlen(b2));

	TEST_ASSERT(&s1 == stxapp_str(&s1, b1));
	TEST_ASSERT(strlen(b1) == s1.len);
	TEST_ASSERT(tlen == s1.size);

	TEST_ASSERT(&s1 == stxapp_str(&s1, b2));
	TEST_ASSERT(s1.mem != NULL);
	TEST_ASSERT(0 == memcmp(s1.mem, tstr, tlen));
	TEST_ASSERT(tlen == s1.len);
	TEST_ASSERT(tlen == s1.size);

	stxfree(&s1);

	TEST_END;
}

int
main(void)
{
	TEST_INIT(ts);
	TEST_RUN(ts, stxapp_mem_zero);
	TEST_RUN(ts, stxapp_mem_once);
	TEST_RUN(ts, stxapp_mem_bytes_seperately);
	TEST_RUN(ts, stxapp_mem_twice);
	TEST_RUN(ts, stxapp_str_zero);
	TEST_RUN(ts, stxapp_str_once);
	TEST_RUN(ts, stxapp_str_twice);
	TEST_PRINT(ts);
}
