SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))
target = hello
DEFS   = 
CFLAGS = -g
CC     := gcc
INCLUDE =
LIB = 
$(target):$(OBJ)
	$(CC) $(CFLAGS) $(DEFS)  $^ -o $(target) $(LIB)
.PHONY:
clean:
	rm -rf *.o $(target)










































	
