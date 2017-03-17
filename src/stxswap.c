// See LICENSE file for copyright and license details
#include "internal.h"

void
stxswap(stx *s1, stx *s2)
{
	stx tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}
