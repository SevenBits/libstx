libistr - Improved String Library
========

### A dynamic string library for C

libistr is a simple dynamic string handling library for C that conforms with C99.
The main goals of the library are strict error handling, function clarity, and
preventing integer and buffer overflows, or resolving them if they would occur. 
To allow for easy interoperability with other string libraries that use strings
terminated by '\0', such as `<string.h>`, all istrings are guaranteed by all
libistr functions to be NULL terminated. Each operation that might overwrite
the terminating '\0' byte, such as string concatenation, will guarantee that
another one is written at the end of the newly concatenated string for example.

This library is heavily modeled after the public interface of the Glib GString
library, although with a clearer naming convention and different considerations
made when designing the library. As a result of not being part of Glib, there
are no obfuscating 'g'-prefixed types here, only standard types one would find 
in standard C to allow for this library to be used easily anywhere outside 
of a Glib project.

## Installation

The installation process is very similar to any of the suckless.org tools. 
First, Edit config.mk to match your local setup.

Second, run the following command:
```sh
make clean install
```

If you would like to change the installation destination or prefix, run:
```sh
make clean install DESTDIR="my/dir/" PREFIX="/my/prefix/"
```
where "my/dir/" is the path to the installation destination (default is "")
and "/my/prefix" is the prefix to install to (default is "/usr/local")

## Usage

#### Basic Example
``` C
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "libistr.h"

int main()
{
	// Creates a new empty istring object
	istring *string = istr_new(NULL);

	// Assigns up until the Null terminating byte
	istr_assign_cstr(string, "Hello, how are you?");

	// The internals of an istring are defined as part of the API, and safe to use.
	printf("str: %s\n", string->buf);

	// Create a new istring from an existing one
	istring *other_string = new_istr(string);

	// Don't forget to clean up!
	istr_free(string, true);
	istr_free(other_string, true);
	
	// The rest of the functions are detailed in libistr.3 and libistr.h
	return 0;
}
```

#### How to link
To compile a project with this library, simply link it with the -l flag like so:
```sh
cc -o project project.c -listr
```

## LICENSE

See LICENSE file included in the repository for copyright and license details
