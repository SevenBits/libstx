libistr - Improved String Library
========

### A dynamic string library for C

libistr is a minimal and simple dynamically-sized string handling library that
conforms to the C99 standards. The main goals of the library are proper error
handling, verifying input, thread concurrency, and preventing overflows
wherever possible. All library documentation is in the man page and header
file.

#### USAGE

``` C
#include <libistr.h>
```

#### INSTALLATION

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

#### EXAMPLES

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

To compile a project with this library, simply link it after with the -l flag like so:
```sh
gcc -o project project.c -listr
```

#### LICENSE

See LICENSE file included in the repository for copyright and license details
