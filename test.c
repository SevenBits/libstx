#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#include "libistr.h"

char G_CSTR[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
size_t G_CSTR_LEN = sizeof(G_CSTR) - 1;

static void test_new_and_free()
{
	istring *is1;

	is1 = istr_new(NULL);
	assert(NULL != is1);
	assert(0 == istr_len(is1));
	assert(0 <= istr_size(is1));
	assert('\0' == is1[0]);
	istr_free(is1);

	for (size_t i=0; i<sizeof(G_CSTR); i++) {
		is1 = istr_new_bytes(G_CSTR, i);
		istr_free(is1);
	}
}

static void test_grow()
{
	istring *is1 = istr_new(NULL);
	for (size_t i=2; i<65536; i*=2) {
		is1 = istr_grow(is1, i);
		assert(0 == istr_len(is1));
		assert(i <= istr_size(is1));
		assert('\0' == is1[0]);
	}
	istr_free(is1);
}

static void test_eq()
{
	istring *is1 = istr_new_cstr("hello");
	istring *is2 = istr_new_cstr("hello");
	assert(0 == istr_eq(is1, is2));
	is2[4] = 'x';
	assert(1 == istr_eq(is1, is2));

	assert(-1 == istr_eq(NULL, NULL));

	istr_free(is1);
	istr_free(is2);

	is1 = istr_new(NULL);
	is2 = istr_new(NULL);

	assert(0 == istr_eq(is1, is2));

	istr_free(is1);
	istr_free(is2);
}

static void test_assign()
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

	is1 = istr_new(NULL);
	is2 = istr_new_cstr("hello");

	is2 = istr_assign(is2, is1);

	assert(0 == istr_len(is2));
	assert(0 <= istr_size(is2));
	assert('\0' == *is2);

	istr_free(is1);
	istr_free(is2);
}

static void test_trunc()
{
	istring *is1 = istr_new_cstr("hello world");

	istr_trunc(is1, 5);
	assert(5 == istr_len(is1));
	assert(11 <= istr_size(is1));
	assert(0 == strcmp(is1, "hello"));

	istr_trunc(is1, 100);
	assert(5 == istr_len(is1));
	assert(11 <= istr_size(is1));
	assert(0 == strcmp(is1, "hello"));

	istr_free(is1);
}

static void test_pop()
{
	istring *is1 = istr_new_cstr(G_CSTR);

	for (size_t i=0; i<G_CSTR_LEN; i++) {
		assert(G_CSTR[G_CSTR_LEN-i-1] == istr_pop(is1));
	}

	istr_free(is1);
}

static void test_write()
{
	istring *is1 = istr_new_cstr("foobar 20");
	is1 = istr_write_bytes(is1, 7, "2000 is aight.", 14);

	assert('\0' == is1[istr_len(is1)]);
	assert(21 == istr_len(is1));
	assert(21 <= istr_size(is1));
	assert(0 == strcmp(is1, "foobar 2000 is aight."));

	istr_free(is1);
}

static void test_insert()
{
	istring *is1 = istr_new_cstr("hello world");

	is1 = istr_insert_bytes(is1, 5, "nomnom", 6);

	assert('\0' == is1[istr_len(is1)]);
	assert(17 == istr_len(is1));
	assert(17 <= istr_size(is1));
	assert(0 == strcmp(is1, "hellonomnom world"));

	istr_free(is1);

	istring *is2 = istr_new(NULL);
	char* ch = "a";
	for (size_t i=1; i<4096; i++) {
		is2 = istr_insert_bytes(is2, 0, ch, 1);
		assert(i == istr_len(is2));
		assert(i <= istr_size(is2));
		assert('\0' == is2[istr_len(is2)]);
	}

	istr_free(is2);
}

static void test_append()
{
	istring *is1 = istr_new(NULL);
	char* ch = "a";
	for (size_t i=1; i<100; i++) {
		is1 = istr_append_bytes(is1, ch, 1);
		assert(i == istr_len(is1));
		assert(i <= istr_size(is1));
		assert('\0' == is1[istr_len(is1)]);
	}

	istr_free(is1);
}

static void test_strip()
{
	istring *is1 = istr_new_cstr("zxcvMMnnhellonnMMzxcv");

	istr_lstrip(is1, "zxcv");
	assert(17 == istr_len(is1));
	assert('\0' == is1[istr_len(is1)]);
	assert(0 == strcmp(is1, "MMnnhellonnMMzxcv"));

	istr_rstrip(is1, "zxcv");
	assert(13 == istr_len(is1));
	assert('\0' == is1[istr_len(is1)]);
	assert(0 == strcmp(is1, "MMnnhellonnMM"));

	istr_strip(is1, "Mn");
	assert(5 == istr_len(is1));
	assert('\0' == is1[istr_len(is1)]);
	assert(0 == strcmp(is1, "hello"));

	istr_free(is1);

	// Empty strip
	istring *is2 = istr_new(NULL);

	istr_lstrip(is2, "hel");
	assert('\0' == *is2);
	assert(0 == istr_len(is2));

	istr_rstrip(is2, "opn");
	assert('\0' == *is2);
	assert(0 == istr_len(is2));

	istr_strip(is2, "zxcl");
	assert('\0' == *is2);
	assert(0 == istr_len(is2));

	istr_free(is2);

	// All chars stripped
	istring *is3 = istr_new_cstr("hello");

	istr_strip(is3, "helo");
	assert('\0' == *is3);
	assert(0 == istr_len(is3));

	istr_free(is3);
}

static void test_find()
{
	istring *is1 = istr_new_cstr("subworldstring");
	char *ss;
		
	ss = istr_find(is1, "world");

	assert(0 == strncmp(ss, "world", 5));

	ss = istr_find(is1, "sub");

	assert(0 == strncmp(ss, "sub", 3));

	ss = istr_find(is1, "string");

	assert(0 == strncmp(ss, "string", 6));

	istr_free(is1);
}

static void test_replace()
{
}

static void test_slice()
{
}

int main()
{
	printf("Starting tests\n");
	test_new_and_free();
	test_grow();
	test_eq();
	test_assign();
	test_trunc();
	test_pop();
	test_write();
	test_insert();
	test_append();
	test_find();
	test_strip();
	test_find();
	//test_replace();
	//test_slice()
	printf("Testing complete\n");

	return 0;
}
