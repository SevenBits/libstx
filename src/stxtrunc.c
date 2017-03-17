// See LICENSE file for copyright and license details
#include "internal.h"

char
stxtrunc(stx *s1, size_t len)
{
	char ret;

	if (len > s1->len) {
		stxterm(s1, 0);
		return '\0';
	}

	if (0 == s1->len)
		return '\0';

	ret = s1->mem[s1->len - 1];
	stxterm(s1, s1->len - len);
	return ret;
}
