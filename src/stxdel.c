// See LICENSE file for copyright and license details
#include "internal.h"

stx *
stxdel(stx *s1)
{
	free(s1->mem);
	memset(s1, 0, sizeof(*s1));
	return s1;
}
