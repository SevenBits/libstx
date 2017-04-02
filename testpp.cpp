#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "libstx.hpp"

/**
 * This test file simply tests to make sure that the C++ header compiles
 * correctly and that all the C++ exclusive features are working properly.
 * Tests for testing functionality of the library should not go in this file.
 */

int main(void)
{
	printf("Testing C++ wrapper ... ");
	fflush(stdout);
	libstx::stx s1, s2, slice;

	libstx::alloc(&s1, 10);
	libstx::grow(&s1, 30);

	libstx::alloc(&s2, 10);
	libstx::grow(&s2, 30);

	libstx::cpy(&s2, "world");
	libstx::cpy(&s1, "hello", 6);
	libstx::cpy(&s1, &s2);

	libstx::app(&s1, " ");
	libstx::app(&s1, " ", 1);
	libstx::app(&s1, &s2);

	libstx::find(&slice, &s1, "world");
	libstx::find(&slice, &s1, "hello", 5);
	libstx::find(&slice, &s1, &s2);

	libstx::eq(&s1, &s2);
	printf("success!\n");
}
