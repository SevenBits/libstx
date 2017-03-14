// See LICENSE file for copyright and license details
#include "internal.h"

char
oystr_trunc(struct oystr *s1, size_t len)
{
	if (len > s1->len) {
		oystr_setlen(s1, 0);
		return '\0';
	}

	if (0 == s1->len)
		return '\0';

	char ret = s1->buf[s1->len - 1];
	oystr_setlen(s1, s1->len - len);

	return ret;
}
