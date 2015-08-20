#ifndef __SOCKETS_H
#define __SOCKETS_H

#include <Select.h>

int createSocket(const char *address, const unsigned short port);
void sendAllBufferedBundles(struct fd_info *fdi);

#endif
