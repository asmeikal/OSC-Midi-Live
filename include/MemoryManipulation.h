#ifndef __MEMORY_MANIPULATION_H
#define __MEMORY_MANIPULATION_H

#include <stdio.h>

int safe_memcpy(void *restrict dst, const void *restrict src, size_t len);

#endif
