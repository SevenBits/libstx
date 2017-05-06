// See LICENSE file for copyright and license details
#include "internal.h"

int
stxdup_mem(stx *sp, const char *src, size_t n)
{
	if (0 < stxalloc(sp, n))
		return -1;

	stxcpy_mem(sp, src, n);

	return 0;
}

int
stxdup_str(stx *sp, const char *src)
{
	if (0 < stxalloc(sp, strlen(src)))
		return -1;

	stxcpy_str(sp, src);

	return 0;
}

int
stxdup_spx(stx *sp, const spx src)
{
	if (0 < stxalloc(sp, src.len))
		return -1;

	stxcpy_spx(sp, src);

	return 0;
}
