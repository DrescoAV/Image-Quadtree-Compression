# Copyright Mihai Bianca Ioana

CC=gcc
CFLAGS=-g -Wall -Wextra -std=c99

SRCS=$(wildcard *.c)
OBJS=$(SRCS:%.c=%.o)
TARGETS=$(OBJS:%.o=%)


build: $(OBJS)
	$(CC) $(CFLAGS) -o quadtree $(OBJS)
	
run_vma:build
	./quadtree

pack:
	zip -FSr 312CC_Mihai_Bianca_Ioana_SD.zip Makefile *.c *.h

clean:
	rm -f $(TARGETS) $(OBJS)

.PHONY: pack clean	
