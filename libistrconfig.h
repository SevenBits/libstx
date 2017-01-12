/* See LICENSE file for copyright and license details */

// Allocation methods
#define ISTR_MALLOC malloc
#define ISTR_REALLOC realloc
#define ISTR_FREE free

// The size of the header type, the type that stores length and size.
#define ISTR_HEADER_TYPE size_t
