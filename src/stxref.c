// See LICENSE file for copyright and license details
#include "internal.h"

spx
stxref(const stx *sp)
{
	spx tmp = {.mem = sp->mem, .len = sp->len};
	return tmp;
}
