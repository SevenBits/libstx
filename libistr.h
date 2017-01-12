/* See LICENSE file for copyright and license details */
#ifndef LIBistr_H_INCLUDED
#define LIBistr_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>

// typedef the istring type for clarity, as its not usually a normal char*
typedef char istring;

// For documentation on functions, please see the man page 'man libistr'.
size_t istr_size(const istring *string);

size_t istr_len(const istring *string);

istring* istr_new(const istring *src);

istring* istr_new_bytes(const char *bytes, size_t bytes_len);

istring* istr_new_cstr(const char *cstr);

istring* istr_grow(istring *string, size_t len);

istring* istr_shrink(istring *string, size_t len);

void istr_free(istring *string);

int istr_eq(const istring *s1, const istring *s2);

char istr_index(const istring *string, size_t index);

istring* istr_slice(istring *slice, const istring *src, size_t begin, size_t end);

istring* istr_assign(istring *string, const istring *cstr);

istring* istr_assign_cstr(istring *string, const char *cstr);

istring* istr_assign_bytes(istring *string, const char *bytes, size_t bytes_len);

void istr_trunc(istring *string, size_t len);

char istr_pop(istring *string);

istring* istr_write(istring *dest, size_t index, const istring *src);

istring* istr_write_cstr(istring *string, size_t index, const char *cstr);

istring* istr_write_bytes(istring *string, size_t index, const char *bytes, size_t bytes_len);

void istr_remove_bytes(istring *string, size_t index, size_t len);

istring* istr_append(istring *dest, const istring *src);

istring* istr_append_cstr(istring *string, const char *cstr);

istring* istr_append_bytes(istring *string, const char *bytes, size_t bytes_len);

istring* istr_insert(istring *dest, size_t index, const istring *src);

istring* istr_insert_cstr(istring *string, size_t index, const char *cstr);

istring* istr_insert_bytes(istring *string, size_t index, const char *bytes, size_t bytes_len);

void istr_rstrip(istring *string, const char *chs);

void istr_lstrip(istring *string, const char *chs);

void istr_strip(istring *string, const char *chs);

char* istr_find(istring *string, const char *substr);

istring* istr_replace(istring *string, const char *find, const istring *replace);

istring* istr_replace_bytes(istring *string, const char *find, const char *replace, size_t r_len);

#endif
