// See LICENSE file for copyright and license details
#include "internal.h"

int
stxdup(stx *dst, stx *src)
{
	stxnew(dst, src->len);
	stxcpy(dst, src->mem, src->len);
	return 0;
}
