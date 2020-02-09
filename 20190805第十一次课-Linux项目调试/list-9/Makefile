SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))
target = Sendmsg
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










































	
