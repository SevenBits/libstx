// See LICENSE file for copyright and license details
#include "internal.h"

bool
oystr_valid(struct oystr *s1)
{
	if (!s1)
		return false;
	if (!s1->buf || s1->size < s1->len)
		return false;

	return true;
}
