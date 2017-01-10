# tshell version
VERSION=0.1

# Paths
PREFIX=/usr/local
MANPREFIX=${PREFIX}/share/man

# Compilation flags
CFLAGS=-g -std=c99 -pedantic -Wall -Wextra -static -O1

# Extra flags below
#-Wfloat-equal -Wundef -Wstrict-prototypes -Wpointer-arith -Wstrict-overflow=5 -Waggregate-return -Wswitch-default -Wconversion -Wswitch-enum -Wunreachable-code

# Linking flags
LDFLAGS=-g

# Compiler and linker
CC=cc