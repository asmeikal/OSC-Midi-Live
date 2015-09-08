/**
 * OSC-Midi-Live
 * Copyright (C) 2015 Michele Laurenti
 * email: asmeikal [at] me [dot] com
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You can find a copy of the GNU General Public License at 
 * http://www.gnu.org/licenses/gpl.html
 */

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
