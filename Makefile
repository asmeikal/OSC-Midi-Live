# OSC-Midi-Live
# Copyright (C) 2015 Michele Laurenti
# email: asmeikal [at] me [dot] com
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You can find a copy of the GNU General Public License at 
# http://www.gnu.org/licenses/gpl.html

LIB_NAME = ArduinoOSC

# directories
BASE_DIR = .

SRC_DIR = $(BASE_DIR)/src
OBJ_DIR = $(BASE_DIR)/obj
BIN_DIR = $(BASE_DIR)/bin
HDR_DIR = $(BASE_DIR)/include

HEADERS = $(HDR_DIR)/*.h
INCLUDE = -I. -I$(HDR_DIR)

# objects
OBJ_LIBS = $(OBJ_DIR)/ArduinoComms.o \
		   $(OBJ_DIR)/Bundle.o \
		   $(OBJ_DIR)/BundleBuffer.o \
		   $(OBJ_DIR)/CircularBuffer.o \
		   $(OBJ_DIR)/CopyrightNotice.o \
		   $(OBJ_DIR)/Debug.o \
		   $(OBJ_DIR)/MemoryManipulation.o \
		   $(OBJ_DIR)/MessageBuffer.o \
		   $(OBJ_DIR)/PatternMatcher.o \
		   $(OBJ_DIR)/Select.o \
		   $(OBJ_DIR)/Sockets.o \
		   $(OBJ_DIR)/StringSplit.o

OBJ_BINS = $(OBJ_DIR)/oscLiveMain.o

# library
LIB_DIR = $(BASE_DIR)/lib
LIB = $(LIB_DIR)/lib$(LIB_NAME).a

# binaries
BINARIES = $(BIN_DIR)/oscLiveMain

# tests

TEST_DIR_SRC = $(SRC_DIR)/test
TEST_DIR_OBJ = $(OBJ_DIR)/test
TEST_DIR_BIN = $(BIN_DIR)/test

TEST_OBJS = $(TEST_DIR_OBJ)/test_osc_stdin.o \
			$(TEST_DIR_OBJ)/test_arduinocomms.o \
			$(TEST_DIR_OBJ)/test_termios.o \
			$(TEST_DIR_OBJ)/test_matchpattern.o
TEST_BINS = $(TEST_DIR_BIN)/test_osc_stdin \
			$(TEST_DIR_BIN)/test_arduinocomms \
			$(TEST_DIR_BIN)/test_termios \
			$(TEST_DIR_BIN)/test_matchpattern

# compiler and flags
STD = --std=c99
UNAME = $(shell uname)
ifeq ($(UNAME), Linux)
STD = --std=gnu99
endif

CC = gcc
CFLAGS_PROD = -DNDEBUG
CFLAGS = $(STD) --pedantic --pedantic-errors -Wall -Wno-unused $(INCLUDE)

# # # # # # # # # #
# main directives #
# # # # # # # # # #

.PHONY: all clean

# object files
$(OBJ_LIBS) $(OBJ_BINS): $(HEADERS)
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/$(@F:.o=.c) -c -o $@ 

$(TEST_OBJS): $(HEADERS)
	mkdir -p $(TEST_DIR_OBJ)
	$(CC) $(CFLAGS) $(TEST_DIR_SRC)/$(@F:.o=.c) -c -o $@

# library
$(LIB): $(OBJ_LIBS)
	mkdir -p $(LIB_DIR)
	ar r $@ $(OBJ_LIBS)
	ranlib $@

# binaries
$(BINARIES): $(LIB) $(OBJ_BINS)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJ_DIR)/$(@F).o -L$(LIB_DIR) -l$(LIB_NAME) -o $@

$(TEST_BINS): $(LIB) $(TEST_OBJS)
	mkdir -p $(TEST_DIR_BIN)
	$(CC) $(CFLAGS) $(TEST_DIR_OBJ)/$(@F).o -L$(LIB_DIR) -l$(LIB_NAME) -o $@


# # # # # # # # # # #
# other  directives #
# # # # # # # # # # #

prod: CFLAGS += $(CFLAGS_PROD)
prod: all

all: $(BINARIES)

tests: $(TEST_BINS)
tests: all

clean: 
	rm -rf $(LIB_DIR)
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)

