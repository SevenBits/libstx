/* See LICENSE file for copyright and license details */
#ifndef LIBSTX_H
#define LIBSTX_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __STDC__
#if (__STDC_VERSION__ == 201112L)
#define stxcpy(sp, src) _Generic((src), \
		const char *: stxcpy_str, \
		char *: stxcpy_str, \
		spx: stxcpy_spx)(sp, src)

#define stxins(sp, src) _Generic((src), \
		const char *: stxins_str, \
		char *: stxins_str, \
		uint32_t: stxins_uni, \
		spx: stxins_spx)(sp, src)

#define stxapp(sp, src) _Generic((src), \
		const char *: stxapp_str, \
		char *: stxapp_str, \
		uint32_t: stxapp_uni, \
		spx: stxapp_spx)(sp, src)

#define stxfind(sp, src) _Generic((src), \
		const char *: stxfind_str, \
		char *: stxfind_str, \
		spx: stxfind_spx)(sp, src)

#define stxdup(sp, src) _Generic((src), \
		const char *: stxdup_str, \
		char *: stxdup_str, \
		spx: stxdup_spx)(sp, src)
#endif
#endif

/**
 * Dynamic and modifiable string data structure. Contents are modifiable and
 * contains both the size of the memory, and how much is being used.
 */
struct stx {
	size_t len;
	size_t size;
	char *mem;
};

/**
 * String slice data structure. Contains a read-only reference to a character
 * array and the length of the character array. Created by calling stxref() on
 * a struct stx.
 */
struct spx {
	size_t len;
	const char *mem;
};

typedef struct stx stx;
typedef struct spx spx;

// Initialize and allocate a new stx.
int stxalloc(stx *sp, size_t n);

// Free all memory used by a stx. Memory must be reinitalized afterwards.
void stxfree(const stx *sp);

// Grow a stx by n bytes.
int stxgrow(stx *sp, size_t n);
// Grow a stx to n bytes if it isn't n size already. Otherwise do nothing.
int stxensuresize(stx *sp, size_t n);
// Validate a stx in the case where some of it's internal data might have been
// changed incorrectly.
bool stxvalid(stx *sp);

// Get the amount of unused space left in a stx.
size_t stxavail(stx *sp);

// Compare two slices for equality, stx's can be compared by turning them into
// references first.
bool stxcmp(spx s1, spx s2);

void stxswap(stx *s1, stx *s2);
stx *stxtrunc(stx *sp, size_t n);
stx *stxterm(stx *sp);

// Create a spx reference from a stx.
spx stxref(const stx *sp);

// Allocte a stx and copy the context of "src" into it.
int stxdup_mem(stx *sp, const void *src, size_t n);
int stxdup_str(stx *sp, const char *src);
int stxdup_spx(stx *sp, const spx src);

// Copy bytes from "src" into a stx.
stx *stxcpy_mem(stx *sp, const void *src, size_t n);
stx *stxcpy_str(stx *sp, const char *src);
stx *stxcpy_spx(stx *sp, const spx src);

// Insert bytes into the middle of stx without overwriting any data.
stx *stxins_mem(stx *sp, size_t pos, const void *src, size_t n);
stx *stxins_str(stx *sp, size_t pos, const char *src);
stx *stxins_utf8f32(stx *sp, size_t pos, uint32_t wc);
stx *stxins_spx(stx *sp, size_t pos, const spx src);

// Append bytes to a stx.
stx *stxapp_mem(stx *sp, const void *src, size_t n);
stx *stxapp_str(stx *sp, const char *src);
stx *stxapp_utf8f32(stx *sp, uint32_t wc);
stx *stxapp_spx(stx *sp, const spx src);

// Find a substring inside a stx and return it as a spx referring to it.
spx stxfind_mem(const spx haystack, const void *needle, size_t n);
spx stxfind_str(const spx haystack, const char *needle);
spx stxfind_spx(const spx haystack, const spx needle);

// Slice a substring inside a spx and return it as a spx referring to it.
spx stxslice(const spx sp, size_t begin, size_t end);

// Strip characters from a stx.
stx *stxrstrip(stx *sp, const char *chs, size_t n);
stx *stxlstrip(stx *sp, const char *chs, size_t n);
stx *stxstrip(stx *sp, const char *chs, size_t n);

// Tokenize a spx.
spx stxtok(spx *sp, const char *chs, size_t n);

// Calculate the number of utf8 encodings in a spx.
size_t stxutf8len(const spx sp);
// Calculate the number of bytes for a utf8 encoding of a utf32 code point.
size_t stxutf8n32(uint32_t wc);
// Convert a "wc" into a utf8 encoding "n" bytes long and store it in "dst".
size_t stxutf8f32(void *dst, uint32_t wc, size_t n);

#endif
