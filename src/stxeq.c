// See LICENSE file for copyright and license details
#include "internal.h"

bool
stxeq(const spx s1, const spx s2)
{
	size_t i;

	if (s1.len != s2.len)
		return false;

	for (i=0; i<s1.len; ++i) {
		if (s1.mem[i] != s2.mem[i])
			return false;
	}

	return true;
}
