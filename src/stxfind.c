// See LICENSE file for copyright and license details
#include "internal.h"

stx *
stxfind(stx *slice, stx *haystack, const char *needle, size_t len)
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
			return NULL;
		}
	}
	return slice;
}
