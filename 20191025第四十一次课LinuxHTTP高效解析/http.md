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


http://192.168.199.183:8000/cgi-bin/postAuth
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

Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.86 Safari/537.36



```
1.根据用户的平台，推送不同的内容，进行探针信息的记录，存储用的各类平台版本以及浏览器版本 
Debian APT-HTTP/1.3 (1.0.1ubuntu2)

2. 钓鱼网站: 记录用户IP 用户终端类型 用户系统版本  
```

* 解析完整的HTTP 
  * 解析http 请求头 ： url 首部行
  * 判断url的有效性 （/../../../../../../../../../../etc/shadow）
  * 响应的种类  301 302 303 http 响应里的303
* 如何提高http协议处理效率 
  * url---->hash .进行hash匹配 
  * /test/bool/xfdsfsfsdsa/asdsadsad/1132232.html ---- 对应的url





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



# 项目

## uac parse 

​	 sudo apt-get install libyaml-cpp-dev 

​     sudo apt-get install libyaml-dev



 **nmap** -p 80,443 --script /home/offensive/**nmap**_nse/http-vuln-check_openssl.nse --script-args="http.useragent=‘Mozilla/5.0 (compatible [[[email protected\]](http://www.bubuko.com/cdn-cgi/l/email-protection)])‘" --script-trace 192.168.199.183



* HASH 解析URL 链接

```c
typedef struct {
    char    url[MAX_URL_LEN];
    size_t  len;

    int     all;

    UT_hash_handle hh;
} url_t;

typedef struct UT_hash_handle {
   struct UT_hash_table *tbl;
   void *prev;                       /* prev element in app order      */
   void *next;                       /* next element in app order      */
   struct UT_hash_handle *hh_prev;   /* previous hh in bucket order    */
   struct UT_hash_handle *hh_next;   /* next hh in bucket order        */
   void *key;                        /* ptr to enclosing struct's key  */
   unsigned keylen;                  /* enclosing struct's key len     */
   unsigned hashv;                   /* result of hash-fcn(key)        */
} UT_hash_handle;

typedef struct UT_hash_table
{
   UT_hash_bucket *buckets;
   unsigned num_buckets, log2_num_buckets;
   unsigned num_items;
   struct UT_hash_handle *tail; /* tail hh in app order, for fast append    */
   ptrdiff_t hho; /* hash handle offset (byte pos of hash handle in element */

   /* in an ideal situation (all buckets used equally), no bucket would have
    * more than ceil(#items/#buckets) items. that's the ideal chain length. */
   unsigned ideal_chain_maxlen;

   /* nonideal_items is the number of items in the hash whose chain position
    * exceeds the ideal chain maxlen. these items pay the penalty for an uneven
    * hash distribution; reaching them in a chain traversal takes >ideal steps */
   unsigned nonideal_items;

   /* ineffective expands occur when a bucket doubling was performed, but
    * afterward, more than half the items in the hash had nonideal chain
    * positions. If this happens on two consecutive expansions we inhibit any
    * further expansion, as it's not helping; this happens when the hash
    * function isn't a good fit for the key domain. When expansion is inhibited
    * the hash will still work, albeit no longer in constant time. */
   unsigned ineff_expands, noexpand;

   uint32_t signature; /* used only to find hash tables in external analysis */
#ifdef HASH_BLOOM
   uint32_t bloom_sig; /* used only to test bloom exists in external analysis */
   uint8_t *bloom_bv;
   uint8_t bloom_nbits;
#endif

}


HASH_FIND_STR(urls, url, u);

#define HASH_FIND_STR(head,findstr,out)                                          \
    HASH_FIND(hh,head,findstr,(unsigned)uthash_strlen(findstr),out)


#define HASH_FIND(hh,head,keyptr,keylen,out)                                     \
do {                                                                             \
  unsigned _hf_hashv;                                                            \
  HASH_VALUE(keyptr, keylen, _hf_hashv);                                         \
  HASH_FIND_BYHASHVALUE(hh, head, keyptr, keylen, _hf_hashv, out);               \
} while (0


#define HASH_VALUE(keyptr,keylen,hashv)                                          \
do {                                                                             \
  HASH_FCN(keyptr, keylen, hashv);                                               \
} while (0)


#ifdef HASH_FUNCTION
#define HASH_FCN HASH_FUNCTION
#else
#define HASH_FCN HASH_JEN
#endif

#define HASH_JEN(key,keylen,hashv)                                               \
do {                                                                             \
  unsigned _hj_i,_hj_j,_hj_k;                                                    \
  unsigned const char *_hj_key=(unsigned const char*)(key);                      \
  hashv = 0xfeedbeefu;                                                           \
  _hj_i = _hj_j = 0x9e3779b9u;                                                   \
  _hj_k = (unsigned)(keylen);                                                    \
  while (_hj_k >= 12U) {                                                         \
    _hj_i +=    (_hj_key[0] + ( (unsigned)_hj_key[1] << 8 )                      \
        + ( (unsigned)_hj_key[2] << 16 )                                         \
        + ( (unsigned)_hj_key[3] << 24 ) );                                      \
    _hj_j +=    (_hj_key[4] + ( (unsigned)_hj_key[5] << 8 )                      \
        + ( (unsigned)_hj_key[6] << 16 )                                         \
        + ( (unsigned)_hj_key[7] << 24 ) );                                      \
    hashv += (_hj_key[8] + ( (unsigned)_hj_key[9] << 8 )                         \
        + ( (unsigned)_hj_key[10] << 16 )                                        \
        + ( (unsigned)_hj_key[11] << 24 ) );                                     \
                                                                                 \
     HASH_JEN_MIX(_hj_i, _hj_j, hashv);                                          \
                                                                                 \
     _hj_key += 12;                                                              \
     _hj_k -= 12U;                                                               \
  }                                                                              \
  hashv += (unsigned)(keylen);                                                   \
  switch ( _hj_k ) {                                                             \
    case 11: hashv += ( (unsigned)_hj_key[10] << 24 ); /* FALLTHROUGH */         \
    case 10: hashv += ( (unsigned)_hj_key[9] << 16 );  /* FALLTHROUGH */         \
    case 9:  hashv += ( (unsigned)_hj_key[8] << 8 );   /* FALLTHROUGH */         \
    case 8:  _hj_j += ( (unsigned)_hj_key[7] << 24 );  /* FALLTHROUGH */         \
    case 7:  _hj_j += ( (unsigned)_hj_key[6] << 16 );  /* FALLTHROUGH */         \
    case 6:  _hj_j += ( (unsigned)_hj_key[5] << 8 );   /* FALLTHROUGH */         \
    case 5:  _hj_j += _hj_key[4];                      /* FALLTHROUGH */         \
    case 4:  _hj_i += ( (unsigned)_hj_key[3] << 24 );  /* FALLTHROUGH */         \
    case 3:  _hj_i += ( (unsigned)_hj_key[2] << 16 );  /* FALLTHROUGH */         \
    case 2:  _hj_i += ( (unsigned)_hj_key[1] << 8 );   /* FALLTHROUGH */         \
    case 1:  _hj_i += _hj_key[0];                                                \
  }                                                                              \
  HASH_JEN_MIX(_hj_i, _hj_j, hashv);                                             \
} while (0)

#define HASH_FIND_BYHASHVALUE(hh,head,keyptr,keylen,hashval,out)                 \
do {                                                                             \
  (out) = NULL;                                                                  \
  if (head) {                                                                    \
    unsigned _hf_bkt;                                                            \
    HASH_TO_BKT(hashval, (head)->hh.tbl->num_buckets, _hf_bkt);                  \
    if (HASH_BLOOM_TEST((head)->hh.tbl, hashval) != 0) {                         \
      HASH_FIND_IN_BKT((head)->hh.tbl, hh, (head)->hh.tbl->buckets[ _hf_bkt ], keyptr, keylen, hashval, out); \
    }                                                                            \
  }                                                                              \
} while (0)


 HASH_ADD_STR(urls, url, u); 
 
```



* HTTP的协议状态机设计

```c
判断方法:判断HTTP协议的第一个字母是G P 
			每个状态机都有一个跳转条件和输出结果
确定方法； 确定当前状态，根据状态的类型，跳转到不同方法的处理状态 

使用协议状态机实现文件筛选过程，需要从大量文件中，筛选出其中包括hello字段的文件 
文件存在 
权限是不是够 
文件的类型有多种：
	dir
	pipe
	c
	b
	-
文本的分类
FIRST_1_K
SENCONF_1_K

1. 更新状态
2. 循环执行



web:
	IO通信模块 + 数据处理模块 + 数据存储模块 +  reactor +  线程池 + http + https+ cgi
```







