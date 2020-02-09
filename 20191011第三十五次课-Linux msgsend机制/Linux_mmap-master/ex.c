#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
 
int main(void)
{
    int *p;
    int fd = open("hello", O_RDWR);
    if (fd < 0) {
        perror("open hello");
        exit(1);
    }
    p = mmap(NULL, 6, PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    close(fd);
    p[0] = 0x30313233;  //little endian, 低位数据0x33先写入文件的低位地址0x0
    munmap(p, 6);
    return 0;
}
/*
make a file named "hello" and contains string "hello" only
$ ./a.out
$ od -tx1 -tc hello 
0000000  33  32  31  30  6f  0a
          3   2   1   0   o  \n
0000006
 
 */
