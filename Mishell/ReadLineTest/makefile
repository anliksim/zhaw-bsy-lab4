# ************************************************************
# Fach    BSy
# Autor   M. Thaler
# Version v.fs17
# ************************************************************

CC = gcc
CF = -Wall -g
EX = rlTest

build:	rlTest.o readline.o
	$(CC) $(CF) rlTest.o readline.o -o $(EX).e

.c.o:
	$(CC) $(CF) -c $<

.cc.o:
	$(CC) $(CF) -c $<

clean:
	rm -f *.o *.e

all:
	@make -s clean
	make 
