libistr - Improved String Library
========

### A dynamic string library for C

libistr is a minimal and simple dynamically-sized string handling library that
conforms to the C99 standards. The main goals of the library are proper error
handling, verifying input, thread concurrency, and preventing integer and 
buffer overflows. All library documentation is in the man page and header
file. All strings are guarenteed to be NULL terminated at all times 
as well with this library, making it safe to use C string functions on them.

## Installation

The installation process is very similar to any suckless.org tools. 
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
	istr_free(istring, true)
	
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
