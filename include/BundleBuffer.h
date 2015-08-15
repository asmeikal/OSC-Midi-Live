#ifndef __BUNDLE_BUFFER_H
#define __BUNDLE_BUFFER_H

#include <stdio.h>

// init Buffer
int initBundleBuffer(void);
// add To Buffer
int addToBundleBuffer(char *b_contents, size_t length);
// get From Buffer
int getTopOfBundleBuffer(char *res_c, size_t *res_s);

int isEmptyBundleBuffer(void);

#endif
