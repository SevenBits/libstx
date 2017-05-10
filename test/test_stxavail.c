#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../libstx.h"
#include "test.h"

TEST_DEFINE(stxavail_none)
{
	stx s1 = {0};
	TEST_ASSERT(0 == stxavail(&s1));
	return 0;
}

TEST_DEFINE(stxavail_full) {
	stx s1 = {
		.mem = NULL,
		.len = 1024,
		.size = 1024,
	};

	TEST_ASSERT(0 == stxavail(&s1));

	return 0;
}

TEST_DEFINE(stxavail_half) {
	stx s1 = {
		.mem = NULL,
		.len = 512,
		.size = 1024,
	};

	TEST_ASSERT(512 == stxavail(&s1));

	return 0;
}

TEST_DEFINE(stxavail_non_powerof_two) {
	stx s1 = {
		.mem = NULL,
		.len = 238,
		.size = 745,
	};

	TEST_ASSERT(745 - 238 == stxavail(&s1));

	return 0;
}

int
main(void)
{
	TEST_INIT(ts);
	TEST_RUN(ts, stxavail_none);
	TEST_RUN(ts, stxavail_full);
	TEST_RUN(ts, stxavail_half);
	TEST_RUN(ts, stxavail_non_powerof_two);
	TEST_PRINT(ts);
}
