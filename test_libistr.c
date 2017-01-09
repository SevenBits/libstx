#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "libistr.h"

int main()
{
	istring *str = istr_new_cstr("hello");

	str = istr_assign_bytes(str, "world", 5);

	printf("%s\n", str);

	istr_free(str);
}
