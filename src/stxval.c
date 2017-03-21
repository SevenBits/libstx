// See LICENSE file for copyright and license details
#include "internal.h"

stx *
stxvalid(stx *s1)
{
	if (!s1)
		return NULL;
	if (!s1->mem || s1->size < s1->len)
		return NULL;

	return s1;
}
