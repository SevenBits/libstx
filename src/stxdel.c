// See LICENSE file for copyright and license details
#include "internal.h"

void
stxdel(const stx *s1)
{
	free(s1->mem);
}
