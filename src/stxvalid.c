// See LICENSE file for copyright and license details
#include "internal.h"

int
stxvalid(stx *s1)
{
	if (!s1)
		return -1;
	if (!s1->mem || s1->size < s1->len)
		return -1;

	return 0;
}
