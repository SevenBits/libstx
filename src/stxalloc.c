// See LICENSE file for copyright and license details
#include "internal.h"

int
stxalloc(stx *sp, size_t n)
{
	sp->len = 0;

	// Always set the memory location to NULL, don't let the implementation
	// of malloc decide what to do.
	if (0 == n) {
		sp->mem = NULL; 
		sp->size = 0;
		return 0;
	}

	if (!(sp->mem = malloc(n)))
		return -1;

	sp->size = n;

	return 0;
}
