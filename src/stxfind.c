// See LICENSE file for copyright and license details
#include "internal.h"

stx *
stxfind_mem(stx *slice, stx *haystack, const char *needle, size_t len)
{
	size_t i, j;

	if (0 == len)
		return NULL;

	if (haystack->len < len)
		return NULL;

	for (i=0; i<haystack->len; ++i) {
		for (j=i; j<i+len; ++j) {
			if (haystack->mem[j] != needle[j-i]) {
				break;
			}
		}

		if (j-i == len) {
			stxslice(slice, haystack, i, j);
			return slice;
		}
	}
	return NULL;
}

stx *
stxfind_str(stx *slice, stx *haystack, const char *needle)
{
	return stxfind_mem(slice, haystack, needle, strlen(needle));
}

stx *
stxfind_stx(stx *slice, stx *sp, const stx *src)
{
	return stxfind_mem(slice, sp, src->mem, src->len);
}
