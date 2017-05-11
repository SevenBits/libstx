// See LICENSE file for copyright and license details
#include "internal.h"

spx
stxtok(spx *sp, const char *chs, size_t n)
{
	spx tok = *sp;
	size_t i, j;

	for (i=0; i<sp->len; ++i) {
		for (j=i; j<i+n; ++j) {
			if (sp->mem[j] != chs[j-i]) {
				break;
			}
		}

		if (j-i == n) {
			// Create the token.
			tok.mem = sp->mem;
			tok.len = i;
			// Move the reference forward.
			sp->mem += i + n;
			sp->len -= i + n;

			break;
		}
	}

	return tok;
}
