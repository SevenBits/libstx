// See LICENSE file for copyright and license details
#include "internal.h"

/**
 * NOTE: 'n' must be greater than zero, otherwise behavior is undefined.
 */
int
stxnew(stx *sp, size_t n)
{
	int err;

	memset(sp, 0, sizeof(*sp));
	err = stxgrow(sp, n);
	if (err)
		return err;
	*sp->mem = '\0';

	return 0;
}
