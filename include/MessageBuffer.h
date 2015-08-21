#ifndef __MESSAGE_BUFFER_H
#define __MESSAGE_BUFFER_H

typedef struct messageBuffer* MessageBuffer;

// create message buffer
MessageBuffer createMessageBuffer(unsigned int var_n, char *address);
// add to message buffer
int addToMessageBuffer(MessageBuffer mb, unsigned char *values, unsigned int var_n);
// flush message buffer
int flushMessageBuffer(MessageBuffer mb);

unsigned int getVarNumber(MessageBuffer mb);
char *getDeviceAddress(MessageBuffer mb);

#endif
