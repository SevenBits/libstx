/* See LICENSE file for copyright and license details */
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "libistr.h"

static const size_t LEN_INDEX = -(sizeof(size_t));
static const size_t SIZE_INDEX = -(sizeof(size_t) * 2);
static const size_t HEADER_SIZE = sizeof(size_t) * 2;

static inline size_t getsize(const istring *string)
{
	return (size_t)string[SIZE_INDEX];
}

static inline size_t getlen(const istring *string)
{
	return (size_t)string[LEN_INDEX];
}

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

	if (getsize(string) < target_size) {
		size_t next_size = nearest_pow(2, target_size);
		istring *rbuf = realloc(string-HEADER_SIZE, HEADER_SIZE +
					            sizeof(*string) * next_size);
		if (NULL == rbuf) {
			errno = ENOMEM;
			return NULL;
		}
		rbuf += HEADER_SIZE;
		string = rbuf;
		string[SIZE_INDEX] = next_size;
	}

	return string;
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
	istring *string = malloc(HEADER_SIZE + sizeof(*string)*init_size);
	if (NULL == string) {
		errno = ENOMEM;
		return NULL;
	}
	string += HEADER_SIZE;

	string[LEN_INDEX] = 0;
	string[SIZE_INDEX] = init_size;

	return string;
}

// PUBLIC LIBRARY FUNCTIONS BELOW
istring* istr_new(const istring *src) 
{
	if (NULL == src) return istr_alloc(0);

	istring *string = istr_alloc(getlen(src));
	if (NULL == string) {
		return NULL;
	}

	return istr_assign_bytes(string, src, getlen(src));
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

/*
istring* istr_grow(istring *string, size_t target_size)
{
	if (NULL == string) {
		errno = EINVAL;
		return NULL;
	}

	if (target_size < *ISTR_LEN(string)) {
		return string;
	}

	istr_ensure_size(string, target_size+1);

	return string;
}

istring* istr_shrink(istring *string, size_t len)
{
	if (NULL == string) {
		errno = EINVAL;
		return NULL;
	}

	if (len > *ISTR_LEN(string)) {
		return string;
	}

	char *rbuf = realloc(string, sizeof(*(string)) * (len));
	if (NULL == rbuf) {
		errno = ENOMEM;
		return NULL;
	}
	string = rbuf;

	return string;
}
*/

void istr_free(istring *string)
{
	if (NULL == string) {
		errno = EINVAL;
		return;
	}

	free(string-HEADER_SIZE);
}

/*
int istr_eq(const istring *s1, const istring *s2)
{
	if (NULL == s1 || NULL == s2) {
		errno = EINVAL;
		return -1;
	}
	
	if (s1->len != s2->len) return 1;

	for (size_t i=0; i<s1->len; i++) {
		if (s1->buf[i] != s2->buf[i]) return 1;
	}

	return 0;
}

char istr_index(const istring *string, size_t index)
{
	if (NULL == string) {
		errno = EINVAL;
		return '\0';
	}

	if (index > ISTR_LEN(string)) {
		errno = ERANGE;
		return '\0';
	}

	return string[index];
}

istring* istr_slice(istring *slice, const istring *src, size_t begin, size_t end)
{
	if (NULL == slice || NULL == src || begin > end) {
		errno = EINVAL;
		return NULL;
	}

	return istr_assign_bytes(slice, src->buf + begin, end - begin);
}

*/
istring* istr_assign_bytes(istring *string, const char *bytes, size_t bytes_len)
{
	if (NULL == string || NULL == bytes) {
		errno = EINVAL;
		return NULL;
	}

	istring *rbuf = istr_ensure_size(string, bytes_len + 1);
	if (NULL == rbuf) {
		errno = ENOMEM;
		return NULL;
	}
	string = rbuf;

	// Don't bother memsetting the buffer, just shorten the logical length
	memcpy(string, bytes, bytes_len);
	string[LEN_INDEX] = bytes_len;
	string[bytes_len] = '\0';

	return string;
}
/*

istring* istr_assign_cstr(istring *string, const char *cstr)
{
	if (NULL == cstr) {
		errno = EINVAL;
		return NULL;
	}

	return istr_assign_bytes(string, cstr, strlen(cstr));
}

istring* istr_truncate(istring *string, size_t len)
{
	if (NULL == string) {
		errno = EINVAL;
		return NULL;
	}

	ISTR_LEN(string) = smin(ISTR_LEN(string), len);
	string[ISTR_LEN(string)] = '\0';
	return string;
}

char istr_pop_byte(istring *string)
{
	if (NULL == string) {
		errno = EINVAL;
		return '\0';
	}

	if (ISTR_LEN(string) <= 0) {
		errno = ERANGE;
		return '\0';
	}

	ISTR_LEN(string) -= 1;
	return string[ISTR_LEN(string) + 1];
}

istring* istr_write(istring *string, size_t index, const istring *ext)
{
	if (NULL == ext) {
		errno = EINVAL;
		return NULL;
	}

	return istr_write_bytes(string, index, ext->buf, ext->len);
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

	if (ISTR_LEN(string) > potential_len) {
		potential_len = ISTR_LEN(string);
	}

	if (NULL == istr_ensure_size(string, safe_add(potential_len, 1))) {
		errno = ENOMEM;
		return NULL;
	}

	memcpy(string + index, bytes, bytes_len);
	ISTR_LEN(string) = potential_len;
	string[ISTR_LEN(string)] = '\0';

	return string;
}

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
