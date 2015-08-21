#include <Select.h>

#include <Debug.h>
#include <MemoryManipulation.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>

/************************************************************
* Local variables and structures
************************************************************/

struct fd_info registered_fds_read[MAX_FDS]  = {{0}};
struct fd_info registered_fds_write[MAX_FDS] = {{0}};

int     max_fd;
fd_set  fds_read;
fd_set  fds_write;

/************************************************************
* Local functions declaration
************************************************************/

static int registerFD(struct fd_info registry[], const int fd, const CallBack function, const unsigned int buffsize, void *extra_info);
static int unregisterFD(struct fd_info registry[], const int fd);
static void buildFDSet(struct fd_info registry[], fd_set *fds);

/************************************************************
* Register/unregister functions
************************************************************/

int registerReadFD(const int fd, const CallBack function, const unsigned int buffsize, void *extra_info)
{
    return registerFD(registered_fds_read, fd, function, buffsize, extra_info);
}

int  unregisterReadFD(const int fd)
{
    return unregisterFD(registered_fds_read, fd);
}

int registerWriteFD(const int fd, const CallBack function, void *extra_info)
{
    return registerFD(registered_fds_write, fd, function, 0, extra_info);
}

int  unregisterWriteFD(const int fd)
{
    return unregisterFD(registered_fds_write, fd);
}

/**
 * Registers file descriptor 'fd', allocates a buffer of size 'buffsize',
 * and assigns 'function' as the file descriptor's callback function.
 *
 * Returns 0 on success, non-zero on failure.
 */
static int registerFD(struct fd_info registry[], const int fd, const CallBack function, const unsigned int buffsize, void *extra_info)
{
    int i, fdNotInserted = 1;

    DEBUG_ASSERT(0 <= fd, "Invalid file descriptor\n");
    if(0 > fd) {
        return fdNotInserted;
    }

    DEBUG_ASSERT(NULL != function, "No valid CallBack function given.\n");
    if(NULL == function) {
        return fdNotInserted;
    }

    for(i = 0; (i < MAX_FDS) && (1 == fdNotInserted); ++i) {
        if(0 == registry[i].enabled) {
            registry[i].fd          = fd;
            registry[i].function    = function;
            registry[i].extra_info  = extra_info;
            registry[i].buff_size   = buffsize;
            registry[i].buff_full   = 0;

            if(0 < buffsize) {
                registry[i].buffer  = (char *) malloc(buffsize);
                if(NULL == registry[i].buffer) {
                    /* If we can't allocate the buffer, I just try with the next empty slot. */
                    DEBUG_PRINT("Attempt to allocate buffer failed.\n");
                    continue;
                }
            }
            else {
                /* No buffer is needed. */
                registry[i].buffer = NULL;
            }

            registry[i].enabled = 1;
            fdNotInserted = 0;
        }
    }

    if(0 != fdNotInserted) {
        DEBUG_PRINT("Unable to register fd.\n");
    }

    return fdNotInserted;
}

/**
 * Removes 'fd' from the file descriptors list.
 */
static int unregisterFD(struct fd_info registry[], const int fd)
{
    int i, fdNotFound = 1;

    DEBUG_ASSERT(0 <= fd, "Invalid file descriptor.");
    if(0 > fd) {
        return fdNotFound;
    }

    for(i = 0; (i < MAX_FDS) && (1 == fdNotFound); ++i) {

        if((0 != registry[i].enabled) && 
           (registry[i].fd == fd)) 
        {
            registry[i].enabled = 0;

            if(NULL != registry[i].buffer) {
                free(registry[i].buffer);
            }

            fdNotFound = 0;
        }
    }
    return fdNotFound;
}

/************************************************************
* Select functions
************************************************************/

void buildReadFDSet(void)
{
    buildFDSet(registered_fds_read, &fds_read);
}

void buildWriteFDSet(void)
{
    buildFDSet(registered_fds_write, &fds_write);
}


/**
 * Prepares the call to selectFDSet.
 */
static void buildFDSet(struct fd_info registry[], fd_set *fds)
{
    int i;
    FD_ZERO(fds);
    for(i = 0; i < MAX_FDS; ++i) {
        if(0 < registry[i].enabled) {
            FD_SET(registry[i].fd, fds);
            if(registry[i].fd > max_fd) {
                max_fd = registry[i].fd;
            }
        }
    }
}

/**
 * PRE: buildFDSet() has been called.
 *
 * Returns 0 on timeout, >0 on success, <0 on error.
 */
int selectFDSet(void)
{
    struct timeval timeout = {MAX_SEC_WAIT, MAX_USEC_WAIT};
    int rv;
    rv = select(max_fd+1, &fds_read, &fds_write, NULL, &timeout);
    max_fd = -1;

    return rv;
}

/**
 * PRE: selectFDSet() returned with success.
 */
void fireCallBacks(void)
{
    int i;
    for(i = 0; i < MAX_FDS; ++i) {
        if((0 < registered_fds_read[i].enabled) && 
           (0 < FD_ISSET(registered_fds_read[i].fd, &fds_read))) 
        {
            registered_fds_read[i].function(registered_fds_read+i);
        }
    }

    for(i = 0; i < MAX_FDS; ++i) {
        if((0 < registered_fds_write[i].enabled) && 
           (0 < FD_ISSET(registered_fds_write[i].fd, &fds_write))) 
        {
            registered_fds_write[i].function(registered_fds_write+i);
        }
    }
}


