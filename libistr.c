/* See LICENSE file for copyright and license details */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "libistr.h"
#include "libistrconfig.h"

// Offsets of header information
// Len offset
#define L_OFFSET (sizeof(ISTR_HEADER_TYPE) * 1)
// Size offset
#define S_OFFSET (sizeof(ISTR_HEADER_TYPE) * 2)
// Beginning of header offset
#define H_OFFSET (sizeof(ISTR_HEADER_TYPE) * 2)

// If a + b would cause an overflow, instead return the maximum size
static inline size_t safe_add(size_t a, size_t b)
{
	if (a > SIZE_MAX - b) {
		return SIZE_MAX - 1;
	} else {
		return a + b;
	}
}

static inline size_t smax(size_t a, size_t b)
{
	return (a > b) ? a : b;
}

static inline size_t smin(size_t a, size_t b)
{
	return (a < b) ? a : b;
}

// Find the nearest power of the given base that can contain at least num
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

// Set the header length information
static inline void istr_set_len(istring *string, size_t len)
{
	*((ISTR_HEADER_TYPE*)(string-L_OFFSET)) = len;
}

// Set the header size information
static inline void istr_set_size(istring *string, size_t size)
{
	*((ISTR_HEADER_TYPE*)(string-S_OFFSET)) = size;
}

/*
 * Either allocs a new istring, or reallocs an existing istring to hold
 * at least target_size. If the istring can already hold the target_size,
 * nothing is done.
 */
static istring* istr_ensure_size(istring *string, size_t target_size)
{
	target_size = nearest_pow(2, safe_add(target_size, 1));

	if (NULL == string) {
		string = ISTR_MALLOC(H_OFFSET + sizeof(*string) * target_size);
	} else if (istr_size(string) < target_size) {
		string = ISTR_REALLOC(string - H_OFFSET, 
		                      H_OFFSET + sizeof(*string) * target_size);
		if (NULL == string) {
			return NULL;
		}
	} else {
		return string;
	}

	string += H_OFFSET;
	istr_set_size(string, target_size);
	return string;
}

// Initializes a new istring with all fields equal to 0
static istring* istr_init(size_t init_size)
{
	init_size = nearest_pow(2, safe_add(init_size, 1));

	// The header for the string is two size_t values containing size and length
	istring *string = istr_ensure_size(NULL, init_size);
	if (NULL == string) {
		return NULL;
	}

	istr_set_len(string, 0);
	string[0] = '\0';

	return string;
}

// Public functions exposed in the header file go below:

size_t istr_size(const istring *string)
{
	return (size_t)*((ISTR_HEADER_TYPE*)(string-S_OFFSET));
}

size_t istr_len(const istring *string)
{
	return (size_t)*((ISTR_HEADER_TYPE*)(string-L_OFFSET));
}

istring* istr_new(const istring *src) 
{
	if (NULL == src) {
		return istr_init(0);
	}
	istring *string = istr_init(istr_len(src));
	if (NULL == string) {
		return NULL;
	}
	return istr_assign_bytes(string, src, istr_len(src));
}

istring* istr_new_bytes(const char *bytes, size_t bytes_len) 
{
	if (NULL == bytes) {
		return istr_init(0);
	}
	istring *string = istr_init(bytes_len);
	if (NULL == string) {
		return NULL;
	}
	return istr_assign_bytes(string, bytes, bytes_len);
}

istring* istr_new_cstr(const char *cstr) 
{
	if (NULL == cstr) {
		return istr_init(0);
	}
	size_t cstr_len = strlen(cstr);
	istring *string = istr_init(cstr_len);
	if (NULL == string) {
		return NULL;
	}
	return istr_assign_bytes(string, cstr, cstr_len);
}

istring* istr_grow(istring *string, size_t target_size)
{
	if (NULL == string) {
		return NULL;
	}

	if (target_size < istr_size(string)) {
		return string;
	}

	string = istr_ensure_size(string, target_size);
	if (NULL == string) {
		return NULL;
	}

	string[target_size] = '\0';
	return string;
}

void istr_free(istring *string)
{
	if (string) ISTR_FREE(string-H_OFFSET);
}

int istr_eq(const istring *s1, const istring *s2)
{
	if (NULL == s1 || NULL == s2) {
		return -1;
	}
	
	if (istr_len(s1) != istr_len(s2)) return 1;

	for (size_t i=0; i<istr_len(s1); i++) {
		if (s1[i] != s2[i]) return 1;
	}

	return 0;
}

istring* istr_slice(istring *slice, const istring *src, size_t begin, size_t end)
{
	if (NULL == slice || NULL == src || begin > end) {
		return NULL;
	}

	return istr_assign_bytes(slice, src + begin, end - begin);
}

istring* istr_assign(istring *dest, const istring *src)
{
	if (NULL == src) {
		return NULL;
	}

	return istr_assign_bytes(dest, src, istr_len(src));
}

istring* istr_assign_cstr(istring *string, const char *cstr)
{
	if (NULL == cstr) {
		return NULL;
	}

	return istr_assign_bytes(string, cstr, strlen(cstr));
}

istring* istr_assign_bytes(istring *string, const char *bytes, size_t bytes_len)
{
	if (NULL == string || NULL == bytes) {
		return NULL;
	}

	string = istr_ensure_size(string, bytes_len);
	if (NULL == string) {
		return NULL;
	}

	// Don't bother memsetting the buffer, just shorten the logical length
	memcpy(string, bytes, bytes_len);
	istr_set_len(string, bytes_len);
	string[bytes_len] = '\0';

	return string;
}

void istr_trunc(istring *string, size_t len)
{
	if (NULL == string) {
		return;
	}

	istr_set_len(string, smin(istr_len(string), len));
	string[istr_len(string)] = '\0';
}

char istr_pop(istring *string)
{
	if (NULL == string) {
		return '\0';
	}

	if (istr_len(string) <= 0) {
		return '\0';
	}

	istr_set_len(string, istr_len(string)-1);
	char ch = string[istr_len(string)];
	string[istr_len(string)] = '\0';
	return ch;
}

// The write_* functions overwrite characters in a istring
istring* istr_write(istring *dest, size_t index, const istring *src)
{
	if (NULL == src) {
		return NULL;
	}

	return istr_write_bytes(dest, index, src, istr_len(src));
}

istring* istr_write_cstr(istring *string, size_t index, const char *cstr)
{
	if (NULL == cstr) {
		return NULL;
	}

	return istr_write_bytes(string, index, cstr, strlen(cstr));
}

istring* istr_write_bytes(istring *string, size_t index, 
                          const char *bytes, size_t bytes_len)
{
	if (NULL == string || NULL == bytes) {
		return NULL;
	}

	size_t potential_len = safe_add(index, bytes_len);

	if (istr_len(string) > potential_len) {
		potential_len = istr_len(string);
	}

	string = istr_ensure_size(string, potential_len);
	if (NULL == string) {
		return NULL;
	}

	memcpy(string + index, bytes, bytes_len);
	istr_set_len(string, potential_len);
	string[istr_len(string)] = '\0';

	return string;
}

/*
istring* istr_remove_unichars()
{
}
*/

void istr_remove_bytes(istring *string, size_t index, size_t remove_len)
{
	if (NULL == string) {
		return;
	}

	if (index > istr_len(string)) {
		return;
	}

	// Special case, if bytes would be removed up until the end of the string,
	// then simply truncate the string rather than trying to memmove.
	if (remove_len >= istr_len(string) - index) {
		istr_trunc(string, istr_len(string) - remove_len);
		return;
	}

	size_t mvlen = safe_add(index, remove_len);

	memmove(string + index, \
			string + mvlen, \
			istr_len(string) - mvlen);
	
	istr_set_len(string, istr_len(string) - remove_len);
	string[istr_len(string)] = '\0';
}

istring* istr_append(istring *dest, const istring *src)
{
	if (NULL == src) {
		return NULL;
	}

	return istr_append_bytes(dest, src, istr_len(src));
}

istring* istr_append_cstr(istring *string, const char *cstr)
{
	if (NULL == cstr) {
		return NULL;
	}

	return istr_append_bytes(string, cstr, strlen(cstr));
}

istring* istr_append_bytes(istring *string, const char *bytes, size_t bytes_len)
{
	if (NULL == string || NULL == bytes) {
		return NULL;
	}

	return istr_insert_bytes(string, istr_len(string), bytes, bytes_len);
}

// The insert_* functions allow insertion of text without overwriting.
istring* istr_insert(istring *dest, size_t index, const istring *src)
{
	if (NULL == dest || NULL == src) {
		return NULL;
	}

	return istr_insert_bytes(dest, index, src, istr_len(src));
}

istring* istr_insert_cstr(istring *string, size_t index, const char *cstr)
{
	if (NULL == cstr) {
		return NULL;
	}

	return istr_insert_bytes(string, index, cstr, strlen(cstr));
}

istring* istr_insert_bytes(istring *string, size_t index, 
                           const char *bytes, size_t bytes_len)
{
	if (NULL == string || NULL == bytes) {
		return NULL;
	}

	// Overflow check
	size_t total_len = safe_add(istr_len(string), bytes_len);

	string = istr_ensure_size(string, total_len);
	if (NULL == string) {
		return NULL;
	}

	if (index < istr_len(string)) {
		// Create some space for the str to be inserted
		// if inserting in the middle or before a string
		//TODO audit for overflow checks here, not fully done
		memmove(string + index + bytes_len, \
		        string + index, \
		        istr_len(string) - index);
	}

	memcpy(string + index, bytes, bytes_len);
	istr_set_len(string, total_len);
	string[istr_len(string)] = '\0';

	return string;
}

/*
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
*/

/*
// Make sure this is unicode friendly!
istring* istr_lower(istring *string)
{
}

// Make sure this is unicode friendly!
istring* istr_upper(istring *string)
{
}
*/

// strip characters from right of string
void istr_rstrip(istring *string, const char *chs)
{
	if (NULL == string || NULL == chs) {
		return;
	}
	size_t len = istr_len(string);
	char *begin = string + smax(1, len) - 1;
	char *end = string;
	while (begin != end && strchr(chs, *begin)) {
		len--;
		begin--;
	}
	istr_set_len(string, len);
	string[len] = '\0';
}

// strip characters from left of string
void istr_lstrip(istring *string, const char *chs)
{
	if (NULL == string || NULL == chs) {
		return;
	}

	size_t len = istr_len(string);
	char *begin = string;
	char *end = string + len;
	while (begin != end && strchr(chs, *begin)) {
		len--;
		begin++;
	}
	if (begin != end) memmove(string, begin, len);
	istr_set_len(string, len);
	string[len] = '\0';
}

// strip characters from both left and right of string
void istr_strip(istring *string, const char *chs)
{
	istr_lstrip(string, chs);
	istr_rstrip(string, chs);
}

// Find the first substring within a string
char* istr_find(istring *string, const char *substr)
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
istring* istr_replace(istring *string, const istring* find, 
                                       const istring* replace)
{
}

istring* istr_replace_cstr(istring *string, const char* find, 
                                            const char* replace)
{
}

istring* istr_replace_bytes(istring *string, const char* find, size_t f_len, 
                                             const char* replace, size_t r_len)
{
}
*/
