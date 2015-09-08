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
    char *name;
    char *buffer;
    unsigned int buff_size;
    unsigned int buff_full;
    void *extra_info;
};

int registerReadFD(const int fd, const char *name, const CallBack function, const unsigned int buffsize, void *extra_info);
int  unregisterReadFD(const int fd);
int registerWriteFD(const int fd, const char *name, const CallBack function, void *extra_info);
int  unregisterWriteFD(const int fd);

void buildReadFDSet(void);
void buildWriteFDSet(void);
int  selectFDSet(void);

void fireCallBacks(void);

#endif
