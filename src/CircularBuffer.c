#include <CircularBuffer.h>

#include <Debug.h>
#include <MemoryManipulation.h>

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

/************************************************************
* Definitions (and macros)
************************************************************/

#define MAX_ELEM_SIZE   256

#define IS_EMPTY(b)     ((b)->first == (b)->last)
#define IS_FULL(b)      ((((b)->last + 1) % (b)->size) == (b)->first)

/************************************************************
* Local structs & variables
************************************************************/

struct circularBufferElement {
    void *content;
    size_t size;
};

struct circularBuffer {
    struct circularBufferElement *head;
    size_t size;
    size_t first;
    size_t last;
};

/************************************************************
* Function definition
************************************************************/

/**
 * 
 * Critical function: allowed to fail.
 */
CircularBuffer createBuffer(const size_t size)
{

    CircularBuffer ret = malloc(sizeof (struct circularBuffer));
    DEBUG_ASSERT_CRITICAL(NULL != ret, "malloc failed\n");

    ret->head = malloc(sizeof (struct circularBufferElement) * size);
    DEBUG_ASSERT_CRITICAL(NULL != ret->head, "malloc failed\n");

    size_t i;
    for(i = 0; i < size; ++i) {
        ret->head[i].content = malloc(MAX_ELEM_SIZE);
        DEBUG_ASSERT_CRITICAL(NULL != ret->head[i].content, "malloc failed\n");
    }

    ret->size  = size;
    ret->first = 0;
    ret->last  = 0;

    return ret;
}

/**
 * Returns 0 on success, sets errno on failure.
 * Non-critical function: not allowed to fail.
 */
int addToBuffer(CircularBuffer b, const void *el, const size_t el_s)
{
    DEBUG_ASSERT(NULL != b,  "NULL pointer argument\n");
    DEBUG_ASSERT(NULL != el, "NULL pointer argument\n");
    if(NULL == b) {
        errno = EINVAL;
        return -1;
    }

    if(NULL == el) {
        errno = EINVAL;
        return -1;
    }

    DEBUG_ASSERT(el_s <= MAX_ELEM_SIZE, "el size too big (max %d)\n", MAX_ELEM_SIZE);
    if(el_s > MAX_ELEM_SIZE) {
        errno = EMSGSIZE;
        return -1;
    }

    int rv;

    if(IS_FULL(b)) {
        /* Free the first element */
        b->first = (b->first + 1) % b->size;
        DEBUG_PRINT("addToBuffer: buffer full, one item lost!\n");
    }

    rv = safe_memcpy(b->head[b->last].content, el, el_s);
    DEBUG_ASSERT(0 == rv, "memcpy failed\n");
    if(0 != rv) {
        return -1;
    }
    b->head[b->last].size = el_s;

    b->last = (b->last + 1) % b->size;

    return 0;
}

/**
 * Returns 0 on success, sets errno on failure.
 * 
 * Before execution, res_s must hold the size of res_c.
 *
 * After execution res_s will hold how many bytes of content
 * have been copied to res_c.
 *
 * Non-critical function: not allowed to fail.
 */
int popTopOfBuffer(CircularBuffer b, void *res_c, size_t *res_s)
{
    errno = 0;
    DEBUG_ASSERT(NULL != b,     "NULL pointer argument\n");
    DEBUG_ASSERT(NULL != res_c, "NULL pointer argument\n");
    DEBUG_ASSERT(NULL != res_s, "NULL pointer argument\n");

    int rv;

    if(NULL == res_s) {
        errno = EINVAL;
        return -1;
    }

    if((NULL == b) || (NULL == res_c)) {
        errno = EINVAL;
        *res_s = 0;
        return -1;
    }

    if(IS_EMPTY(b)) {
        *res_s = 0;
        return -1;
    }

    DEBUG_ASSERT(*res_s >= b->head[b->first].size, \
        "result buffer too small\n");
    if(*res_s < b->head[b->first].size) {
        errno = ENOBUFS;
        *res_s = 0;
        return -1;
    }

    *res_s = b->head[b->first].size;

    rv = safe_memcpy(res_c, b->head[b->first].content, *res_s);
    DEBUG_ASSERT(0 == rv, "memcpy failed\n");
    if(0 != rv) {
        *res_s = 0;
        return -1;
    }

    b->first = (b->first + 1) % b->size;  

    return 0;
}
/**
 * Returns 0 if empty, return 0 and sets errno on failure.
 */
int isEmpty(CircularBuffer b)
{
    errno = 0;
    DEBUG_ASSERT(NULL != b, "NULL pointer argument\n");
    if(NULL == b) {
        errno = EINVAL;
        return 0;
    }
    
    return IS_EMPTY(b);
}
