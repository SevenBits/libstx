#include "internal.h"

stx *
stxterm(stx *sp)
{
	sp->mem[sp->len] = '\0';
	++sp->len;
	return sp;
}
