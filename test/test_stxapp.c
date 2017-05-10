#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../libstx.h"
#include "test.h"

#define bytes1 "--testbytes\0\nbc\rfghij\12\0\0\n"
#define bytes2 "hello\0world\n9"

TEST_DEFINE(stxapp_mem_empty_to_full) {
	stx s1;
	stxalloc(&s1, sizeof(bytes1));

	TEST_ASSERT(&s1 == stxapp_mem(&s1, bytes1, sizeof(bytes1)));
	TEST_ASSERT(s1.mem != NULL);
	TEST_ASSERT(0 == memcmp(s1.mem, bytes1, sizeof(bytes1)));
	TEST_ASSERT(sizeof(bytes1) == s1.len);
	TEST_ASSERT(sizeof(bytes1) == s1.size);

	stxfree(&s1);

	TEST_END;
}

TEST_DEFINE(stxapp_mem_concatenate) {
	stx s1;
	char tstr[sizeof(bytes1) + sizeof(bytes2)];

	memcpy(tstr, bytes1, sizeof(bytes1));
	memcpy(tstr + sizeof(bytes1), bytes2, sizeof(bytes2));
	const size_t tlen = sizeof(bytes1) + sizeof(bytes2);

	stxalloc(&s1, sizeof(bytes1) + sizeof(bytes2));

	TEST_ASSERT(&s1 == stxapp_mem(&s1, bytes1, sizeof(bytes1)));
	TEST_ASSERT(sizeof(bytes1) == s1.len);
	TEST_ASSERT(tlen == s1.size);

	TEST_ASSERT(&s1 == stxapp_mem(&s1, bytes2, sizeof(bytes2)));
	TEST_ASSERT(s1.mem != NULL);
	TEST_ASSERT(0 == memcmp(s1.mem, tstr, tlen));
	TEST_ASSERT(tlen == s1.len);
	TEST_ASSERT(tlen == s1.size);

	stxfree(&s1);

	TEST_END;
}

TEST_DEFINE(stxapp_mem_bytes_seperately) {
	stx s1;
	stxalloc(&s1, sizeof(bytes1));

	for (size_t i=0; i<sizeof(bytes1); ++i) {
		TEST_ASSERT(&s1 == stxapp_mem(&s1, &bytes1[i], 1));
		TEST_ASSERT(s1.mem != NULL);
		TEST_ASSERT(0 == memcmp(s1.mem, bytes1, i + 1));
		TEST_ASSERT(i + 1 == s1.len);
		TEST_ASSERT(sizeof(bytes1) == s1.size);
	}

	stxfree(&s1);

	TEST_END;
}

TEST_DEFINE(stxapp_str_empty) {
	stx s1;
	stxalloc(&s1, strlen(bytes1));

	TEST_ASSERT(&s1 == stxapp_str(&s1, bytes1));
	TEST_ASSERT(s1.mem != NULL);
	TEST_ASSERT(0 == memcmp(s1.mem, bytes1, strlen(bytes1)));
	TEST_ASSERT(strlen(bytes1) == s1.len);
	TEST_ASSERT(strlen(bytes1) == s1.size);

	stxfree(&s1);

	TEST_END;
}

TEST_DEFINE(stxapp_str_concatenate) {
	stx s1;
	char tstr[strlen(bytes1) + strlen(bytes2)];

	strcpy(tstr, bytes1);
	strcpy(tstr + strlen(bytes1), bytes2);
	const size_t tlen = strlen(tstr);

	stxalloc(&s1, strlen(bytes1) + strlen(bytes2));

	TEST_ASSERT(&s1 == stxapp_str(&s1, bytes1));
	TEST_ASSERT(strlen(bytes1) == s1.len);
	TEST_ASSERT(tlen == s1.size);

	TEST_ASSERT(&s1 == stxapp_str(&s1, bytes2));
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
	TEST_ADD(ts, stxapp_mem_empty_to_full);
	TEST_ADD(ts, stxapp_mem_concatenate);
	TEST_ADD(ts, stxapp_mem_bytes_seperately);
	TEST_ADD(ts, stxapp_str_empty);
	TEST_ADD(ts, stxapp_str_concatenate);
	TEST_RUN(ts);
}
