#include <Bundle.h>

// libs

#include <Debug.h>
#include <BundleBuffer.h>
#include <MemoryManipulation.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>      /* For htonl */

/************************************************************
* Constants, magic numbers and macros
************************************************************/

#define BUNDLE_MAX_SIZE     256

#define BSIZELEN        4
#define BHEADER         "#bundle\0"
#define BHEADERLEN      8
#define BTIMESTAMP      "\0\0\0\0\0\0\0\1"
#define BTIMESTAMPLEN   8

/* Time to leave ;) */
#define BUNDLE_TTL      1000L

#define MAX_MSG_SIZE    (BUNDLE_MAX_SIZE - (BSIZELEN + BHEADERLEN + BTIMESTAMPLEN))

#define NOT_INITIALIZED(b)  (NULL == (b).content)

/************************************************************
* Local functions specification
************************************************************/

static int startBundle(void);
static int sendBundle(void);

/************************************************************
* Structs and local variables
************************************************************/

struct Bundle local_bundle = {0};

/************************************************************
* Function definition
************************************************************/

/**
 * Critical function: allowed to fail.
 */
static int startBundle(void)
{

    DEBUG_ASSERT(NULL == local_bundle.content, "bundle already exists");
    if(NULL != local_bundle.content) {
        /* Bundle already initializated */
        return 0;
    }

    local_bundle.content = malloc(BUNDLE_MAX_SIZE);
    DEBUG_ASSERT(NULL != local_bundle.content, "bundle malloc failed");
    if(NULL == local_bundle.content) {
        /* malloc failed */
        errno = ENOMEM;
        return -1;
    }

    local_bundle.size   = BUNDLE_MAX_SIZE;
    local_bundle.length = 0;
    local_bundle.birth.tv_sec  = 0;
    local_bundle.birth.tv_usec = 0;

    gettimeofday(&(local_bundle.birth), NULL);

    return 0;
}

/**
 * Pre: the first 4 bytes of msg represent htonl(msg_s - 4).
 */
int addToBundle(const char *msg, const size_t msg_s)
{
    int rv;

    if(NOT_INITIALIZED(local_bundle)) {
        (void) startBundle();
    }
    /* Holds: the bundle exists */

    DEBUG_ASSERT(NULL != msg, "NULL pointer argument\n");
    if((NULL == msg)) {
        errno = EINVAL;
        return -1;
    }

    DEBUG_ASSERT(msg_s <= MAX_MSG_SIZE, "msg size too big (max %d)\n", MAX_MSG_SIZE);
    if(msg_s > MAX_MSG_SIZE) {
        errno = EMSGSIZE;
        return -1;
    }
    /* Holds: message fits in a bundle */

    if(msg_s > (local_bundle.size - local_bundle.length)) {
        (void) sendBundle();
    }
    /* Holds: there is enough space in the bundle for the message, or the bundle is empty */

    if(0 == local_bundle.length) {
        gettimeofday(&local_bundle.birth, NULL);
        local_bundle.length += BSIZELEN;
        rv = safe_memcpy(local_bundle.content + local_bundle.length, BHEADER, BHEADERLEN);
        DEBUG_ASSERT(0 == rv, "memcpy failed\n");
        if(0 != rv) {
            local_bundle.length = 0;
            return -1;
        }

        local_bundle.length += BHEADERLEN;
        rv = safe_memcpy(local_bundle.content + local_bundle.length, BTIMESTAMP, BTIMESTAMPLEN);
        DEBUG_ASSERT(0 == rv, "memcpy failed\n");
        if(0 != rv) {
            local_bundle.length = 0;
            return -1;
        }

        local_bundle.length += BTIMESTAMPLEN;
    }
    /* Holds: the bundle is not empty, and there is enough space for the message */

    rv = safe_memcpy(local_bundle.content + local_bundle.length, msg, msg_s);
    DEBUG_ASSERT(0 == rv, "memcpy failed\n");
    if(0 != rv) {
        return -1;
    }
    local_bundle.length += msg_s;

    pokeBundle();

    return 0;
}

void pokeBundle(void)
{
    if(NOT_INITIALIZED(local_bundle)) {
        (void) startBundle();
    }

    if(0 == local_bundle.length) {
        return;
    }

    struct timeval c_t = {0,0};
    gettimeofday(&c_t, NULL);
    unsigned long diff = (c_t.tv_sec - local_bundle.birth.tv_sec) * 1000 * 1000 - c_t.tv_usec - local_bundle.birth.tv_usec;

    if(diff > BUNDLE_TTL) {
        (void) sendBundle();
    }

}

// send Bundle (adds bundle to bundle buffer)
static int sendBundle(void)
{
    if(0 == local_bundle.length) {
        return 0;
    }
    int rv;

    uint32_t msg_size = htonl(local_bundle.length - 4);
    rv = safe_memcpy(local_bundle.content, &msg_size, BSIZELEN);

    DEBUG_ASSERT(0 == rv, "memcpy failed\n");
    if(0 != rv) {
        return -1;
    }

    (void) addToBundleBuffer(local_bundle.content, local_bundle.length);

    local_bundle.length = 0;
    gettimeofday(&local_bundle.birth, NULL);

    return 0;
}
