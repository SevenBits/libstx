#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#include "libistr.h"

void test_new_and_free()
{
	istring *is1 = istr_new(NULL);
	assert(NULL != is1);
	assert(0 == istr_len(is1));
	assert(0 <= istr_size(is1));
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

	istring *is3;
	char varstr[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	for (size_t i=1; i<sizeof(varstr); i++) {
		is3 = istr_new_bytes(varstr, i);
		assert(i == istr_len(is3));
		assert(i <= istr_size(is3));
		assert('\0' == is3[i]);
		istr_free(is3);
	}
}

void test_grow()
{
	istring *is1 = istr_new(NULL);
	is1 = istr_grow(is1, 64);
	assert(0 == istr_len(is1));
	assert(64 <= istr_size(is1));
	assert('\0' == is1[0]);

	is1 = istr_grow(is1, 4096);
	assert(0 == istr_len(is1));
	assert(4096 <= istr_size(is1));
	assert('\0' == is1[0]);

	istr_free(is1);
}

void test_eq()
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

void test_assign()
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

void test_trunc()
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

void test_pop()
{
	istring *is1 = istr_new_cstr("hello");

	char test[] = "hello";
	for (size_t i=0; i<=4; i++) {
		assert(test[4-i] == istr_pop(is1));
	}

	istr_free(is1);
}

void test_write()
{
	istring *is1 = istr_new_cstr("foobar 20");
	is1 = istr_write_bytes(is1, 7, "2000 is aight.", 14);

	assert('\0' == is1[istr_len(is1)]);
	assert(21 == istr_len(is1));
	assert(21 <= istr_size(is1));
	assert(0 == strcmp(is1, "foobar 2000 is aight."));

	istr_free(is1);
}

void test_insert()
{
	istring *is1 = istr_new_cstr("hello world");

	is1 = istr_insert_bytes(is1, 5, "nomnom", 6);

	assert('\0' == is1[istr_len(is1)]);
	assert(17 == istr_len(is1));
	assert(17 <= istr_size(is1));
	assert(0 == strcmp(is1, "hellonomnom world"));

	istr_free(is1);

	istring *us1 = istr_new(NULL);
	char* ch = "a";
	for (size_t i=1; i<4096; i++) {
		us1 = istr_insert_bytes(us1, 0, ch, 1);
		assert(i == istr_len(us1));
		assert(i <= istr_size(us1));
		assert('\0' == us1[istr_len(us1)]);
	}

	istr_free(us1);
}

void test_append()
{
	istring *us1 = istr_new(NULL);
	char* ch = "a";
	for (size_t i=1; i<100; i++) {
		us1 = istr_append_bytes(us1, ch, 1);
		assert(i == istr_len(us1));
		assert(i <= istr_size(us1));
		assert('\0' == us1[istr_len(us1)]);
	}

	istr_free(us1);
}

void test_find()
{
	istring *us1 = istr_new_cstr("hellosubworld!");
	char *substr = istr_find(us1, "sub");
	assert(NULL != substr);
	assert(0 == strncmp(substr, "sub", 3));

	substr[1] = 'o';

	assert(0 == strcmp(us1, "hellosobworld!"));

	istr_free(us1);
}

void test_strip()
{
	istring *us1 = istr_new_cstr("zxcvMMnnhellonnMMzxcv");

	istr_lstrip(us1, "zxcv");
	assert(17 == istr_len(us1));
	assert('\0' == us1[istr_len(us1)]);
	assert(0 == strcmp(us1, "MMnnhellonnMMzxcv"));

	istr_rstrip(us1, "zxcv");
	assert(13 == istr_len(us1));
	assert('\0' == us1[istr_len(us1)]);
	assert(0 == strcmp(us1, "MMnnhellonnMM"));

	istr_strip(us1, "Mn");
	assert(5 == istr_len(us1));
	assert('\0' == us1[istr_len(us1)]);
	assert(0 == strcmp(us1, "hello"));

	istr_free(us1);

	// Empty strip
	istring *us2 = istr_new(NULL);

	istr_lstrip(us2, "hel");
	assert('\0' == *us2);
	assert(0 == istr_len(us2));

	istr_rstrip(us2, "opn");
	assert('\0' == *us2);
	assert(0 == istr_len(us2));

	istr_strip(us2, "zxcl");
	assert('\0' == *us2);
	assert(0 == istr_len(us2));

	istr_free(us2);

	// All chars stripped
	istring *us3 = istr_new_cstr("hello");

	istr_strip(us3, "helo");
	assert('\0' == *us3);
	assert(0 == istr_len(us3));

	istr_free(us3);
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
	//test_find();
	//test_replace();
	printf("Testing complete\n");

	return 0;
}
