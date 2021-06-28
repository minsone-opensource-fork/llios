#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdio.h>

void read_bytes(FILE *fptr, int offset, void *buf, int size);

void *load_bytes(FILE *fptr, int offset, int size);

// Read a uleb128 number int to `out` and return the number of bytes processed.
// This method assumes the input correctness and doesn't handle error cases.
int read_uleb128(const uint8_t *p, uint64_t *out);

// If the string contains '\n', replace with literal "\n".
void format_string(char *str, char *formatted);

#endif /* UTIL_H */