#include "internal.h"

stx *
stxterm(stx *sp)
{
	if (sp->len < sp->size)
		sp->mem[sp->len] = '\0';

	return sp;
}
