#Makefile for simple programs

#INC=
#LIB= -lpthread

CC = gcc
CC_FLAG = -Wall

PRG = FS
OBJ = other.o init.o dos.o\
      operation.o oper_util.o

$(PRG):$(OBJ)
	$(CC) $(INC) $(LIB) -o $@ $(OBJ)

.SUFFIXES: .c .o

.c .o:
	$(CC) $(CC_FLAG) $(INC) -c $*.c -o $*.o

.PRONY:clean
clean:
	@echo "Removing linked and compiled files..."
	rm -f $(OBJ) $(PRG)
