#include <BundleBuffer.h>

#include <Bundle.h>
#include <Debug.h>
#include <CircularBuffer.h>

#include <assert.h>

/************************************************************
* Defines
************************************************************/

#define BUFFER_SIZE     16

/************************************************************
* Local structs & variables
************************************************************/

// circular bundle buffer
CircularBuffer bundle_buffer = NULL;

/************************************************************
* Functions definition
************************************************************/

/**
 * Critical function: allowed to fail.
 */
int initBundleBuffer(void)
{
    DEBUG_ASSERT(NULL == bundle_buffer, "bundle already exists");
    if(NULL != bundle_buffer) {
        return 0;
    }

    bundle_buffer = createBuffer(BUFFER_SIZE);

    return 0;
}
// add To Buffer

int addToBundleBuffer(const char *b_contents, const size_t length)
{
    return addToBuffer(bundle_buffer, (void *) b_contents, length);
}

// get From Buffer
int getTopOfBundleBuffer(char *res_c, size_t *res_s)
{
    return popTopOfBuffer(bundle_buffer, res_c, res_s);
}

int isEmptyBundleBuffer(void)
{
    return isEmpty(bundle_buffer);
}
