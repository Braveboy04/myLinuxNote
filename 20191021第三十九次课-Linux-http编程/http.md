#   HTTP协议解析

## 概念
* 超文本传输协议:承载的数据类型多
* 持久性链接和非持久性链接类型


## 协议分析
1. 静态分析

   HTTP解析：
       请求行： 由3个字段组成：方法字段、URL字段、HTTP版本 并且以/r/n结尾 

   ​	首部行：  各类信息的键值对

2. 动态分析

## 请求方式

get 

```
Frame 4585: 573 bytes on wire (4584 bits), 573 bytes captured (4584 bits) on interface 0
Ethernet II, Src: IntelCor_6f:98:34 (58:91:cf:6f:98:34), Dst: Vmware_85:72:a5 (00:0c:29:85:72:a5)
Internet Protocol Version 4, Src: 192.168.199.26, Dst: 192.168.199.183
Transmission Control Protocol, Src Port: 50780, Dst Port: 8000, Seq: 1, Ack: 1, Len: 519
Hypertext Transfer Protocol
    GET /cgi-bin/getAuth?email=1111%40qq.com&password=12345678 HTTP/1.1\r\n
    Host: 192.168.199.183:8000\r\n
    Connection: keep-alive\r\n
    Upgrade-Insecure-Requests: 1\r\n
    User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.86 Safari/537.36\r\n
    Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3\r\n
    Referer: http://192.168.199.183:8000/getAuth.html\r\n
    Accept-Encoding: gzip, deflate\r\n
    Accept-Language: zh-CN,zh;q=0.9\r\n
    \r\n
    [Full request URI: http://192.168.199.183:8000/cgi-bin/getAuth?email=1111%40qq.com&password=12345678]
    [HTTP request 1/1]
    [Response in frame: 4588]
    
    
GET /cgi-bin/getAuth?email=1111%40qq.com&password=12345678 HTTP/1.1\r\n
```

post

```
Frame 1154: 712 bytes on wire (5696 bits), 712 bytes captured (5696 bits) on interface 0
Ethernet II, Src: IntelCor_6f:98:34 (58:91:cf:6f:98:34), Dst: Vmware_85:72:a5 (00:0c:29:85:72:a5)
Internet Protocol Version 4, Src: 192.168.199.26, Dst: 192.168.199.183
Transmission Control Protocol, Src Port: 50872, Dst Port: 8000, Seq: 1, Ack: 1, Len: 658
Hypertext Transfer Protocol
    POST /cgi-bin/postAuth HTTP/1.1\r\n
    Host: 192.168.199.183:8000\r\n
    Connection: keep-alive\r\n
    Content-Length: 42\r\n
    Cache-Control: max-age=0\r\n
    Origin: http://192.168.199.183:8000\r\n
    Upgrade-Insecure-Requests: 1\r\n
    Content-Type: application/x-www-form-urlencoded\r\n
    User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.86 Safari/537.36\r\n
    Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3\r\n
    Referer: http://192.168.199.183:8000/postAuth.html\r\n
    Accept-Encoding: gzip, deflate\r\n
    Accept-Language: zh-CN,zh;q=0.9\r\n
    \r\n
    [Full request URI: http://192.168.199.183:8000/cgi-bin/postAuth]
    [HTTP request 1/1]
    [Response in frame: 1159]
    File Data: 42 bytes
HTML Form URL Encoded: application/x-www-form-urlencoded
    Form item: "email" = "dasdsada@qq.com"
    Form item: "password" = "123456789"

```



# 首部行

1. Host ：主机地址  域名   ip:addr
2. Connection : keep-alive       close
3. Content-Length: 42
4. Cache-Control: max-age=0
5. Origin: http://192.168.199.183:8000
6. User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/
7. Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3
8. Accept-Encoding: gzip, deflate



# HTTP proxy

1. burp suit 

## http协议实现流程
1.利用url中的主机信息，进行主机tcp连接
2.利用url中的路径和参数等信息，对主机进行文件访问(http请求)
3.服务器根据请求路径，从数据库取出文件
4.服务器将取出的文件打包为http响应发回客户端
5.客户端接收响应数据

http://www.sxtyu.com:80/news/index.html

http://192.168.199.183:8000/

## http协议解析
```
GET /somedir/page.html HTTP/1.1/r/n　　
Host:www.chinaitlab.com/r/n　
Connection:close/r/n　
User-agent:Mozilla/4.0/r/n　　
Accept-language:zh-cn/r/n

GET:http类型 
	post get head 
url：路径

$root/somedir/page.html 

　HTTP/1.1 200 0K　　Connectlon:close　　Date: Thu, 13 Oct 2005 03:17:33 GMT　　Server: Apache/2.0.54 (Unix)　　Last—Nodified:Mon,22 Jun 1998 09;23;24 GMT　　Content—Length:682l　　Content—Type:text/html	

```


　HTTP/1.1 200 0K　  状态行
　Connectlon:close　
　Date: Thu, 13 Oct 2005 03:17:33 GMT　
　Server: Apache/2.0.54 (Unix)　
　Last—Nodified:Mon,22 Jun 1998 09;23;24 GMT　
　Content—Length:682l　
　Content—Type:text/html


## http代理

