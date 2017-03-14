// See LICENSE file for copyright and license details
#include "internal.h"

int
oystr_setlen(struct oystr *s1, size_t len)
{
	if (len > (s1->size ? s1->size - 1 : 0)) {
			return OYSTR_ERR;
	}
	s1->len = len;
	s1->buf[s1->len]= '\0';
	return OYSTR_OK;
}
