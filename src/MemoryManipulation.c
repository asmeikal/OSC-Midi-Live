#include <MemoryManipulation.h>

#include <stdlib.h>
#include <string.h>

int safe_memcpy(void *restrict dst, const void *restrict src, size_t len)
{
    int rv;

    memcpy(dst, src, len);
    rv = memcmp(dst, src, len);

    return rv;
}
