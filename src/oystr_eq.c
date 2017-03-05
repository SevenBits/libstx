// See LICENSE file for copyright and license details
#include "internal.h"

bool
oystr_eq(const struct oystr *s1, const struct oystr *s2)
{
	if (s1->len != s2->len)
		return false;

	for (size_t i=0; i<s1->len; ++i) {
		if (s1->buf[i] != s2->buf[i])
			return false;
	}

	return true;
}
