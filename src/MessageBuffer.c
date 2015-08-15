#include <MessageBuffer.h>

#include <Debug.h>
#include <Bundle.h>
#include <MemoryManipulation.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <arpa/inet.h>      /* For htonl */

/************************************************************
* Defines and macros
************************************************************/

#define VAR_MAX_VALUE       255.0f

#define MSG_BUFF_CAPACITY   256
/* That's how many n-uples the buffer can hold. Must be BIG */
#define DER_THRESHOLD       8
/* Min difference between two values */

#define ABS(x)              ((x) < 0) ? -(x) : (x)
#define ROUND_TO_FOUR(x)    (x) + (4 - ((x) % 4))

/************************************************************
* Local structures
************************************************************/

struct messageBuffer {
    char *address;
    unsigned int var_n;
    unsigned char *buff;
    unsigned int n_vals;
    unsigned char *last_sent;
};

/************************************************************
* Local functions signatures
************************************************************/

static int sendMessageToBundle(MessageBuffer mb, unsigned char *values);

/************************************************************
* Function definition
************************************************************/

/**
 * A MessageBuffer memorizes up to MSG_BUFF_CAPACITY (var_n)-uples of 
 * unsigned chars, without giving them any structure.
 * Critical function: allowed to fail.
 */
MessageBuffer createMessageBuffer(unsigned int var_n, char *address)
{
    int rv;

    assert(NULL != address);
    MessageBuffer ret = malloc(sizeof (struct messageBuffer));
    assert(NULL != ret);

    ret->var_n  = var_n;
    ret->n_vals = 0;

    ret->address = malloc(strlen(address) + 1);
    assert(NULL != ret->address);
    rv = safe_memcpy(ret->address, address, strlen(address) + 1);
    assert(0 == rv);

    ret->buff = malloc(var_n * MSG_BUFF_CAPACITY);
    assert(NULL != ret->buff);

    ret->last_sent = malloc(var_n);
    assert(NULL != ret->last_sent);

    return ret;
}

/**
 * Pre: 
 */
int addToMessageBuffer(MessageBuffer mb, unsigned char *values, unsigned int var_n)
{
    int rv;

    DEBUG_ASSERT(NULL != values, "NULL pointer as argument.\n");
    DEBUG_ASSERT(NULL != mb, "NULL pointer as argument.\n");
    DEBUG_ASSERT(var_n == mb->var_n, "invalid var number.\n");
    DEBUG_ASSERT(mb->n_vals < MSG_BUFF_CAPACITY, "message buffer full.\n");

    if((NULL == values) || (NULL == mb) || (var_n != mb->var_n)) {
        errno = EINVAL;
        return -1;
    }

    if(mb->n_vals >= MSG_BUFF_CAPACITY) {
        errno = ENOBUFS;
        return -1;
    }

    rv = safe_memcpy(mb->buff+(mb->n_vals * mb->var_n), values, var_n);

    DEBUG_ASSERT(0 == rv, "memcpy failed.\b");
    if(0 != rv) {
        errno = EINVAL;
        return -1;
    }

    mb->n_vals += 1;

    return 0;
}

int flushMessageBuffer(MessageBuffer mb)
{
    DEBUG_ASSERT(NULL != mb, "NULL pointer as argument.\n");
    if((NULL == mb)) {
        errno = EINVAL;
        return -1;
    }

    if(0 == mb->n_vals) {
        return 0;
    }

    int rv;
    unsigned int i, j;
    unsigned char *tmp, *changed, changed_any;

    tmp = malloc(mb->var_n);
    DEBUG_ASSERT(NULL != tmp, "malloc failed.\n");
    if((NULL == tmp)) {
        errno = ENOMEM;
        return -1;
    }

    changed = malloc(mb->var_n);
    DEBUG_ASSERT(NULL != changed, "malloc failed.\n");
    if((NULL == changed)) {
        errno = ENOMEM;
        return -1;
    }

    rv = safe_memcpy(tmp, mb->last_sent, mb->var_n);
    DEBUG_ASSERT(0 == rv, "memcpy failed.\b");
    if(0 != rv) {
        errno = EINVAL;
        return -1;
    }

    memset(changed, 0, mb->var_n);

    for(i = 0; i < mb->n_vals; ++i) {

        for(j = 0; j < mb->var_n; ++j) {

            if((0 == changed[j]) && 
               (ABS((mb->buff)[(i*mb->var_n)+j] - tmp[j]) > DER_THRESHOLD)) 
            {
                /* Take this value */

                tmp[j] = (mb->buff)[(i*mb->var_n)+j];
                changed[j] = 1;

            }
        }

        if(i + 1 < mb->var_n) {
            /* It's not the last item in buffer */

            for(j = 0; j < mb->var_n; ++j) {
                /* Then we check if we should send straight away */

                if((0 < changed[j]) && 
                   (ABS((mb->buff)[((i+1)*mb->var_n)+j] - tmp[j]) > DER_THRESHOLD)) 
                {
                    /* Send now */

                    (void) sendMessageToBundle(mb, tmp);

                    memset(changed, 0, mb->var_n);

                    break;
                }
            }
        }

    }

    changed_any = 0;
    for(j = 0; (j < mb->var_n) && (0 == changed_any); ++j) {
        changed_any = changed_any || changed[j];
    }

    if(0 < changed_any) {
        (void) sendMessageToBundle(mb, tmp);

        // memset(changed, 0, mb->var_n);
    }

    free(tmp);
    free(changed);

    mb->n_vals = 0;

    return 0;

}

unsigned int getVarNumber(MessageBuffer mb)
{
    DEBUG_ASSERT(NULL != mb, "NULL pointer as argument.\n");
    if((NULL == mb)) {
        errno = EINVAL;
        return 0;
    }

    return mb->var_n;

}

/************************************************************
* Local functions definition
************************************************************/

static int sendMessageToBundle(MessageBuffer mb, unsigned char *values)
{
    int rv;

    unsigned int var_n = mb->var_n, 
                 addr_len = strlen(mb->address);

    size_t tot_msg_len = 4, 
           msg_len = 0, 
           i = 0;

    unsigned char j;

    msg_len += ROUND_TO_FOUR(1 + addr_len); /* bytes for the address */
    msg_len += ROUND_TO_FOUR(1 + var_n);    /* bytes for the type specs */
    msg_len += var_n * sizeof(float);        /* bytes for the actual data */

    tot_msg_len += msg_len;     /* 4 bytes for the total size */

    char *message = malloc(tot_msg_len);
    DEBUG_ASSERT(NULL != message, "malloc failed.\n");
    if(NULL == message) {
        errno = ENOMEM;
        return -1;
    }

    memset(message, 0, tot_msg_len);

    /* add the msg size */
    uint32_t msg_s = htonl(msg_len);
    rv = safe_memcpy(message, &msg_s, 4);
    i += 4;

    /* add the address */
    message[i] = '/';
    ++i;
    rv = safe_memcpy(message+i, mb->address, addr_len);
    i += ROUND_TO_FOUR(1 + addr_len) - 1;

    DEBUG_ASSERT(0 == rv, "copy failed.\n");
    if(0 != rv) {
        free(message);
        return -1;
    }

    /* add the type specifiers */
    message[i] = ',';
    ++i;
    memset(message+i, 'f', var_n);
    i += ROUND_TO_FOUR(1 + var_n) - 1;

    DEBUG_ASSERT(0 == rv, "copy failed.\n");
    if(0 != rv) {
        free(message);
        return -1;
    }

    /* add the actual data */
    float val;
    uint32_t val_i;
    for(j = 0; j < var_n; ++j) {
        val = ((float) values[j]) / VAR_MAX_VALUE;
        val_i = htonl(*((uint32_t *) &val));

        rv = safe_memcpy(message+i, &val_i, sizeof(float));

        DEBUG_ASSERT(0 == rv, "copy failed.\n");
        if(0 != rv) {
            free(message);
            return -1;
        }

        i += sizeof(float);
    }

    /* ready to send */
    (void) addToBundle(message, tot_msg_len);

    rv = safe_memcpy(mb->last_sent, values, mb->var_n);

    DEBUG_ASSERT(0 == rv, "copy failed.\n");
    if(0 != rv) {
        free(message);
        return -1;
    }

    free(message);

    return 0;

}
