## -*- Makefile -*-
##
## Author: kishore
## Time: Jun 7, 2015 4:21:06 PM
##

CC = gcc

TESTS_O = $(foreach lib, $(wildcard test_*.c), $(lib:%.c=%.o))
TESTS_C = $(wildcard test_*.c)


LIBRARIES_C = $(wildcard ../libraries/lib_*.c)
LIBRARIES_O = $(foreach lib, $(LIBRARIES_C), $(lib:%.c=%.o))
test_%.o : test_%.c test_%.h
	gcc -c -g $<

../libraries/%.o : ../libraries/%.c
	gcc -c $^

test : tests.c tests.a library.a
	gcc -o test seatest/seatest.c $^
	-rm *.a

tests.a : $(TESTS_O)
	ar r $@ *.o
	-rm *.o
library.a : $(LIBRARIES_O)
	ar r $@ *.o
	-rm *.o
