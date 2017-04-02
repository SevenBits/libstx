VERSION = 1.0.0

# System paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/man
LIBPREFIX = ${PREFIX}/lib
INCLUDEPREFIX = ${PREFIX}/include

# Linking flags
LDFLAGS = -s

# C Compiler settings
CC = cc
CFLAGS = -std=c11 -pedantic -O2 -Wall -Wextra

# C++ compiler settings
CPP = g++
CPFLAGS = -std=c++98 -pedantic -O2 -Wall -Wextra
