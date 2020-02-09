#.c .h--->.i 预编译
#hello.i:hello.c
#	gcc -E hello.c -o hello.i
#.i--->.s 汇编	
#hello.s:hello.i
#	gcc -S hello.i -o hello.s
#hello.o:hello.s
#	gcc -c hello.s -o hello.o
	
hello:hello.o
	gcc  hello.o -o hello
clean:
	rm -rf hello hello.i hello.s hello.o