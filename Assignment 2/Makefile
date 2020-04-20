#
# CMSC257 - Utility Library 
# Code for the CMSC257 Project 2
#

all: cmsc-assign2-main

cmsc-assign2-main: cmsc-assign2-main.o cmsc-assign2-support.o
	gcc -Wall $^ -o $@

cmsc-assign2-main.o: cmsc-assign2-main.c cmsc-assign2-support.h
	gcc -Wall -c $<

cmsc-assign2-support.o: cmsc-assign2-support.c cmsc-assign2-support.h
	gcc -Wall -c $<

clean:
	rm -f cmsc-assign2-main cmsc-assign2-main.o cmsc-assign2-support.o
