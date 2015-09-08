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

#ifndef __BUNDLE_BUFFER_H
#define __BUNDLE_BUFFER_H

#include <stdio.h>

// init Buffer
int initBundleBuffer(void);
// add To Buffer
int addToBundleBuffer(const char *b_contents, const size_t length);
// get From Buffer
int getTopOfBundleBuffer(char *res_c, size_t *res_s);

int isEmptyBundleBuffer(void);

#endif
