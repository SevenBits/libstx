// See LICENSE file for copyright and license details
#include "internal.h"

size_t
stxavail(stx *sp)
{
	return sp->size ? sp->size - sp->len : 0;
}
