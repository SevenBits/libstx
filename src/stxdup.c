// See LICENSE file for copyright and license details
#include "internal.h"

stx *
stxdup(stx *dst, const stx *src)
{
	dst = stxnew(dst, src->len);
	return stxcpy(dst, src->mem, src->len);
}
