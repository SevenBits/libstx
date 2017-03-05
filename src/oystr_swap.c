// See LICENSE file for copyright and license details
#include "internal.h"

void
oystr_swap(struct oystr *s1, struct oystr *s2)
{
	struct oystr tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}
