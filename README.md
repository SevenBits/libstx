libistr - Improved String Library
========

### A dynamic string library for C

The istring library (hereafter referred to as libistr) is an efficient and 
simple dynamic string library for C modeled after the SDS string library
(Although libistr is a separate codebase). The reason for the reimplementation 
of the concept is to allow for a cleaner codebase with an emphasis on unicode
character handling. Most design changes have been made to avoid ambiguity, 
like typedefing the istring type to char instead of char \*, 
which makes the library easier to use and less error prone.

This idea for modeling strings, rather than defining a structure to contain a 
string's metadata, allocates room for a header prefixed to the string to store 
metadata about the string. The SDS string libraryThis has a number of advantages detailed below.

### Gotcha's
1. ALWAYS assign the return value of an 'istr\_' prefixed function that returns
an istring * because the function may have to call a reallocation method on the istring
which will change where your istring should reference.

##### DO:
``` C
istring string = istr_new(NULL);
// string still points towards your string, even if a realloc was called.
string = istr_append_cstr(string, "hello");
printf("%s\n", string);
```
##### DONT:
``` C
istring string = istr_new(NULL);
// string may not point to your string anymore at this point.
istr_append_cstr(string, "hello");
printf("%s\n", string);
```

2. Because of the above, keeping references to istring is hard and should
be done with care, usually with a reference counter of some sort.

### features
The advantages here are nearly the same as the SDS string library.

1. The istring model is ease of interoperability with read-only C string
functions, as the istring can simply be passed as a parameter as if it
were a normal char \*.

2. Memory allocation in libistr is very efficient, following a rule to 
keep an istring's size as a power of 2 that can at least hold the amount
of contents specified. The keeps reallocations to a minimum.

3. Cache locality. Since all the metadata of a string is always prefixed to the
char \*,memory allocated to an istring is always contiguous in memory.
This keeps memory fragmentation low and speed high when using istrings.

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
#include <libistr.h>

int main()
{
	// Creates a new empty istring object
	istring *string = istr_new(NULL);

	// Assigns up until a '\0' is reached, then appends a '\0' to the end
	string = istr_assign_cstr(string, "Hello, how are you?");

	// Istrings are safe to use with read-only functions that accept char *.
	printf("str: %s\n", string);

	// Create a new istring from an existing one
	istring *other_string = istr_new(string);

	// Don't forget to clean up! Call this instead of free to make sure metadata is free'd too
	istr_free(string);

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
