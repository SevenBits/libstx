VERSION=0.0.0

# System paths
PREFIX=/usr/local
MANPREFIX=${PREFIX}/man
LIBPREFIX=${PREFIX}/lib
INCLUDEPREFIX=${PREFIX}/include

# Linking flags
LDFLAGS=

# Compiler settings
CC=cc
CFLAGS=-std=c99 -pedantic -O2 -Wall -Wextra -static
