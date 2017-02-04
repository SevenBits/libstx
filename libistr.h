/* See LICENSE file for copyright and license details */
#ifndef LIBISTR_H_INCLUDED
#define LIBISTR_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>

// This istring type is simply for code clarity, its completely optional.
typedef char istring;

/* Return the size of an istring's buffer in bytes. */
size_t istr_size(const istring *string);

/* Return the length used of an istring's buffer in bytes. */
size_t istr_len(const istring *string);

/* Create a new heap allocated istring as a copy of @src */
istring* istr_new(const istring *src);

/* Create a new heap allocated istring as a copy of @bytes_len bytes from @bytes */
istring* istr_new_bytes(const char *bytes, size_t bytes_len);

/* Create a new heap allocated istring as a copy of NULL-terminated @cstr */
istring* istr_new_cstr(const char *cstr);

/* Grow the istring buffer to at least @len bytes. Nothing will occur
 * if the buffer is already longer than @len. */
istring* istr_grow(istring *string, size_t len);

/* Free an istring allocated with any of the "new" functions. */
void istr_free(istring *string);

/* Check for equality between two istrings.
 * return:
 *   -1 <- If either input is NULL.
 *    0 <- If strings are equal.
 *    1 <- If strings are not equal. */
int istr_eq(const istring *s1, const istring *s2);

/* Slice part of an @src into a new @slice.
 * let @slice be any initialized istring.
 *     @src be any initialized istring.
 *     @begin be any index with the length of @src.
 *     @end be any index after @begin but within the length of @src.
 * return:
 *   A pointer to @slice's updated contents.
 *   NULL <- If any of the arguments are invalid.
 */
istring* istr_slice(istring *slice, const istring *src, size_t begin, size_t end);

/* Assign an istring a copy of the contents of @src */
istring* istr_assign(istring *dest, const istring *src);

/* Assign an istring a copy of the contents of @cstr */
istring* istr_assign_cstr(istring *string, const char *cstr);

/* Assign an istring a copy of the contents of @bytes_len bytes from @bytes */
istring* istr_assign_bytes(istring *string, const char *bytes, size_t bytes_len);

/* Truncate an @string by @len bytes. Nothing will occur if @len is greater 
 * than the length of @string. */
void istr_trunc(istring *string, size_t len);

/* Remove the last byte from the end of @string and return it. */
char istr_pop(istring *string);

/* Overwrite contents of @string with @src at @index of @string. */
istring* istr_write(istring *dest, size_t index, const istring *src);

/* Overwrite contents of @string with NULL-terminated @cstr at @index of @string. */
istring* istr_write_cstr(istring *string, size_t index, const char *cstr);

/* Overwrite contents of @string with @len bytes of @bytes at @index of @string. */
istring* istr_write_bytes(istring *string, size_t index, const char *bytes, size_t bytes_len);

/* Remove bytes from the middle of an istring, and concatenate the sides
 * those bytes were between back together. 
 * eg. "hello" -> remove "ll" -> "heo" */
void istr_remove_bytes(istring *string, size_t index, size_t len);

/* Append a copy of @src to @dest */
istring* istr_append(istring *dest, const istring *src);

/* Append a copy of NULL-terminated @cstr to @dest */
istring* istr_append_cstr(istring *string, const char *cstr);

/* Append a copy of @bytes_len bytes from @bytes to @dest */
istring* istr_append_bytes(istring *string, const char *bytes, size_t bytes_len);

/* Insert a copy of @src to @dest at @index */
istring* istr_insert(istring *dest, size_t index, const istring *src);

/* Insert a copy of NULL-terminated @cstr to @string at @index */
istring* istr_insert_cstr(istring *string, size_t index, const char *cstr);

/* Insert a copy of @bytes_len bytes from @bytes to @string at @index */
istring* istr_insert_bytes(istring *string, size_t index, const char *bytes, size_t bytes_len);

/* Insert a copy of @ch converted to utf8 from utf32 @index in @string */
istring* istr_insert_utf32(istring *string, size_t index, const uint32_t ch);

/* Strip all characters in the NULL-terminated @chs from the right side of @string */
void istr_rstrip(istring *string, const char *chs);

/* Strip all characters in the NULL-terminated @chs from the left side of @string */
void istr_lstrip(istring *string, const char *chs);

/* Strip all characters in the NULL-terminated @chs from both sides of @string */
void istr_strip(istring *string, const char *chs);

/* Find the first occurance of @substr in @string, and return a pointer to it.
 * If there are no occurances of @substr in @string, NULL is returned. */
char* istr_find(istring *string, const char *substr);

#endif
