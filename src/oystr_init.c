// See LICENSE file for copyright and license details
#include "internal.h"

int
oystr_init_buf(struct oystr *s1, size_t size)
{
	int err;

	oystr_init(s1);

	if (0 != (err = oystr_ensure_size(s1, size)))
		return err;

	s1->buf[s1->len] = '\0';
	return OYSTR_OK;
}
	
void
oystr_init(struct oystr *s1)
{
	s1->buf = NULL;
	s1->len = 0;
	s1->size = 0;
}
