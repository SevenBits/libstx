/* See LICENSE file for copyright and license details */
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "libistr.h"

// Length index
static const size_t LINDEX = -(sizeof(size_t));
// Size index
static const size_t SINDEX = -(sizeof(size_t) * 2);
// Header size
static const size_t HSIZE = sizeof(size_t) * 2;

/* 
safe_add:
	Safely adds together two size_t values while flagging an error
	if integer overflow would happen.

return -> size_t:
	success: Sum of the two arguments
	range error: SIZE_MAX and errno = ERANGE
*/
static inline size_t safe_add(size_t a, size_t b)
{
	if (a > SIZE_MAX - b) {
		errno = ERANGE;
		return SIZE_MAX;
	} else {
		return a + b;
	}
}

/*
smax:
	simple inline function that returns the largest input. Used instead of a 
	macro for type safety

return -> size_t:
	success: largest value _INDEXbetween a and b
*/
static inline size_t smax(size_t a, size_t b)
{
	return (a > b) ? a : b;
}

/* 
smin:
	simple inline function that returns the smallest input. Used instead of a 
	macro for type safety

return -> size_t:
	success: smallest value between a and b
*/
static inline size_t smin(size_t a, size_t b)
{
	return (a < b) ? a : b;
}

/* 
nearest_pow:
	Find the nearest power of two that can contain the given value.
return -> size_t:
	success: The closest power of two larger than num
	range error: SIZE_MAX and errno = ERANGE
 */
static inline size_t nearest_pow(size_t base, size_t num)
{
	// Catch powers of 0, as they will break the loop below
	if (base == 0) return 0;

	// Check if the next pow will overflow
	if (num > SIZE_MAX / 2) {
		errno = ERANGE;
		return SIZE_MAX;
	}
		
	while (base < num) base <<= 1 ;
	return base;
}

/* 
istr_alloc:
	A Helper function that allocates memory for an istring
	and initializes all of it's fields.
return -> istring*:
	success: The pointer to a newly allocated istring
	failure: NULL and errno = ENOMEM
 */
static istring* istr_alloc(size_t init_size)
{
	init_size = smax(2, init_size);

	// The header for the string is two size_t values containing size and length
	istring *string = malloc(HSIZE + sizeof(*string)*init_size);
	if (NULL == string) {
		errno = ENOMEM;
		return NULL;
	}
	string += HSIZE;

	string[LINDEX] = 0;
	string[SINDEX] = init_size;
	string[0] = '\0';

	return string;
}

/* 
istr_realloc:
	A Helper function that reallocates memory for an istring
return -> istring*:
	success: The pointer to a newly allocated istring
	failure: NULL and errno = ENOMEM
 */
static inline istring* istr_realloc(istring *string, size_t target_size) 
{
	if (NULL == string) {
		errno = EINVAL;
		return NULL;
	}

	string = realloc(string - HSIZE, HSIZE + sizeof(*string) * target_size);
	if (NULL == string) {
		errno = ENOMEM;
		return NULL;
	}
	string += HSIZE;
	string[SINDEX] = target_size;
	return string;
}

/* 
istr_ensure_size:
	A Helper function that guarentees to the caller 
	that, if memory can be allocated successfully, the given istring's
	char buffer will be able to hold at least the amount of requested bytes
return -> istring*):
	success: The original string object
	bad args: NULL and errno = EINVAL
	memory error: NULL and errno = ENOMEM
 */
static istring* istr_ensure_size(istring *string, size_t target_size)
{
	if (NULL == string) {
		errno = EINVAL;
		return NULL;
	}

	if (istr_size(string) < target_size) {
		string = istr_realloc(string, nearest_pow(2, target_size));
		if (NULL == string) {
			errno = ENOMEM;
			return NULL;
		}
	}

	return string;
}

// PUBLIC LIBRARY FUNCTIONS BELOW
size_t istr_size(const istring *string)
{
	return (size_t)string[SINDEX];
}

size_t istr_len(const istring *string)
{
	return (size_t)string[LINDEX];
}

istring* istr_new(const istring *src) 
{
	if (NULL == src) return istr_alloc(0);

	istring *string = istr_alloc(istr_len(src));
	if (NULL == string) {
		return NULL;
	}

	return istr_assign_bytes(string, src, istr_len(src));
}

istring* istr_new_bytes(const char *bytes, size_t bytes_len) 
{
	if (NULL == bytes) return istr_alloc(0);

	istring *string = istr_alloc(bytes_len);
	if (NULL == string) {
		return NULL;
	}

	return istr_assign_bytes(string, bytes, bytes_len);
}

istring* istr_new_cstr(const char *cstr) 
{
	if (NULL == cstr) return istr_alloc(0);

	size_t cstr_len = strlen(cstr);

	istring *string = istr_alloc(cstr_len);
	if (NULL == string) {
		return NULL;
	}

	return istr_assign_bytes(string, cstr, cstr_len);
}

istring* istr_grow(istring *string, size_t target_size)
{
	if (NULL == string) {
		errno = EINVAL;
		return NULL;
	}

	if (target_size < istr_size(string)) {
		return string;
	}

	string = istr_ensure_size(string, safe_add(target_size, 1));
	string[istr_len(string)] = '\0';

	return string;
}

istring* istr_shrink(istring *string, size_t target_size)
{
	if (NULL == string) {
		errno = EINVAL;
		return NULL;
	}

	string = istr_realloc(string, target_size);
	if (NULL == string) {
		errno = ENOMEM;
		return NULL;
	}
	string[LINDEX] = smin(istr_len(string), target_size);

	return string;
}

void istr_free(istring *string)
{
	if (NULL == string) {
		errno = EINVAL;
		return;
	}

	free(string-HSIZE);
}

int istr_eq(const istring *s1, const istring *s2)
{
	if (NULL == s1 || NULL == s2) {
		errno = EINVAL;
		return -1;
	}
	
	if (istr_len(s1) != istr_len(s2)) return 1;

	for (size_t i=0; i<istr_len(s1); i++) {
		if (s1[i] != s2[i]) return 1;
	}

	return 0;
}

/*
istring* istr_slice(istring *slice, const istring *src, size_t begin, size_t end)
{
	if (NULL == slice || NULL == src || begin > end) {
		errno = EINVAL;
		return NULL;
	}

	return istr_assign_bytes(slice, src->buf + begin, end - begin);
}
*/

istring* istr_assign(istring *dest, const istring *src)
{
	if (NULL == src) {
		errno = EINVAL;
		return NULL;
	}

	return istr_assign_bytes(dest, src, istr_len(src));
}

istring* istr_assign_cstr(istring *string, const char *cstr)
{
	if (NULL == cstr) {
		errno = EINVAL;
		return NULL;
	}

	return istr_assign_bytes(string, cstr, strlen(cstr));
}

istring* istr_assign_bytes(istring *string, const char *bytes, size_t bytes_len)
{
	if (NULL == string || NULL == bytes) {
		errno = EINVAL;
		return NULL;
	}

	string = istr_ensure_size(string, safe_add(bytes_len, 1));
	if (NULL == string) {
		errno = ENOMEM;
		return NULL;
	}

	// Don't bother memsetting the buffer, just shorten the logical length
	memcpy(string, bytes, bytes_len);
	string[LINDEX] = bytes_len;
	string[bytes_len] = '\0';

	return string;
}

istring* istr_truncate(istring *string, size_t len)
{
	if (NULL == string) {
		errno = EINVAL;
		return NULL;
	}

	string[LINDEX] = smin(istr_len(string), len);
	string[istr_len(string)] = '\0';
	return string;
}

char istr_pop(istring *string)
{
	if (NULL == string) {
		errno = EINVAL;
		return '\0';
	}

	if (istr_len(string) <= 0) {
		errno = ERANGE;
		return '\0';
	}

	string[LINDEX] -= 1;
	string[LINDEX] = '\0';
	return string[istr_len(string) + 1];
}

istring* istr_write(istring *dest, size_t index, const istring *src)
{
	if (NULL == src) {
		errno = EINVAL;
		return NULL;
	}

	return istr_write_bytes(dest, index, src, istr_len(src));
}

istring* istr_write_cstr(istring *string, size_t index, const char *cstr)
{
	if (NULL == cstr) {
		errno = EINVAL;
		return NULL;
	}

	return istr_write_bytes(string, index, cstr, strlen(cstr));
}

istring* istr_write_bytes(istring *string, size_t index, const char *bytes, size_t bytes_len)
{
	if (NULL == string || NULL == bytes) {
		errno = EINVAL;
		return NULL;
	}

	size_t potential_len = safe_add(index, bytes_len);

	if (istr_len(string) > potential_len) {
		potential_len = istr_len(string);
	}

	if (NULL == istr_ensure_size(string, safe_add(potential_len, 1))) {
		errno = ENOMEM;
		return NULL;
	}

	memcpy(string + index, bytes, bytes_len);
	string[LINDEX] = potential_len;
	string[istr_len(string)] = '\0';

	return string;
}
/*

istring* istr_remove_bytes(istring *string, size_t index, size_t len)
{
	if (NULL == string) {
		errno = EINVAL;
		return NULL;
	}

	if (index > ISTR_LEN(string)) {
		errno = ERANGE;
		return NULL;
	}

	// Special case, if bytes would be removed up until the end of the string,
	// then simply truncate the string rather than trying to memmove.
	if (len >= ISTR_LEN(string) - index) {
		return istr_truncate(string, ISTR_LEN(string) - len);
	}

	size_t rm_len = safe_add(index, len);

	memmove(string + index, \
			string + rm_len, \
			ISTR_LEN(string) - rm_len);
	
	ISTR_LEN(string) -= len;
	string[ISTR_LEN(string)] = '\0';

	return string;
}

istring* istr_append(istring *dest, const istring *src)
{
	if (NULL == src) {
		errno = EINVAL;
		return NULL;
	}

	return istr_append_bytes(dest, src->buf, src->len);
}

istring* istr_append_cstr(istring *string, const char *cstr)
{
	if (NULL == cstr) {
		errno = EINVAL;
		return NULL;
	}

	return istr_append_bytes(string, cstr, strlen(cstr));
}

istring* istr_append_bytes(istring *string, const char *bytes, size_t bytes_len)
{
	if (NULL == string || NULL == bytes) {
		errno = EINVAL;
		return NULL;
	}

	return istr_insert_bytes(string, ISTR_LEN(string), bytes, bytes_len);
}

istring* istr_insert(istring *dest, size_t index, const istring *src)
{
	if (NULL == dest || NULL == src) {
		errno = EINVAL;
		return NULL;
	}

	return istr_insert_bytes(dest, index, src->buf, src->len);
}

istring* istr_insert_cstr(istring *string, size_t index, const char *cstr)
{
	if (NULL == cstr) {
		errno = EINVAL;
		return NULL;
	}

	return istr_insert_bytes(string, index, cstr, strlen(cstr));
}

istring* istr_insert_unichar(istring *string, size_t index, int32_t unichar)
{
	size_t unilen;
	if (unichar < 0x80) {
		unilen = 1;
	} else if (unichar < 0x800) {
		unilen = 2;
	} else if (unichar < 0x8000) {
	}
}

istring* istr_insert_bytes(istring *string, size_t index, const char *bytes, size_t bytes_len)
{
	if (NULL == string || NULL == bytes) {
		errno = EINVAL;
		return NULL;
	}

	// Overflow check
	size_t total_len = safe_add(ISTR_LEN(string), bytes_len);

	if (NULL == istr_ensure_size(string, safe_add(total_len, 1))) {
		errno = ENOMEM;
		return NULL;
	}

	if (index < ISTR_LEN(string)) {
		// Create some space for the str to be inserted
		memmove(string + index + bytes_len, \
		        string + index, \
		        ISTR_LEN(string) - index);
	}

	memcpy(string + index, bytes, bytes_len);
	ISTR_LEN(string) = total_len;
	string-[ISTR_LEN(string)] = '\0';

	return string;
}
*/
