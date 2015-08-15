#ifndef __SELECT_H
#define __SELECT_H

#define MAX_FDS         20

#define MAX_SEC_WAIT    0
#define MAX_USEC_WAIT   100

struct fd_info;

typedef void (*CallBack)(struct fd_info *fd);

struct fd_info {
    int fd;
    int enabled;
    CallBack function;
    char *buffer;
    unsigned int buff_size;
    unsigned int buff_full;
    void *extra_info;
};

int registerReadFD(int fd, CallBack function, unsigned int buffsize, void *extra_info);
int  unregisterReadFD(int fd);
int registerWriteFD(int fd, CallBack function, void *extra_info);
int  unregisterWriteFD(int fd);

void buildReadFDSet(void);
void buildWriteFDSet(void);
int  selectFDSet(void);

void fireCallBacks(void);

#endif
