# if you type 'make' without arguments, this is the default
PROG        = encrypter
CPROG       = Cencrypter
all:        $(PROG)

# header files and the associated object files
HEAD        = encrypter.h
OBJ         = main.o subs.o encrypt.o decrypt.o errmsg.o rdbuf.o wrbuf.o
COBJ        = Cmain.o subs.o Cerrmsg.o Crdbuf.o Cwrbuf.o encrypt.o decrypt.o

# Testing
TDIR        = ./tests

# special libraries
LIB         = libpa5.a
LIBFLAGS    = -L ./ $(LIB)

# select the compiler and flags yopu can over-ride on command line e.g. make DEBUG= 
CC	        = gcc
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

# specify how to assemble a .S file
%.o : %.S
	$(CC) $(CFLAGS) $(ASFLAGS)  -c -o $@ $<

# specify how to compile/assemble the target
A:      $(PROG)
encrypter:  $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $@


C:      $(CPROG)
Cencrypter: $(COBJ)
	$(CC) $(CFLAGS) $(COBJ) $(LIB) -o $@

# test the program
test:   Atest
Atest:	    $(PROG)
	(cd $(TDIR); make $@)

Ctest:	    $(PROG)
	(cd $(TDIR); make $@)

# get an object dump
dump:       $(PROG)
	$(DUMP) $(DFLAGS) $^

# remove binaries
.PHONY: clean clobber
clean:
	rm -f $(OBJ) $(COBJ) $(PROG)
	(cd $(TDIR); make $@)

# remove binaries and other junk
clobber: clean
	rm -f core
