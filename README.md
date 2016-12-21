libistr - Improved String Library
========

### A dynamic string library for C

libistr is a minimal and simple dynamically-sized string handling library that
conforms to the C99 standards. The main goals of the library are proper error
handling, verifying input, thread concurrency, and preventing overflows
wherever possible. All library documentation is in the man page and header
file.

## Installation

The installation process is very similar to any suckless.org tools. 
Edit config.mk to match your local setup

Afterwards run the following command:
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

	// Access to the internals of an istring are done through functions
	printf("str: %s\n", istr_str(string));

	// Create a new istring from an existing one
	istring *other_string = new_istr(string);

	// Don't forget to clean up!
	istr_free(istring, true)
	
	// The rest of the functions are detailed in libistr.3 and libistr.h
	return 0;
}
```

#### Keep references alive
This is useful if you need to keep references to a particular istring's
character buffer rather than references to the istring itself. Avoid using it
unless you really need to because it can be a little confusing with all the
dereferencing, but it is guaranteed to work.
``` C
	// With this assignment the string buffer is 8 bytes long
	istring *string = istr_new_cstr("string");

	// Grab a reference to the istring's buffer
	char **ptr = istr_strptr(string);

	// Assign the string (which causes a realloc to 32 bytes)
	string = istr_assign_cstr("This string will realloc");

	// And the reference is guaranteed to work
	printf("%s\n", *ptr);
```

#### How to link
To compile a project with this library, simply link it with the -l flag like so:
```sh
cc -o project project.c -listr
```

## LICENSE

See LICENSE file included in the repository for copyright and license details
