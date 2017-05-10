#include "internal.h"

stx *
stxterm(stx *sp, size_t len)
{
	sp->mem[len] = '\0';
	sp->len = len;
	return sp;
}
