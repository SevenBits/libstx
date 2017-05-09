// See LICENSE file for copyright and license details
#include "internal.h"

bool
stxvalid(stx *s1)
{
	if (!s1)
		return false;
	if (!s1->mem || s1->size < s1->len)
		return false;

	return true;
}
