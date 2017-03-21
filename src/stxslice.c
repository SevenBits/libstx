// See LICENSE file for copyright and license details
#include "internal.h"

stx *
stxslice(stx *slice, stx *src, size_t begin, size_t end)
{
	slice->mem = src->mem + begin;
	slice->size = src->size - begin;
	slice->len = end - begin;
	return slice;
}
