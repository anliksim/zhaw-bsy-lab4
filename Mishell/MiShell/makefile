# ************************************************************
# Fach    BSy
# Autor   M. Thaler
# Version v.fs19
# ************************************************************

CC = gcc
CF = -Wall -g
EX = $(notdir $(shell pwd))

build:	siShell.o readline.o
	$(CC) $(CF) siShell.o readline.o -o $(EX).e

.c.o:
	$(CC) $(CF) -c $<

.cc.o:
	$(CC) $(CF) -c $<

clean:
	rm -f *.o *.e

all:
	@make -s clean
	make 
