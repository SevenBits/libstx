#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "libistr.h"

void test_new_and_free() 
{
	istring *is1;
	istring *is2;

	is1 = istr_new(NULL);

	assert(0 == is1->len);
	assert(2 <= is1->size);
	assert('\0' == is1->buf[0]);

	is2 = istr_new(is1);
	istr_free(is1, true);

	assert(0 == is2->len);
	assert(2 <= is2->size);
	assert('\0' == is2->buf[0]);

	istr_free(is2, true);
	is1 = istr_new_bytes("hello", 5);

	assert(5 == is1->len);
	assert(5 <= is1->size);
	assert('\0' == is1->buf[5]);
	assert(0 == strcmp(is1->buf, "hello"));

	is2 = istr_new(is1);
	istr_free(is1, true);

	assert(5 == is2->len);
	assert(5 <= is2->size);
	assert('\0' == is2->buf[5]);
	assert(0 == strcmp(is2->buf, "hello"));

	istr_free(is2, true);
	is1 = istr_new_cstr("world");

	assert(5 == is1->len);
	assert(5 <= is1->size);
	assert('\0' == is1->buf[5]);
	assert(0 == strcmp(is1->buf, "world"));

	istr_free(is1, true);
	
	printf("test_new_and_free success!\n");
}

void test_free_release()
{
	istring *is1 = istr_new_cstr("hello");
	char *buf = istr_free(is1, false);

	assert('\0' == buf[5]);
	assert(5 == strlen(buf));
	assert(0 == strcmp(buf, "hello"));

	free(buf);

	printf("test_free_release success!\n");
}

void test_slice()
{
	istring *is1 = istr_new_cstr("test string zebra");
	istring *slice = istr_new(NULL);

	slice = istr_slice(slice, is1, 2, 7);
	istr_free(is1, true);

	assert(5 == slice->len);
	assert(5 <= slice->size);
	assert(0 == strcmp(slice->buf, "st st"));

	istr_free(slice, true);

	printf("test_slice success!\n");
}

void test_truncate()
{
	istring *is1;

	is1 = istr_new_cstr("hello world");
	assert(11 == is1->len);
	assert(11 <= is1->size);

	is1 = istr_truncate_bytes(is1, 5);
	assert(5 == is1->len);
	assert(5 <= is1->size);
	assert(0 == strcmp(is1->buf, "hello"));

	is1 = istr_assign_cstr(is1, "test");
	is1 = istr_truncate_bytes(is1, 0);

	assert(0 == is1->len);
	assert(0 <= is1->size);
	assert('\0' == is1->buf[0]);

	is1 = istr_assign_cstr(is1, "test");
	is1 = istr_truncate_bytes(is1, 100);

	assert(4 == is1->len);
	assert(4 <= is1->size);
	assert(0 == strcmp(is1->buf, "test"));

	istr_free(is1, true);
}

void test_write()
{
	istring *is1;

	is1 = istr_new_cstr("hel00000rld");
	assert(11 == is1->len);
	assert(11 <= is1->size);

	is1 = istr_write_cstr(is1, 3, "lo wo");
	assert(11 == is1->len);
	assert(11 <= is1->size);
	assert(0 == strcmp(is1->buf, "hello world"));

	istr_free(is1, true);
}

void test_remove()
{
	istring *is1;

	is1 = istr_new_cstr("hello_________ world");
	assert(20 == is1->len);
	assert(20 <= is1->size);

	is1 = istr_remove_bytes(is1, 5, 9);
	assert(11 == is1->len);
	assert(20 <= is1->size);
	assert(0 == strcmp(is1->buf, "hello world"));

	assert(NULL == istr_remove_bytes(is1, 12, 0));
	assert(ERANGE == errno);

	assert(11 == is1->len);
	assert(20 <= is1->size);
	assert(0 == strcmp(is1->buf, "hello world"));

	istr_free(is1, true);
}

void test_insert()
{
	istring *is1;
	istring *is2;
		
	is1 = istr_new_cstr("test string");
	istr_insert_cstr(is1, 5, "my ");

	assert(14 == is1->len);
	assert(14 <= is1->size);
	assert(0 == strcmp(is1->buf, "test my string"));

	is2 = istr_new(is1);
	istr_free(is1, true);

	istr_insert_bytes(is2, is2->len, " pleaseTHISISJUNK", 7);
	assert(21 == is2->len);
	assert(21 <= is2->size);
	assert(0 == strcmp(is2->buf, "test my string please"));
	assert('\0' == is2->buf[is2->len]);

	istr_free(is2, true);
}

void test_append()
{
	istring *is1;
	istring *is2;
		
	is1 = istr_new_cstr("hello ");
	is2 = istr_append_bytes(is2, "wo", 2);
	is2 = istr_append_cstr(is2, "rld");

	assert(11 == is1->len);
	assert(11 <= is1->size);
	assert(0 == strcmp(is1->buf, "hello world"));

	is2 = istr_new(NULL);
	is2 = istr_append(is2, is1);

	assert(11 == is2->len);
	assert(11 <= is2->size);
	assert(0 == strcmp(is2->buf, "hello world"));

	istr_free(is1, true);
	istr_free(is2, true);
}

void test_istr_eq()
{
	istring *is1;
	istring *is2;

	is1 = istr_new_cstr("hello ");
	is2 = istr_new_cstr("hello");

	assert(0 != istr_eq(is1, is2));

	is1 = istr_truncate_bytes(is1, 5);

	assert(0 == istr_eq(is1, is2));

	istr_free(is1, true);
	istr_free(is2, true);
}

int main()
{
	printf("Testing libistr...\n");
	test_new_and_free();

	test_free_release();

	test_slice();

	test_truncate();

	test_write();

	test_remove();

	test_insert();

	test_append();

	test_istr_eq();

	printf("libistr testing success!\n");
	return 0;
}
