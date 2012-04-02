# Makefile for ush
#
# Manojkumar Muralidharan
# 

CC=gcc
CFLAGS=-g
SRC=main.c parse.c parse.h
OBJ=main.o parse.o

ush:	$(OBJ)
	$(CC) -o $@ $(OBJ)

tar:
	tar czvf ush.tar.gz $(SRC) Makefile README

clean:
	\rm $(OBJ) ush
