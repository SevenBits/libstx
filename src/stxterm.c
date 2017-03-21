// See LICENSE file for copyright and license details
#include "internal.h"

stx *
stxterm(stx *sp, size_t len)
{
	sp->len = len;
	sp->mem[sp->len]= '\0';
	return sp;
}
