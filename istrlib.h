/* See LICENSE file for copyright and license details */
#ifndef istr_H_INCLUDED
#define istr_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>

// WARNING: Do not access these fields directly outside of test code
// this API is designed to not require the internals to be messed with
// as they could potentially change in the future.
typedef struct istring {
	char *buf;     // Character buffer.
	size_t size;   // Size of char buffer.
	size_t len; // Amount of bytes in the buffer.
} istring;

/* istr_new
 * (desc): Copy or initialize a new istring.
 * (return -> istring*):
 *   success: if @src is not NULL, duplicate @src exactly and return the 
 *     duplicate. Otherwise return an initialized (but empty) string object.
 *   memory error: NULL and errno = ENOMEM
 */
istring* istr_new(const istring *src);

/* istr_new_bytes
 * (desc): Initialize a new istring with arbitrary bytes.
 * (return -> istring*):
 *   success: if @bytes is not NULL and bytes_len is not 0, 
 *     duplicate @src exactly and return the duplicate. Otherwise return an 
 *     initialized (but empty) string object.
 *   memory error: NULL and errno = ENOMEM
 */
istring* istr_new_bytes(const char *bytes, size_t bytes_len);

/* istr_new_bytes
 * (desc): Initialize a new istring with a string with terminating null-byte.
 * (return -> istring*):
 *   success: if @bytes is not NULL and bytes_len is not 0, 
 *     duplicate @src exactly and return the duplicate. Otherwise return an 
 *     initialized (but empty) string object.
 *   memory error: NULL and errno = ENOMEM
 */
istring* istr_new_cstr(const char *cstr);

/* istr_free
 * (desc): Free all memory allocated to an istring structure.
 *   please use this instead of manually freeing.
 * (return -> char*):
 *   success: the string object's char buffer
 *   bad args: errno = EINVAL and NULL;
 */
char* istr_free(istring *string, bool free_buf);

/* istr_str
 * (desc): Returns a pointer to the istr char buffer.
 *   Use this for interoperability with <string.h> functions.
 * (warn): Please be careful, the buffer might be realloc'd by
 *   any of the non-const istr_funcs and you'll have to call
 *   this function again to avoid pointing to
 *   memory you shouldn't be pointing to.
 *
 *   TL;DR - Use the pointer returned by this functions
 *   as soon as possible before calling other istr_funcs on it
 * (return -> char*):
 *   success: The string object's char buffer
 *   bad args: errno = EINVAL and return NULL.
 */
char* istr_str(const istring *string);

/* istr_size
 * (desc): Return the amount of bytes in @string's char buffer
 * (return -> size_t):
 *   success: the amount of bytes in @string's char buffer
 *   bad args: 0 and errno = EINVAL
 */
size_t istr_len(const istring *string);

/* istr_size
 * (desc): Return the size of @string's char buffer
 * (return -> size_t):
 *   success: the size of the char buffer
 *   bad args: 0 and errno = EINVAL
 */
size_t istr_size(const istring *string);

/* istr_assign_bytes
 * (desc): Reassign an istring's contents to arbitrary bytes, overwriting any
 *   old contents.
 * (return -> istring*):
 *   success: the original string object
 *   bad args: NULL & errno=EINVAL 
 *   memory error: NULL & errno = ENOMEM
 */
istring* istr_assign_bytes(istring *string, const char *bytes, size_t bytes_len);

/* istr_assign_cstr
 * (desc): Reassign an istring's to a string with a terminating null-byte,
 *   overwriting any old contents.
 * (return -> istring*):
 *   success: the original string object
 *   bad args: NULL & errno=EINVAL 
 *   memory error: NULL & errno = ENOMEM
 */
istring* istr_assign_cstr(istring *string, const char *cstr);


/* istr_eq
 * (desc): Check if two istring objects are equivalent
 * (return -> int):
 *   success: 0 if equal, 1 if not equal
 *   bad args: -1 and errno = EINVAL
 */
int istr_eq(const istring *s1, const istring *s2);

/* istr_truncate_bytes
 * (desc): Shorten's the istring to a specified length.
 * (return -> istring*):
 *   success: the original istring object with truncated length
 *   bad args: NULL and errno = EINVAL
 */
istring* istr_truncate_bytes(istring *string, size_t len);

/* istr_pop_byte
 * (desc): Removes the last byte in an istring and returns it.
 * (return -> char):
 *   success: the byte that was removed
 *   bad args: -1 and errno = EINVAL
 */
char istr_pop_byte(istring *string);

/* istr_write
 * (desc): Overwrites an istring at an index with another istring
 * (return -> istring*):
 *   success: the original string object
 *   bad args: NULL & errno=EINVAL 
 *   memory error: NULL & errno = ENOMEM
 */
istring* istr_write(istring *dest, size_t index, const istring *src);

/* istr_write_bytes
 * (desc): Overwrites an istring at an index with arbitrary bytes
 * (return -> istring*):
 *   success: the original string object
 *   bad args: NULL & errno=EINVAL 
 *   memory error: NULL & errno = ENOMEM
 */
istring* istr_write_bytes(istring *string, size_t index, const char *bytes, size_t bytes_len);

/* istr_prepend
 * (desc): Prepends a copy of an istring to another istring.
 * (return -> istring*):
 *   success: the original string object
 *   bad args: NULL & errno=EINVAL 
 *   memory error: NULL & errno = ENOMEM
 */
istring* istr_prepend(istring *dest, const istring *src);

/* istr_prepend_bytes
 * (desc): Prepends arbitrary bytes onto an istring.
 * (return -> istring*):
 *   success: original string object
 *   bad args: NULL & errno = EINVAL
 *   memory error: NULL & errno = ENOMEM
 */
istring* istr_prepend_bytes(istring *string, const char *bytes, size_t bytes_len);

/* istr_append
 * (desc): Appends a copy of an istring to another istring.
 * (return -> istring*):
 *   success: the original string object
 *   bad args: NULL & errno=EINVAL 
 *   memory error: NULL & errno = ENOMEM
 */
istring* istr_append(istring *dest, const istring *src);

/* istr_append_bytes
 * (desc): Appends arbitrary bytes onto an istring.
 * (return -> istring*):
 *   success: original string object
 *   bad args: NULL & errno = EINVAL
 *   memory error: NULL & errno = ENOMEM
 */
istring* istr_append_bytes(istring *string, const char *bytes, size_t bytes_len);

/* istr_insert
 * (desc): Inserts an istring into an istring at an index without overwriting
 * (return -> istring*):
 *   success: the original string object
 *   bad args: NULL & errno=EINVAL 
 *   memory error: NULL & errno = ENOMEM
 */
istring* istr_insert(istring *dest, size_t index, const istring *src);

/* istr_insert_bytes
 * (desc): Inserts arbitrary bytes into an istring at an index without 
 *   overwriting
 * (return -> istring*):
 *   success: the original string object
 *   bad args: NULL & errno=EINVAL 
 *   memory error: NULL & errno = ENOMEM
 */
istring* istr_insert_bytes(istring *string, size_t index, const char *bytes, size_t bytes_len);

#endif
