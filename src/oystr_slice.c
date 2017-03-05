// See LICENSE file for copyright and license details
#include "internal.h"

void
oystr_slice(struct oystr *slice, struct oystr *s1, size_t begin, size_t end)
{
	slice->buf = s1->buf + begin;
	slice->size = s1->size - begin;
	slice->len = end - begin;
}
