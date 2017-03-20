// See LICENSE file for copyright and license details
#include "internal.h"

int
stxterm(stx *sp, size_t len)
{
	if (len > (sp->size ? sp->size - 1 : 0)) {
		return -1;
	}
	sp->len = len;
	sp->mem[sp->len]= '\0';
	return 0;
}
