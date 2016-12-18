istrlib - Improved String Library
========

### A dynamic string library for C

istrlib is a minimal and simple dynamically-sized string handling library that
conforms to the C99 standards. The main goals of the library are proper error
handling, verifying input, thread concurrency, and preventing overflows
wherever possible. All library documentation is in the man page and header
file.

#### USAGE

\#include <istrlib.h>
typedef struct istring istring;

#### INSTALLATION

The installation process is very similar to any suckless.org tools. 
Edit config.mk to match your local setup

Afterwards run the following command:
```bash
make clean install
```

If you would like to change the installation destination or prefix, run:
```bash
make clean install DESTDIR="my/dir/" PREFIX="/my/prefix/"
```
where "my/dir/" is the path to the installation destination (default is "")
and "/my/prefix" is the prefix to install to (default is "/usr/local")

#### LICENSE

See LICENSE file included in the repository for copyright and license details
