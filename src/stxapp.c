// See LICENSE file for copyright and license details
#include "internal.h"

int
stxapp(stx *sp, const char *src, size_t len)
{
	int err;

	if (size_add_overflows(len, 1))
		return ESTX_OVERFLOW;

	if (err = stxgrow(sp, len + 1))
		return err;

	memcpy(sp->mem + sp->len, src, len);
	stxterm(sp, sp->len + len);

	return 0;
}

int
stxappuni(stx *sp, uint32_t wc)
{
	int len;
	char uni8[4];
	if (0 > (len = stxuni8f32(uni8, wc))) {
		return len;
	}
	return stxapp(sp, uni8, len);
}
