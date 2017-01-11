#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#include "libustr.h"

void test_new_and_free()
{
	ustring *is1 = ustr_new(NULL);
	assert(NULL != is1);
	assert(0 == ustr_len(is1));
	assert(2 == ustr_size(is1));
	assert('\0' == is1[0]);
	ustr_free(is1);

	is1 = ustr_new_bytes("hello", 5);
	assert(NULL != is1);
	assert(5 == ustr_len(is1));
	assert(5 <= ustr_size(is1));
	assert('\0' == is1[5]);
	assert(0 == strcmp(is1, "hello"));
	ustr_free(is1);

	is1 = ustr_new_cstr("hello world");
	assert(NULL != is1);
	assert(11 == ustr_len(is1));
	assert(11 <= ustr_size(is1));
	assert('\0' == is1[11]);
	assert(0 == strcmp(is1, "hello world"));

	ustring *is2 = ustr_new(is1);
	ustr_free(is1);

	assert(NULL != is2);
	assert(11 == ustr_len(is2));
	assert(11 <= ustr_size(is2));
	assert('\0' == is2[11]);
	assert(0 == strcmp(is2, "hello world"));
	ustr_free(is2);

	ustring *is3;
	char varstr[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	for (size_t i=1; i<sizeof(varstr); i++) {
		is3 = ustr_new_bytes(varstr, i);
		assert(i == ustr_len(is3));
		assert(i <= ustr_size(is3));
		assert('\0' == is3[i]);
		ustr_free(is3);
	}
}

void test_grow_and_shrink()
{
	ustring *is1 = ustr_new(NULL);
	is1 = ustr_grow(is1, 64);
	assert(0 == ustr_len(is1));
	assert(64 <= ustr_size(is1));
	assert('\0' == is1[0]);

	is1 = ustr_grow(is1, 4096);
	assert(0 == ustr_len(is1));
	assert(4096 <= ustr_size(is1));
	assert('\0' == is1[0]);

	is1 = ustr_shrink(is1, 8);
	assert(0 == ustr_len(is1));
	assert(8 <= ustr_size(is1));
	assert(32 >= ustr_size(is1));
	assert('\0' == is1[0]);

	ustr_free(is1);
}

void test_eq()
{
	ustring *is1 = ustr_new_cstr("hello");
	ustring *is2 = ustr_new_cstr("hello");
	assert(0 == ustr_eq(is1, is2));
	is2[4] = 'x';
	assert(1 == ustr_eq(is1, is2));

	assert(-1 == ustr_eq(NULL, NULL));

	ustr_free(is1);
	ustr_free(is2);
}

void test_assign()
{
	ustring *is1 = ustr_new(NULL);
	assert(NULL == ustr_assign(is1, NULL));

	is1 = ustr_assign_bytes(is1, "hello", 5);

	assert(NULL != is1);
	assert(5 == ustr_len(is1));
	assert(5 <= ustr_size(is1));
	assert(0 == strcmp(is1, "hello"));
	
	ustring *is2 = ustr_new_cstr("whatwhatwhatwhat");
	is2 = ustr_assign(is2, is1);
	ustr_free(is1);

	assert(5 == ustr_len(is2));
	assert(5 <= ustr_size(is2));
	assert(0 == strcmp(is2, "hello"));

	ustr_free(is2);
}

void test_trunc()
{
	ustring *is1 = ustr_new_cstr("hello world");

	is1 = ustr_trunc(is1, 5);
	assert(5 == ustr_len(is1));
	assert(11 <= ustr_size(is1));
	assert(0 == strcmp(is1, "hello"));

	ustr_free(is1);
}

void test_pop()
{
	ustring *is1 = ustr_new_cstr("hello");

	char test[] = "hello";
	for (size_t i=0; i<=4; i++) {
		assert(test[4-i] == ustr_pop(is1));
	}

	ustr_free(is1);
}

void test_write()
{
	ustring *is1 = ustr_new_cstr("foobar 20");
	is1 = ustr_write_bytes(is1, 7, "2000 is aight.", 14);

	assert('\0' == is1[ustr_len(is1)]);
	assert(21 == ustr_len(is1));
	assert(21 <= ustr_size(is1));
	assert(0 == strcmp(is1, "foobar 2000 is aight."));

	ustr_free(is1);
}

void test_insert()
{
	ustring *is1 = ustr_new_cstr("hello world");

	is1 = ustr_insert_bytes(is1, 5, "nomnom", 6);

	assert('\0' == is1[ustr_len(is1)]);
	assert(17 == ustr_len(is1));
	assert(17 <= ustr_size(is1));
	assert(0 == strcmp(is1, "hellonomnom world"));

	ustr_free(is1);

	ustring *us1 = ustr_new(NULL);
	char* ch = "a";
	for (size_t i=1; i<4096; i++) {
		us1 = ustr_insert_bytes(us1, 0, ch, 1);
		assert(i == ustr_len(us1));
		assert(i <= ustr_size(us1));
		assert('\0' == us1[ustr_len(us1)]);
	}

	ustr_free(us1);
}

void test_append()
{
	ustring *us1 = ustr_new(NULL);
	char* ch = "a";
	for (size_t i=1; i<100; i++) {
		us1 = ustr_append_bytes(us1, ch, 1);
		assert(i == ustr_len(us1));
		assert(i <= ustr_size(us1));
		assert('\0' == us1[ustr_len(us1)]);
	}

	ustr_free(us1);
}

void test_find()
{
	ustring *us1 = ustr_new_cstr("hellosubworld!");
	char *substr = ustr_find(us1, "sub");
	assert(NULL != substr);
	assert(0 == strncmp(substr, "sub", 3));

	substr[1] = 'o';

	assert(0 == strcmp(us1, "hellosobworld!"));

	ustr_free(us1);
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
	test_insert();
	test_append();
	test_find();
	//test_replace();
	printf("Testing complete\n");

	return 0;
}
