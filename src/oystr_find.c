// See LICENSE file for copyright and license details
#include "internal.h"

bool
oystr_find(struct oystr *slice, struct oystr *s1, const char *buf, size_t len)
{
	size_t i, j;

	if (0 == len)
		goto not_found;

	if (s1->len < len)
		goto not_found;

	for (i=0; i<s1->len; ++i) {
		for (j=i; j<i+len; ++j) {
			if (s1->buf[j] != buf[j-i]) {
				break;
			}
		}

		if (j-i == len)
			goto found;
	}

not_found:
	return false;

found:
	oystr_slice(slice, s1, i, j);
	return true;
}
