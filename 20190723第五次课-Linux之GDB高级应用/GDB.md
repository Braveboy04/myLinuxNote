# Linux的调试器 
## 概念
* 基于命令行的调试方法 
* 所有的调试都是可以进行脚本编写的
* 能够调试所有架构的代码
* 有三种调试方法供大家选择
* GDB支持远程调试，支持与IDA进行联调 






## 安装GDB增强工具 (gef)
* GDB的版本大于7.7  
* wget -q -O- https://github.com/hugsy/gef/raw/master/scripts/gef.sh | sh
* 确保网络连通 并且成功更新ubuntu (更新source.list 使用apt-get update) 


## 尝试使用gdb进行小程序的调试
* gcc -g的选项 
* gdb gdb-test 
* 一些GDB的命令 

## GDB的命令 
#### 启动方法
* 本地普通启动 gdb <program> 
* 本地段错误文件启动 gdb <program> core 
* attch方式启动 gdb <program> <PID>
* 远程启动 gdbserver 0.0.0.0:1234 /path/to/file

#### 启动选项 
-se sys-file 

#### 基本命令
set listsize 
set args 10 20 30 40 50
path
show paths 
save breakpoint name.bp
gdb elf -x name.bp
print *array@10
print file::variable 
x/n、f、u
n 是一个正整数，表示显示内存的长度，也就是说从当前地址向后显示几个地址的内容。
f 表示显示的格式，跟print 的格式参数相同
u 表示从当前地址往后请求的字节数，如果不指定的话，GDB默认是4个bytes。u参数可以用下面的字符来代替，b表示单字节，h表示双字节，w表示四字节，g表示八字节。当我们指定了字节长度后，GDB会从指内存定的内存地址开始，读写指定字节，并把其当作一个值取出来。


ps -A 查看当前进程
strip test-1
bt 
info locals


#### Linux程序发布流程
* 确定程序是否存在符号表 
readelf -s test-1 
* 生成符号表
objcopy --only-keep-debug test-1 test-1.symbol
* 生成发布程序 
objcopy --strip-debug test-1 test-release
* 使用符号表进行程序debug
gdb -q --symbol=test-1.symbol --exec=test-release



#### GDB安装插件
git clone https://github.com/gatieme/GdbPlugins.git ~/GdbPlugins

echo "source ~/GdbPlugins/peda/peda.py" > ~/.gdbinit
echo "source ~/GdbPlugins/gef/gef.py" > ~/.gdbinit
echo "source ~/GdbPlugins/gdbinit/gdbinit" > ~/.gdbinit



#### GDB中暂停/恢复程序运行
* 断点
 条件断点
 break     if 
 info breakpoints
 delete   
 disable  
 enable  


condition 1 i==90
condition 1



* 观察点
 watch  地址
 info watchpoints
 rwatch 

* 捕捉点
catch event
throw 抛出一个C++的异常 catch throw
catch 捕捉一个C++的异常 catch catch
exec  调用系统调用exev时停止 catch exec
fork  调用系统调用fork时停止 catch fork
load/load libname 载入动态链接库时 catch load / catch load libname
unload


暂停命令
commands bnum 
.
.
.
.
end


