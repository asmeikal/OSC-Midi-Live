
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
		   $(OBJ_DIR)/MessageBuffer.o \
		   $(OBJ_DIR)/Select.o \
		   $(OBJ_DIR)/Sockets.o \
		   $(OBJ_DIR)/Debug.o \
		   $(OBJ_DIR)/MemoryManipulation.o

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

# TEST_OBJS = $(TEST_DIR_OBJ)/...
# TEST_BINS = $(TEST_DIR_BIN)/...

# compiler and flags
CC = gcc
# CFLAGS_PROD = -DNDEBUG
CFLAGS = --std=c99 --pedantic --pedantic-errors -Wall -Wno-unused $(INCLUDE) $(CFLAGS_PROD)

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

all: $(BINARIES)

tests: $(TEST_BINS)

clean: 
	rm -rf $(LIB_DIR)
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)

