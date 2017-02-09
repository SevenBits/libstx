/* See LICENSE file for copyright and license details */
#ifndef LIBISTR_H_INCLUDED
#define LIBISTR_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * istring - a typedef for char.
 *
 * Useful if the distrinction between an istring and a char wants to be made
 */
typedef char istring;

/**
 * istr_size - return the size a string
 * @string: the string to return the size of
 *
 * The size is the total size of the buffer including unused bytes. This
 * does not include the size of the prefix.
 */
size_t istr_size(const istring *string);

/**
 * istr_len - return the length of a string
 * @string: the string to return the length of
 *
 * The length is the the amount of defined bytes before
 * the '\0' character at the end.
 */
size_t istr_len(const istring *string);

/**
 * Create a new heap allocated istring either empty or initialized
 * as a copy of the first argument. Remember to use `istr_free` on
 * any istrings created this way. 
 */
istring* istr_new(const istring *src);
istring* istr_new_bytes(const char *bytes, size_t bytes_len);
istring* istr_new_cstr(const char *cstr);
void istr_free(istring *string);

/**
 * Grow the istring buffer to at least `len` bytes. Nothing will occur
 * if the buffer is already longer than `len`. 
 */
istring* istr_grow(istring *string, size_t len);

/**
 * Check for equality between two istrings.
 */
int istr_eq(const istring *s1, const istring *s2);

/**
 * Copy part of `src` into `slice` from `begin` to `end`.
 */
istring* istr_slice(istring *slice, const istring *src, size_t begin, size_t end);

/** 
 * Assign `string` a copy of the contents of the second argument. 
 */
istring* istr_assign(istring *string, const istring *src);
istring* istr_assign_cstr(istring *string, const char *cstr);
istring* istr_assign_bytes(istring *string, const char *bytes, size_t bytes_len);

/** 
 * Truncate `string` by `len` bytes. Nothing will occur if `len` is greater
 * than or equal to the length of `string`. 
 */
void istr_trunc(istring *string, size_t len);

/** 
 * Remove the last byte from the end of `string` and return it. 
 */
char istr_pop(istring *string);

/**
 * Copy the contents of `src` to `string``string` with a copy of `src` 
 * at `index` of @string. 
 */
istring* istr_write(istring *string, size_t index, const istring *src);
istring* istr_write_cstr(istring *string, size_t index, const char *cstr);
istring* istr_write_bytes(istring *string, size_t index, const char *bytes, size_t bytes_len);

/** 
 * Remove `len` bytes from the middle of an `string`, and concatenate 
 * the sides those bytes were inbetween back together. 
 * eg. "hello" -> remove "ll" -> "heo".
 */
void istr_remove_bytes(istring *string, size_t index, size_t len);

/** 
 * Append a copy of the contents of the second argument to `string`.
 */
istring* istr_append(istring *string, const istring *src);
istring* istr_append_cstr(istring *string, const char *cstr);
istring* istr_append_bytes(istring *string, const char *bytes, size_t bytes_len);

/**
 * Insert a copy of the contents of the second argument to `string`
 */
istring* istr_insert(istring *string, size_t index, const istring *src);
istring* istr_insert_cstr(istring *string, size_t index, const char *cstr);
istring* istr_insert_bytes(istring *string, size_t index, const char *bytes, size_t bytes_len);

/**
 * Insert a copy of `ch` converted to a utf8 string at `index` in `string`
 */
istring* istr_insert_utf32(istring *string, size_t index, const uint32_t ch);

/**
 * istr_strip - Strip a set of characters from a string.
 * @string: string to strip
 * @chs: set of characters to strip
 *
 * Strip all characters specified in @chs from @string.
 * rstrip is from the right side,
 * lstrip is from the left side, 
 * strip is from both sides.
 */
void istr_rstrip(istring *string, const char *chs);
void istr_lstrip(istring *string, const char *chs);
void istr_strip(istring *string, const char *chs);

/** 
 * istr_find - find a substring within a string.
 * @string: string to be searched.
 * @substr: string to search for.
 *
 * Find the first occurance of @substr in @string, and return a pointer to it.
 * If there are no occurances of @substr in @string, NULL is returned. @substr
 * can either be a char* or an istring*.
 */
char* istr_find(istring *string, const char *substr);

#endif
