VERSION = 1.0.0

# System paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/man
LIBPREFIX = ${PREFIX}/lib
INCLUDEPREFIX = ${PREFIX}/include

# Linking flags
LDFLAGS =

# C Compiler settings
CC = cc
CFLAGS = -g -std=c11 -pedantic -O2 -Wall -Wextra
