#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "libistr.h"

// Currently unused macro
#define istr_assert(a) ((a) ? :glb_test_fail++)

static int glb_test_fail = 0;

static int test_new_and_free()
{
	istring *is1 = istr_new(NULL);
	assert(NULL != is1);
	assert(0 == istr_len(is1));
	assert(2 == istr_size(is1));
	assert('\0' == is1[0]);
	istr_free(is1);

	is1 = istr_new_bytes("hello", 5);
	assert(NULL != is1);
	assert(5 == istr_len(is1));
	assert(5 <= istr_size(is1));
	assert('\0' == is1[5]);
	assert(0 == strcmp(is1, "hello"));
	istr_free(is1);

	is1 = istr_new_cstr("hello world");
	assert(NULL != is1);
	assert(11 == istr_len(is1));
	assert(11 <= istr_size(is1));
	assert('\0' == is1[11]);
	assert(0 == strcmp(is1, "hello world"));

	istring *is2 = istr_new(is1);
	istr_free(is1);

	assert(NULL != is2);
	assert(11 == istr_len(is2));
	assert(11 <= istr_size(is2));
	assert('\0' == is2[11]);
	assert(0 == strcmp(is2, "hello world"));
	istr_free(is2);
}

static int test_grow_and_shrink()
{
	istring *is1 = istr_new(NULL);
	is1 = istr_grow(is1, 64);
	assert(0 == istr_len(is1));
	assert(64 <= istr_size(is1));
	assert('\0' == is1[0]);

	is1 = istr_shrink(is1, 16);
	assert(0 == istr_len(is1));
	assert(16 <= istr_size(is1));
	assert(64 >= istr_size(is1));
	assert('\0' == is1[0]);

	istr_free(is1);
}

static int test_eq()
{
	istring *is1 = istr_new_cstr("hello");
	istring *is2 = istr_new_cstr("hello");
	assert(0 == istr_eq(is1, is2));
	is2[4] = 'x';
	assert(1 == istr_eq(is1, is2));

	assert(-1 == istr_eq(NULL, NULL));

	istr_free(is1);
	istr_free(is2);
}

static int test_assign()
{
	istring *is1 = istr_new(NULL);
	assert(NULL == istr_assign(is1, NULL));

	is1 = istr_assign_bytes(is1, "hello", 5);

	assert(NULL != is1);
	assert(5 == istr_len(is1));
	assert(5 <= istr_size(is1));
	assert(0 == strcmp(is1, "hello"));
	
	istring *is2 = istr_new_cstr("whatwhatwhatwhat");
	is2 = istr_assign(is2, is1);
	istr_free(is1);

	assert(5 == istr_len(is2));
	assert(5 <= istr_size(is2));
	assert(0 == strcmp(is2, "hello"));

	istr_free(is2);
}

static int test_trunc()
{
	istring *is1 = istr_new_cstr("hello world");

	is1 = istr_trunc(is1, 5);
	assert(5 == istr_len(is1));
	assert(11 <= istr_size(is1));
	assert(0 == strcmp(is1, "hello"));

	istr_free(is1);
}

static int test_pop()
{
	istring *is1 = istr_new_cstr("hello");

	char test[] = "hello";
	for (size_t i=0; i<=4; i++) {
		assert(test[4-i] == istr_pop(is1));
	}

	istr_free(is1);
}

static int test_write()
{
	istring *is1 = istr_new_cstr("foobar 20");
	is1 = istr_write_bytes(is1, 7, "2000 is aight.", 14);

	assert('\0' == is1[istr_len(is1)]);
	assert(21 == istr_len(is1));
	assert(21 <= istr_size(is1));
	assert(0 == strcmp(is1, "foobar 2000 is aight."));

	istr_free(is1);

	return 0;
}

int main()
{
	printf("Starting tests\n");
	test_new_and_free();
	test_grow_and_shrink();
	test_eq();
	test_assign();
	test_trunc();
	test_pop();
	test_write();
	//test_insert();
	//test_append();
	//loop_test_0();
	//printf("# of test failures: %d\n", glb_test_fail);
	printf("Testing complete\n");

	return 0;
}
