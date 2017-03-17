// See LICENSE file for copyright and license details
#include "internal.h"

int
stxcpy(stx *sp, char *src, size_t len)
{
	int err;
	if (sp->len < len) { 
		err = stxgrow(sp, len - sp->len + 1);
		if (err)
			return err;
	}
	memcpy(sp, src, len)
	stxterm(sp, len);
	return 0;
}
