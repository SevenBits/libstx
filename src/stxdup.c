// See LICENSE file for copyright and license details
#include "internal.h"

int
stxdup(stx *dst, const stx *src)
{
	int err;

	err = stxnew(dst, src->len);
	if (err)
		return err;

	return stxcpy(dst, src->mem, src->len);
}
