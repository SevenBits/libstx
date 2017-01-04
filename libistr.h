/* See LICENSE file for copyright and license details */
#ifndef LIBISTR_H_INCLUDED
#define LIBISTR_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>

// Fields of this struct are public
typedef struct istring {
	char *buf;     // Character buffer.
	size_t len; // Amount of bytes in the buffer.
	size_t size;   // Size of char buffer.
} istring;

// For documentation on functions, please see the man page 'man libistr'.

istring* istr_new(const istring *src);

istring* istr_new_bytes(const char *bytes, size_t bytes_len);

istring* istr_new_cstr(const char *cstr);

char* istr_free(istring *string, bool free_buf);

int istr_eq(const istring *s1, const istring *s2);

char istr_index(const istring *string, size_t index);

istring* istr_slice(istring *slice, const istring *src, size_t begin, size_t end);

istring* istr_assign_bytes(istring *string, const char *bytes, size_t bytes_len);

istring* istr_assign_cstr(istring *string, const char *cstr);

istring* istr_trunc(istring *string, size_t len);

char istr_pop_byte(istring *string);

istring* istr_write(istring *dest, size_t index, const istring *src);

istring* istr_write_cstr(istring *string, size_t index, const char *cstr);

istring* istr_write_bytes(istring *string, size_t index, const char *bytes, size_t bytes_len);

istring* istr_remove_bytes(istring *string, size_t index, size_t len);

istring* istr_insert(istring *dest, size_t index, const istring *src);

istring* istr_insert_cstr(istring *string, size_t index, const char *cstr);

istring* istr_insert_bytes(istring *string, size_t index, const char *bytes, size_t bytes_len);

#endif
