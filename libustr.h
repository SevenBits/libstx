/* See LICENSE file for copyright and license details */
#ifndef LIBustr_H_INCLUDED
#define LIBustr_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>

// typedef the ustring type for clarity, as its not usually a normal char*
typedef char ustring;

// For documentation on functions, please see the man page 'man libustr'.
size_t ustr_size(const ustring *string);

size_t ustr_len(const ustring *string);

ustring* ustr_new(const ustring *src);

ustring* ustr_new_bytes(const char *bytes, size_t bytes_len);

ustring* ustr_new_cstr(const char *cstr);

ustring* ustr_grow(ustring *string, size_t len);

ustring* ustr_shrink(ustring *string, size_t len);

void ustr_free(ustring *string);

int ustr_eq(const ustring *s1, const ustring *s2);

char ustr_index(const ustring *string, size_t index);

ustring* ustr_slice(ustring *slice, const ustring *src, size_t begin, size_t end);

ustring* ustr_assign(ustring *string, const ustring *cstr);

ustring* ustr_assign_cstr(ustring *string, const char *cstr);

ustring* ustr_assign_bytes(ustring *string, const char *bytes, size_t bytes_len);

ustring* ustr_trunc(ustring *string, size_t len);

char ustr_pop(ustring *string);

ustring* ustr_write(ustring *dest, size_t index, const ustring *src);

ustring* ustr_write_cstr(ustring *string, size_t index, const char *cstr);

ustring* ustr_write_bytes(ustring *string, size_t index, const char *bytes, size_t bytes_len);

ustring* ustr_remove_bytes(ustring *string, size_t index, size_t len);

ustring* ustr_append(ustring *dest, const ustring *src);

ustring* ustr_append_cstr(ustring *string, const char *cstr);

ustring* ustr_append_bytes(ustring *string, const char *bytes, size_t bytes_len);

ustring* ustr_insert(ustring *dest, size_t index, const ustring *src);

ustring* ustr_insert_cstr(ustring *string, size_t index, const char *cstr);

ustring* ustr_insert_bytes(ustring *string, size_t index, const char *bytes, size_t bytes_len);

#endif
