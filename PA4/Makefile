# if you type 'make' without arguments, this is the default
PROG        = cw ccw
all:        $(PROG)

# other source files and the associated object files (this can be blank) SRC is unused here
HEAD        = cw.h
OBJ         = rd.o setup.o result.o
OBJ1        = cw.o
OBJ2        = ccw.o

# Testing
TDIR        = ./tests
# special libraries
LIBi        =
LIBFLAGS    =

# select the compiler and flags yopu can over-ride on command line e.g. make DEBUG= 
CC	= gcc
DEBUG       = -ggdb
CSTD        =
WARN        = -Wall -Wextra -Werror
CDEFS       =
CFLAGS      = -I. $(DEBUG) $(WARN) $(CSTD) $(CDEFS)

# some flags to use with assembler tools
ASLIST	    = -Wa,-adhln
ASFLAGS     =

# how to get an object dump of the target
DUMP        = objdump
DFLAGS      = -d

$(OBJ):     $(HEAD)
$(OBJ1):    $(HEAD)
$(OBJ2):    $(HEAD)

%.o : %.S
	$(CC) $(CFLAGS) $(ASFLAGS)  -c -o $@ $<

# specify how to compile the target
cw:     $(OBJ) $(OBJ1)
	$(CC) $(CFLAGS) $(OBJ) $(OBJ1) $(LIB) -o $@

ccw:    $(OBJ) $(OBJ2)
	$(CC) $(CFLAGS) $(OBJ) $(OBJ2) $(LIB) -o $@

# test the program
test:	    $(PROG)
	(cd $(TDIR); make $@)

cwtest:	   cw
	(cd $(TDIR); make $@)
 
ccwtest:   ccw
	(cd $(TDIR); make $@)
 
# get an object dump
dump:       $(PROG)
	$(DUMP) $(DFLAGS) $^

# remove binaries
.PHONY: clean clobber
clean:
	rm -f $(OBJ) $(OBJ1) $(OBJ2) $(PROG) $(OUT)
	(cd $(TDIR); make $@)

# remove binaries and other junk
clobber: clean
	rm -f core
