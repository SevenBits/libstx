// See LICENSE file for copyright and license details
#include "internal.h"

int
stxnew(stx *s1, size_t size)
{
	int err;

	memset(s1, 0, sizeof(*s1));

	if (size_add_overflows(size, 1))
		return ESTX_OVERFLOW;

	err = stxgrow(s1, size + 1);
	if (err)
		return err;

	*s1->mem = '\0';
	return 0;
}
