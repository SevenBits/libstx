/* See LICENSE file for copyright and license details */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "libustr.h"

// Offsets of header information
// len offset
#define L_OFFSET (sizeof(size_t) * 1)
// size offset
#define S_OFFSET (sizeof(size_t) * 2)
// beginning of header offset
#define H_OFFSET (sizeof(size_t) * 2)

/* 
safe_add:
	Safely adds together two size_t values while flagging an error
	if integer overflow would happen.

return -> size_t:
	success: Sum of the two arguments
	range error: SIZE_MAX
*/
static inline size_t safe_add(size_t a, size_t b)
{
	if (a > SIZE_MAX - b) {
		return SIZE_MAX - 1;
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
	range error: SIZE_MAX
 */
static size_t nearest_pow(size_t base, size_t num)
{
	// Catch bases of 0, as they will break the loop below
	if (base == 0) return 0;

	// Check if the next pow will overflow
	if (num >= SIZE_MAX / 2) {
		return SIZE_MAX;
	}

	while (base < num) base <<= 1 ;
	return base;
}

// These are used internally to avoid the mess of setting the header info
static inline void ustr_setlen(ustring *string, size_t len)
{
	*((size_t*)(string-L_OFFSET)) = len;
}

// These are used internally to avoid the mess of setting the header info
static inline void ustr_setsize(ustring *string, size_t size)
{
	*((size_t*)(string-S_OFFSET)) = size;
}

/* 
ustr_realloc:
	A Helper function that reallocates memory for an ustring,
	automatically taking care of header space and setting the new size
return -> ustring*:
	success: The pointer to a newly allocated ustring
	failure: NULL
 */
static ustring* ustr_realloc(ustring *string, size_t size) 
{
	if (NULL == string) {
		string = realloc(NULL, H_OFFSET + sizeof(*string) * size);
	} else {
		string = realloc(string - H_OFFSET, H_OFFSET + sizeof(*string) * size);
	}

	if (NULL == string) {
		return NULL;
	}

	string += H_OFFSET;
	ustr_setsize(string, size);

	return string;
}

/* 
ustr_alloc:
	A Helper function that allocates memory for an ustring
	and initializes all of it's fields.
return -> ustring*:
	success: The pointer to a newly allocated ustring
	failure: NULL
 */
static ustring* ustr_init(size_t init_size)
{
	init_size = nearest_pow(2, smax(2, init_size));

	// The header for the string is two size_t values containing size and length
	ustring *string = ustr_realloc(NULL, init_size);
	if (NULL == string) {
		return NULL;
	}

	ustr_setlen(string, 0);
	string[0] = '\0';

	return string;
}

/* 
ustr_ensure_size:
	A Helper function that guarentees to the caller 
	that, if memory can be allocated successfully, the given ustring's
	char buffer will be able to hold at least the amount of requested bytes
return -> ustring*):
	success: The original string object
	bad args: NULL
	memory error: NULL
 */
static ustring* ustr_ensure_size(ustring *string, size_t target_size)
{
	if (NULL == string) {
		return NULL;
	}

	if (ustr_size(string) < target_size) {
		string = ustr_realloc(string, nearest_pow(2, target_size));
		if (NULL == string) {
			return NULL;
		}
	}

	return string;
}

// PUBLIC LIBRARY FUNCTIONS BELOW
size_t ustr_size(const ustring *string)
{
	return *((size_t*)(string-S_OFFSET));
}

size_t ustr_len(const ustring *string)
{
	return *((size_t*)(string-L_OFFSET));
}

ustring* ustr_new(const ustring *src) 
{
	if (NULL == src) return ustr_init(0);

	// +1 for '\0'
	ustring *string = ustr_init(ustr_len(src) + 1);
	if (NULL == string) {
		return NULL;
	}

	return ustr_assign_bytes(string, src, ustr_len(src));
}

ustring* ustr_new_bytes(const char *bytes, size_t bytes_len) 
{
	if (NULL == bytes) return ustr_init(0);

	// +1 for '\0'
	ustring *string = ustr_init(bytes_len + 1);
	if (NULL == string) {
		return NULL;
	}

	return ustr_assign_bytes(string, bytes, bytes_len);
}

ustring* ustr_new_cstr(const char *cstr) 
{
	if (NULL == cstr) return ustr_init(0);

	size_t cstr_len = strlen(cstr);

	// +1 for '\0'
	ustring *string = ustr_init(cstr_len + 1);
	if (NULL == string) {
		return NULL;
	}

	return ustr_assign_bytes(string, cstr, cstr_len);
}

ustring* ustr_grow(ustring *string, size_t target_size)
{
	if (NULL == string) {
		return NULL;
	}

	if (target_size < ustr_size(string)) {
		return string;
	}

	string = ustr_ensure_size(string, safe_add(target_size, 1));
	if (NULL == string) {
		return NULL;
	}

	string[target_size] = '\0';
	return string;
}

ustring* ustr_shrink(ustring *string, size_t target_size)
{
	if (NULL == string) {
		return NULL;
	}

	string = ustr_realloc(string, target_size);
	if (NULL == string) {
		return NULL;
	}

	ustr_setlen(string, smin(ustr_len(string), target_size));
	string[ustr_len(string)] = '\0';

	return string;
}

void ustr_free(ustring *string)
{
	if (string) free(string-H_OFFSET);
}

int ustr_eq(const ustring *s1, const ustring *s2)
{
	if (NULL == s1 || NULL == s2) {
		return -1;
	}
	
	if (ustr_len(s1) != ustr_len(s2)) return 1;

	for (size_t i=0; i<ustr_len(s1); i++) {
		if (s1[i] != s2[i]) return 1;
	}

	return 0;
}

/*
ustring* ustr_slice(ustring *slice, const ustring *src, size_t begin, size_t end)
{
	if (NULL == slice || NULL == src || begin > end) {
		return NULL;
	}

	return ustr_assign_bytes(slice, src->buf + begin, end - begin);
}
*/

ustring* ustr_assign(ustring *dest, const ustring *src)
{
	if (NULL == src) {
		return NULL;
	}

	return ustr_assign_bytes(dest, src, ustr_len(src));
}

ustring* ustr_assign_cstr(ustring *string, const char *cstr)
{
	if (NULL == cstr) {
		return NULL;
	}

	return ustr_assign_bytes(string, cstr, strlen(cstr));
}

ustring* ustr_assign_bytes(ustring *string, const char *bytes, size_t bytes_len)
{
	if (NULL == string || NULL == bytes) {
		return NULL;
	}

	string = ustr_ensure_size(string, safe_add(bytes_len, 1));
	if (NULL == string) {
		return NULL;
	}

	// Don't bother memsetting the buffer, just shorten the logical length
	memcpy(string, bytes, bytes_len);
	ustr_setlen(string, bytes_len);
	string[bytes_len] = '\0';

	return string;
}

void ustr_trunc(ustring *string, size_t len)
{
	if (NULL == string) {
		return;
	}

	ustr_setlen(string, smin(ustr_len(string), len));
	string[ustr_len(string)] = '\0';
}

char ustr_pop(ustring *string)
{
	if (NULL == string) {
		return '\0';
	}

	if (ustr_len(string) <= 0) {
		return '\0';
	}

	ustr_setlen(string, ustr_len(string)-1);
	char ch = string[ustr_len(string)];
	string[ustr_len(string)] = '\0';
	return ch;
}

ustring* ustr_write(ustring *dest, size_t index, const ustring *src)
{
	if (NULL == src) {
		return NULL;
	}

	return ustr_write_bytes(dest, index, src, ustr_len(src));
}

ustring* ustr_write_cstr(ustring *string, size_t index, const char *cstr)
{
	if (NULL == cstr) {
		return NULL;
	}

	return ustr_write_bytes(string, index, cstr, strlen(cstr));
}

ustring* ustr_write_bytes(ustring *string, size_t index, const char *bytes, size_t bytes_len)
{
	if (NULL == string || NULL == bytes) {
		return NULL;
	}

	size_t potential_len = safe_add(index, bytes_len);

	if (ustr_len(string) > potential_len) {
		potential_len = ustr_len(string);
	}

	string = ustr_ensure_size(string, safe_add(potential_len, 1));
	if (NULL == string) {
		return NULL;
	}

	memcpy(string + index, bytes, bytes_len);
	ustr_setlen(string, potential_len);
	string[ustr_len(string)] = '\0';

	return string;
}

/*
ustring* ustr_remove_unichars()
{
}
*/

void ustr_remove_bytes(ustring *string, size_t index, size_t remove_len)
{
	if (NULL == string) {
		return;
	}

	if (index > ustr_len(string)) {
		return;
	}

	// Special case, if bytes would be removed up until the end of the string,
	// then simply truncate the string rather than trying to memmove.
	if (remove_len >= ustr_len(string) - index) {
		ustr_trunc(string, ustr_len(string) - remove_len);
		return;
	}

	size_t mvlen = safe_add(index, remove_len);

	memmove(string + index, \
			string + mvlen, \
			ustr_len(string) - mvlen);
	
	ustr_setlen(string, ustr_len(string) - remove_len);
	string[ustr_len(string)] = '\0';
}

ustring* ustr_append(ustring *dest, const ustring *src)
{
	if (NULL == src) {
		return NULL;
	}

	return ustr_append_bytes(dest, src, ustr_len(src));
}

ustring* ustr_append_cstr(ustring *string, const char *cstr)
{
	if (NULL == cstr) {
		return NULL;
	}

	return ustr_append_bytes(string, cstr, strlen(cstr));
}

ustring* ustr_append_bytes(ustring *string, const char *bytes, size_t bytes_len)
{
	if (NULL == string || NULL == bytes) {
		return NULL;
	}

	return ustr_insert_bytes(string, ustr_len(string), bytes, bytes_len);
}

ustring* ustr_insert(ustring *dest, size_t index, const ustring *src)
{
	if (NULL == dest || NULL == src) {
		return NULL;
	}

	return ustr_insert_bytes(dest, index, src, ustr_len(src));
}

ustring* ustr_insert_cstr(ustring *string, size_t index, const char *cstr)
{
	if (NULL == cstr) {
		return NULL;
	}

	return ustr_insert_bytes(string, index, cstr, strlen(cstr));
}

/*
ustring* ustr_insert_unichar(ustring *string, size_t index, int32_t unichar)
{
	size_t unilen;
	if (unichar < 0x80) {
		unilen = 1;
	} else if (unichar < 0x800) {
		unilen = 2;
	} else if (unichar < 0x8000) {
	}
}
*/

ustring* ustr_insert_bytes(ustring *string, size_t index, const char *bytes, size_t bytes_len)
{
	if (NULL == string || NULL == bytes) {
		return NULL;
	}

	// Overflow check
	size_t total_len = safe_add(ustr_len(string), bytes_len);

	string = ustr_ensure_size(string, safe_add(total_len, 1));
	if (NULL == string) {
		return NULL;
	}

	if (index < ustr_len(string)) {
		// Create some space for the str to be inserted
		// if inserting in the middle or before a string
		//TODO audit for overflow checks here, not fully done
		memmove(string + index + bytes_len, \
		        string + index, \
		        ustr_len(string) - index);
	}

	memcpy(string + index, bytes, bytes_len);
	ustr_setlen(string, total_len);
	string[ustr_len(string)] = '\0';

	return string;
}

/*
// Make sure this is unicode friendly!
ustring* ustr_lower(ustring *string)
{
}

// Make sure this is unicode friendly!
ustring* ustr_upper(ustring *string)
{
}
*/

// strip characters from right of string
void ustr_rstrip(ustring *string, const char *chs)
{
	size_t len = ustr_len(string);
	char *right = string + len - 1;

	while(right != string) {
		const char *cptr = chs;
		while (*cptr) {
			if (*right == *cptr) {
				len--;
				break;
			}
			cptr++;
		}
		// No characters matched, stop
		if ('\0' == *cptr) break;

		right--;
	}
	ustr_setlen(string, len);
	string[len] = '\0';
}

// strip characters from left of string
void ustr_lstrip(ustring *string, const char *chs)
{
	size_t len = ustr_len(string);
	char *left = string;
	char *right = string + len - 1;

	while(left != right) {
		const char *cptr = chs;
		while ('\0' != *cptr) {
			if (*left == *cptr) {
				len--;
				break;
			}
			cptr++;
		}
		// No characters matched, stop
		if ('\0' == *cptr) break;

		left++;
	}
	// If left characters were stripped, then move the string back
	if (left != string) memmove(string, left, len);
	ustr_setlen(string, len);
	string[len] = '\0';
}

// strip characters from both left and right of string
void ustr_strip(ustring *string, const char *chs)
{
	ustr_lstrip(string, chs);
	ustr_rstrip(string, chs);
}

// Find a substring withing the ustring
char* ustr_find(ustring *string, const char *substr)
{
	if (NULL == string || NULL == substr) {
		return NULL;
	}
	
	while (*string) {
		char *begin = string;
		const char *token = substr;
		while (*begin && *token && *begin == *token) {
			begin++;
			token++;
		}
		// If the token reached the end of the substr, it has been found.
		if (!*token) {
			return string;
		}
		string++;
	}
	// No substr could be found
	return NULL;
}

/*
// Replace a substring
ustring* ustr_replace(ustring *string, const char *find, const char *replace)
{
	return NULL;
}
*/
