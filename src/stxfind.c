// See LICENSE file for copyright and license details
#include "internal.h"

spx
stxfind_mem(const spx haystack, const void *needle, size_t len)
{
	spx slice = {0};
	size_t i, j;

	if (0 == len)
		return slice;

	if (haystack.len < len)
		return slice;

	for (i=0; i<haystack.len; ++i) {
		for (j=i; j<i+len; ++j) {
			if (haystack.mem[j] != ((const char *)needle)[j-i]) {
				break;
			}
		}

		if (j-i == len) {
			return stxslice(haystack, i, j);
		}
	}
	return slice;
}

spx
stxfind_str(const spx haystack, const char *needle)
{
	return stxfind_mem(haystack, needle, strlen(needle));
}

spx
stxfind_stx(const spx haystack, const stx *needle)
{
	return stxfind_mem(haystack, needle->mem, needle->len);
}

spx
stxfind_spx(const spx haystack, const spx needle)
{
	return stxfind_mem(haystack, needle.mem, needle.len);
}
