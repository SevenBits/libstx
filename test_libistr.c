#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "libistr.h"

#define TEST(name) void name ## () \
{

void test_new_and_free() 
{
	istring *str;
	istring *two;

	str = istr_new(NULL);
	istr_free(str, true);

	two = istr_new_cstr("hi");
	assert(strcmp(two->buf, "hi") == 0);
	assert(two->len == 2);
	assert(two->size >= 2);

	str = istr_new(two);
	assert(strcmp(str->buf, "hi") == 0);
	assert(str->len == 2);
	assert(str->size >= 2);

	istr_free(two, true);
	istr_free(str, true);

	str = istr_new_bytes(NULL, 0);
	assert(str->len == 0);
	assert(str->size >= 0);
	istr_free(str, true);

	str = istr_new_bytes("hello", 6);
	istr_free(str, true);

	str = istr_new_cstr(NULL);
	istr_free(str, true);

	str = istr_new_cstr("hello");
	istr_free(str, true);
}

void test_istr_strptr()
{
	istring *n = istr_new_cstr("hello");
	char **tmp = &n->buf;
	n = istr_assign_cstr(n, "This string is longer, so it will realloc");
	printf("Pointer pointer: %s\n", *tmp);

	istr_free(n, true);
}

static inline void assign_assert(istring *string, char *str)
{
	size_t str_len = strlen(str);
	string = istr_assign_cstr(string, str);

	assert(strcmp(string->buf, str) == 0);
	assert(string->len == str_len);
	assert(string->size >= str_len);
}

void test_assign()
{
	istring *string = istr_new(NULL);

	// case1: assign a cstr to a NULL initialized istring
	assign_assert(string, "hello");
	// case2: assign a cstr to an already assigned istring
	assign_assert(string, "OH");
	// case3: assign a larger cstr to an istring to expand the size
	assign_assert(string, "WOWOWOWOWOWOWOWOWOW BIG STRING IS BIG");

	istr_free(string, true);
}

static inline void insert_assert(istring *string, size_t index, char *str,
                                 size_t str_len, char *outcome,
                                 size_t expected_len)
{
	string = istr_insert_bytes(string, index, str, str_len);

	assert(strcmp(string->buf, outcome) == 0);
	assert(string->len == expected_len);
	assert(string->size >= expected_len);
}

void test_insert()
{
	istring *string = istr_new_bytes("hello", 5);

	// case1: append
	insert_assert(string, 5, "omg", 4, "helloomg", 9);

	// case2: prepend
	insert_assert(string, 0, "omg", 3, "omghelloomg", 12);

	// case3: in the middle
	insert_assert(string, 6, "omg�", 6, "omghelomg�loomg", 18);
	
	istr_free(string, true);
}

void test_insert_bounds()
{
	istring *string = istr_new(NULL);
	for(int i=0; i<10000; i++) {
		string = istr_insert_bytes(string, 0, "123456789", 9);
	}
	string->buf[string->len] = '\0';
	printf("%s\n", string->buf + (string->len - 100));
	istr_free(string, false);
}

void test_append()
{
	istring *string = istr_new_cstr("hello");
	assert(string->len == 5);

	// case1a: small append
	string = istr_append_cstr(string, "omg");

	assert(strcmp(string->buf, "helloomg") == 0);
	assert(string->len == 8);
	assert(string->size >= 8);

	// case1b: large append
	string = istr_append_bytes(string, "123456789_123456789_", 20);

	assert(strncmp(string->buf, "helloomg123456789_123456789_", 28) == 0);
	assert(string->len == 28);
	assert(string->size >= 28);

	// case1c: append less than char buffer
	string = istr_append_bytes(string, "123456789_123456789_", 2);

	assert(strncmp(string->buf, "helloomg123456789_123456789_12", 30) == 0);
	assert(string->len == 30);
	assert(string->size >= 30);

	istr_free(string, true);

	// case2a: append NULL
	string = istr_new_bytes("hello", 5);

	assert(NULL == istr_append_bytes(string, NULL, 0));
	assert(errno == EINVAL);
	assert(0 == strncmp(string->buf, "hello", 5));
	assert(5 == string->len);
	assert(5 < string->size);

	istr_free(string, true);
}

void test_index()
{
	istring *string = istr_new_cstr("Hello");
	assert('H' == istr_index(string, 0));
	assert('o' == istr_index(string, 4));
}

void test_slice()
{
	istring *string = istr_new_cstr("Hello");
	istring *slice = istr_new(NULL);
	slice = istr_slice(slice, string, 2, 6);
	printf("string: %s; slice: %s\n", string->buf, slice->buf);
	istr_free(slice, true);
	istr_free(string, true);
}

int main()
{
	printf("Testing libistr...\n");
	test_new_and_free();
	test_istr_strptr();
	test_assign();
	test_insert();
	//test_insert_bounds();
	test_append();
	test_insert();
	test_slice();
	//test_prepend();
	printf("libistr testing success!\n");
	return 0;
}
