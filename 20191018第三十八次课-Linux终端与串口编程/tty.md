# Linux中的终端编程

1.  终端是一个字符型设备驱动

   1. 类型 
   2. 属性控制
   3. 读写操作 
   
2. 终端类型

   ```shell
   * 操作与测试命令 
   cat /proc/tty/drivers
   /dev/ttyS2
   cat /sys/class/tty/ttyS2/dev  查看设备号
   cat /sys/class/tty/ttyS2/device/resources 查看设备资源
   ```

   ```
   1. 标准控制台(tty)
   /dev/tty1 --- tty6
   2. 虚拟控制台(pty ptmx)
   /dev/ptmx 
   3. $$ 指定当前终端 
   
   ```

   ```c
   //虚拟终端编程 
   int ret , pid, pty, tty, n;
   char *slave, *temp, *shell;
   fd_set rd;
   struct winsize ws;
   if( openpty( &pty, &tty, NULL, NULL, NULL ) < 0 )
   {
       return( 24 );
   }
   
   slave = ttyname( tty );
   
   if( slave == NULL )
   {
       return( 25 );
   }
   
   if( ioctl(tty, TIOCSCTTY, NULL ) < 0 )
   {
       return( 45 );
   }
   dup2( tty, 0 );
   dup2( tty, 1 );
   dup2( tty, 2 );
   execl
   		 
   FD_ZERO( &rd );
   FD_SET( client, &rd );
   FD_SET( pty, &rd );
   if( select( n + 1, &rd, NULL, NULL, NULL  < 0 )
   {
       return( 49 );
   }
   ```

3. 串口编程 

   ```
   属性：
   	波特率： 串口发送与接收的速度
   	数据宽度： 传输数据的宽度
   	停止位的长度
   	奇偶校验位 
   	
   ```

   

   