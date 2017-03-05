// See LICENSE file for copyright and license details
#include "internal.h"

void
oystr_deinit(struct oystr *s1)
{
	free(s1->buf);
	s1->buf = NULL;
	s1->size = 0;
	s1->len = 0;
}
