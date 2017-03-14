// See LICENSE file for copyright and license details
#include "internal.h"

int
oystr_write(struct oystr *s1, size_t pos, const char *bytes, size_t len)
{
	size_t newlen;

	if (pos > s1->len) {
		if (overflow_size_add(pos, len))
			return OYSTR_OVERFLOW;
		newlen = pos + len;
	} else {
		if (overflow_size_add(s1->len - pos, len))
			return OYSTR_OVERFLOW;
		newlen = (s1->len - pos + len) + 1;
	}

	int err;
	if (0 != (err = oystr_ensure_size(s1, newlen)))
		return err;

	memcpy(s1->buf + pos, bytes, len);
	oystr_setlen(s1, newlen);

	return OYSTR_OK;
}
