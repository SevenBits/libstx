/* See LICENSE file for copyright and license details */
#ifndef LIBISTR_H_INCLUDED
#define LIBISTR_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>

/* 
WARNING: Do not access these fields directly outside of test code
this API is designed to not require the internals to be messed with
as they could potentially change in the future.
*/
typedef struct istring {
	char *buf;     // Character buffer.
	size_t size;   // Size of char buffer.
	size_t len; // Amount of bytes in the buffer.
} istring;

/*
istr_new:
	Initialize a new istring from an existing istring.

args:
	@src: existing istring or NULL

return -> istring*:
	success: istring initialized with @src, or an empty initialized 
		string object if @src is NULL.
	memory error: NULL and errno = ENOMEM
*/
istring* istr_new(const istring *src);

/*
istr_new_bytes:
	Initialize a new istring with a string with arbitrary bytes.

args:
	@bytes: bytes to initialize the new istring with or NULL
	@bytes_len: length of @bytes

return -> istring*:
	success: istring initialized with @bytes, or an empty initialized 
		string object if @bytes is NULL.
	memory error: NULL and errno = ENOMEM
*/
istring* istr_new_bytes(const char *bytes, size_t bytes_len);

/*
istr_new_cstr:
	Initialize a new istring with a null-terminated string.

args:
	@cstr: null-terminated string to initialize the new istring with or NULL

return -> istring*:
	success: istring initialized with @cstr or an empty initialized 
		string object if @cstr is NULL.
	memory error: NULL and errno = ENOMEM'
*/
istring* istr_new_cstr(const char *cstr);

/*
istr_free:
	Free all memory allocated to an istring structure. please use this 
	instead of manually freeing.

args:
	@string: the istring to be freed
	@free_buf: if true, free the internal buffer. if false, return the buffer

return -> char*:
	success: NULL or the char buffer
	bad args: NULL and errno = EINVAL
*/
char* istr_free(istring *string, bool free_buf);

/* 
istr_str:
	Get a the istr char buffer.
	Use this for interoperability with <string.h> functions.
	Please be careful, the buffer might be realloc'd by
	any of the non-const istr_funcs and you'll have to call
	this function again to avoid pointing to
	memory you shouldn't be pointing to.

	TL;DR - Use the pointer returned by this functions
	as soon as possible before calling other istr_funcs on it

return -> char*:
	success: The string object's char buffer
	bad args: errno = EINVAL and return NULL.
 */
char* istr_str(const istring *string);

/* 
istr_str:
	Get a pointer to the istring char buffer if you need a reference to
	the char buffer that won't be invalid upon automatic string reallocation
	from any of the other functions in this library.

return -> char**:
	success: A pointer to the string object's char buffer
	bad args: errno = EINVAL and return NULL.
 */
char** istr_strptr(istring *string);

/*
istr_len:
	Get the amount of bytes contained by the given istring.

return -> size_t:
	success: the amount of bytes in @string's char buffer
	bad args: 0 and errno = EINVAL
 */
size_t istr_len(const istring *string);

/* 
istr_size:
	Get the size allocated to the given istring's character buffer.

return -> size_t:
	success: the size of the char buffer
	bad args: 0 and errno = EINVAL
 */
size_t istr_size(const istring *string);

/* 
istr_eq:
	Check if two istings contain the same contents.

return -> int:
	success: 0 if equal, 1 if not equal
	bad args: -1 and errno = EINVAL
 */
int istr_eq(const istring *s1, const istring *s2);

/* 
istr_slice:
	Copy a slice of characters from one string to another

args:
	@slice: istring to store the slice
	@src: initialized istring to get the slice from
	@begin: beginning index of the slice
	@end: ending index of the slice

return -> istring*:
	success: The slice istring
	bad args: NULL and errno = EINVAL
	memory error: NULL & errno = ENOMEM
 */
istring* istr_slice(istring *slice, const istring *src, size_t begin, size_t end);

/* 
istr_assign_bytes:
	Reassign an istring's contents to arbitrary bytes, overwriting any
	old contents.

return -> istring*:
	success: the original string object
	bad args: NULL & errno=EINVAL 
	memory error: NULL & errno = ENOMEM
 */
istring* istr_assign_bytes(istring *string, const char *bytes, size_t bytes_len);

/* 
istr_assign_cstr:
	Reassign an istring's to a string with a terminating null-byte,
	overwriting any old contents.

return -> istring*:
	success: the original string object
	bad args: NULL & errno=EINVAL 
	memory error: NULL & errno = ENOMEM
 */
istring* istr_assign_cstr(istring *string, const char *cstr);

/* 
istr_truncate_bytes:
	Shorten's the istring to a specified length.

return -> istring*:
	success: the original istring object with truncated length
	bad args: NULL and errno = EINVAL
 */
istring* istr_truncate_bytes(istring *string, size_t len);

/* 
istr_pop_byte:
	Removes the last byte in an istring and returns it.

return -> char:
	success: the byte that was removed
	bad args: -1 and errno = EINVAL
 */
char istr_pop_byte(istring *string);

/* 
istr_write:
	Overwrites an istring at an index with another istring

return -> istring*:
	success: the original string object
	bad args: NULL & errno=EINVAL 
	memory error: NULL & errno = ENOMEM
 */
istring* istr_write(istring *dest, size_t index, const istring *src);

/* 
istr_write_bytes:
	Overwrites an istring at an index with arbitrary bytes
return -> istring*:
	success: the original string object
	bad args: NULL & errno=EINVAL 
	memory error: NULL & errno = ENOMEM
 */
istring* istr_write_bytes(istring *string, size_t index, const char *bytes, size_t bytes_len);

/* 
istr_prepend:
	Prepends a copy of an istring to another istring.

return -> istring*:
	success: the original string object
	bad args: NULL & errno=EINVAL 
	memory error: NULL & errno = ENOMEM
 */
istring* istr_prepend(istring *dest, const istring *src);

/* 
istr_prepend_bytes:
	Prepends arbitrary bytes onto an istring.

return -> istring*:
	success: original string object
	bad args: NULL & errno = EINVAL
	memory error: NULL & errno = ENOMEM
 */
istring* istr_prepend_bytes(istring *string, const char *bytes, size_t bytes_len);

/* 
istr_append:
	Appends a copy of an istring to another istring.

return -> istring*:
	success: the original string object
	bad args: NULL & errno=EINVAL 
	memory error: NULL & errno = ENOMEM
 */
istring* istr_append(istring *dest, const istring *src);

/* 
istr_append_bytes:
	Appends arbitrary bytes onto an istring.

return -> istring*:
	success: original string object
	bad args: NULL & errno = EINVAL
	memory error: NULL & errno = ENOMEM
 */
istring* istr_append_bytes(istring *string, const char *bytes, size_t bytes_len);

/* 
istr_insert:
	Inserts an istring into an istring at an index without overwriting
return -> istring*:
	success: the original string object
	bad args: NULL & errno=EINVAL 
	memory error: NULL & errno = ENOMEM
 */
istring* istr_insert(istring *dest, size_t index, const istring *src);

/* 
istr_insert_bytes:
	Inserts arbitrary bytes into an istring at an index without overwriting

return -> istring*:
	success: the original string object
	bad args: NULL & errno=EINVAL 
	memory error: NULL & errno = ENOMEM
 */
istring* istr_insert_bytes(istring *string, size_t index, const char *bytes, size_t bytes_len);

#endif
