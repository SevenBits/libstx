// See LICENSE file for copyright and license details
#include "internal.h"

spx
stxslice(const spx src, size_t begin, size_t end)
{
	spx slice = {
		.mem = src.mem + begin,
		.len = end - begin
	};

	return slice;
}
