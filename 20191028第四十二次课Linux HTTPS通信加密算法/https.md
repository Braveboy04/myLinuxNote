# https 协议
## 概念 
https=http+SSL
SSL：加密套接字 
ssl加密使用openssl库 
openssl :
	1. ssl加密套件
	2. https的通信 
   apt-get install openssl 

## 加密基础知识

1.对称加密：指的是加密方和解密方使用的是同一个密钥
优点：加密解密的速度很快	
缺点：如果两个从未通信过的用户要进行通信的时候，
该如何把解密的密钥传输给对方呢（密钥仍然要在网络上传输，所以密钥还是可能会被“中间人”截获），这是对称加密最大的缺点；
常见的对称加密算法有：
    DES：使用56位的密钥，2000年的时候被人破解了，所以现在基本不再使用
    AES：高级加密标准，可以使用128，129，256 512 1024 2048 4096密钥
其他的还有blowfish,Twofish和RC6,IDEA(商业算法)，CAST5等

2. 非对称加密：非对称加密方式解决了对称加密的缺陷，它的加密和解密密钥是不同的，比如对一组数字加密，我们可以用公钥对其加密，然后我们想要将其还原，就必须用私钥进行解密，公钥和私钥是配对使用的，常见的非对称加密算法有：
优点： 加密和解密 用不同密钥 
缺点：加密速度非常慢  
RSA：既可以用来加密解密，又可以用来实现用户认证
DSA：只能用来加密解密，所以使用范围没有RSA广
非对称加密长度通常有512，1024，2048，4096位，最常用的就是2048位，长度固然可以增加安全性但是需要花费很长时间来进行加密/解密，和对称加密相比，加密/解密的时间差不多是对称加密的1000倍，所以我们通常用其作为用户认证，用对称加密来实现数据的加密/解密

3. 单项加密：
单向加密就是用来计算一段数据的特征码的，为了防止用户通过“暴力破解”的方式解密，所以单向加密一般具有“雪崩效应”就是说：只要被加密内容有一点点的不同，加密所得结果就会有很大的变化。单项加密还有一个特点就是无论被加密的内容多长/短，加密的结果（就是提取特征码）是定长的，用途：用于验证数据的完整性，常用的单项加密算法
特点：加密后的数据长度固定:128
	 雪崩效应 
MD5:这种加密算法固定长度为128位
SHA1：这种加密算法固定长度是160位





## HTTPS简介
### ssl加密流程 
* 每个数据包都有不同的对称加密密钥 ---随机生成
* 如何获得对方的公钥 
* 证书：
	 1. 公钥
数据 -->单项加密-
			---->对称加密
				----->非对称加密(使用对方的公钥做加密的密钥)
* 每次进行数据传输的时候，每包数据都有一个随机生成的密钥 
## 自建根证书 
.key ：通过传入的口令，按照系统的算法生成的私钥
openssl genrsa -des3 -out root.key
.csr格式：证书签名请求（证书请求文件），含有公钥信息，certificate signing request的缩写
openssl req -new -key root.key -out root.csr
.crt格式：证书文件，certificate的缩写
openssl x509 -req -days 3650 -sha1 -extensions v3_ca -signkey root.key -in root.csr -out root.crt
.crl格式：证书吊销列表，Certificate Revocation List的缩写
.pem格式：用于导出，导入证书时候的证书的格式，有证书开头，结尾的格式


## 生成服务器证书，并用自建根证书进行签名
4. 创建服务器证书密钥：server.key

openssl genrsa -out server.key 2048

去除key文件的口令：openssl rsa -in server.key -out server.key

5. 创建服务器证书申请文件：server.csr

openssl req -new -key server.key -out server.csr

6. 创建服务证书：server.crt

openssl x509 -req -days 730 -sha1 -extensions v3_req -CA root.crt -CAkey root.key -CAcreateserial -in server.csr -out server.crt

注意，-days为证书有效期



## openssl工具库用法：
查看openssl组件是否安装
rpm -ql openssl
/usr/lib64/libcrypt.so 
/usr/bin/openssl --- 加密
openssl version     

对文件进行加密测试输出到fstab.des3
openssl enc -des3 -in fstab -e -out fstab.des3

对文件进行解密
openssl enc -des3 -in fstab.des3 -d -out fstab

对文件进行单项加密 
openssl dgst -md5 fstab

### ssl协议功能
* 保证传输数据的保密性
* 保证传输数据的完整性  rc4 
* 实现通信双方的互相身份认证-----非对称加密



### ssl加密 解密过程
* 加密过程
	单项加密  对称加密  非对称加密 
* 证书
	对方的公钥 
	数字签名--->公钥的单项加密结果
* https通信中 要进行证书交互 
* SSL 协议是一个分层的协议，共有两层组成
* 高层协议包括 
   * SSL 握手协议
   * 改变加密约定协议
   * 报警协议
* 处于 SSL 协议的底层
   * SSL记录层协议

* 建立ssl套接字 
	


getopt 参数解析
./webd  -p 80 -k server.key -c server.cert -l chain.cert 


编译ssl程序时
* 安装ssl的库
* -lopenssl




./tls_one_way_server -p john123 -k ../pem/server/server.key.pem -c ../pem/server/server.cert.pem -l ../pem/server/server_chain.cert.pem


