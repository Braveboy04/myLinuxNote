#通配符
#包含头文件原则
#第二层 变量 
#第三层 隐含规则
#第四层 自动变量
# $^：代表依赖文件
# $@ ：目标文件
# $< : 代表第一个依赖文件
#第五层 使用函数
PROG = test
SRC  = $(wildcard *.c)
OBJ  = $(patsubst %.c,%.o,$(SRC))
CC	:= gcc 
CCFLAG = -g  
all:$(PROG)
$(PROG):$(OBJ)
	$(CC) $(CCFLAG) $^ -o $@ 
.PHONY:
clean:
	rm -rf *.o $(PROG)
