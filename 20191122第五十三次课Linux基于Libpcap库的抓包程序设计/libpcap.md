# Libpcap 编程

* libpcap ：流量镜像、流量过滤、流量抓取等功能 
  * 数据包截获:抓到流经网卡的原始数据包
  * 自定义数据包发送:任意构造原始数据包发送
  * 流量采集与统计
  * 过滤规则 ：自带流量匹配库
* 在线安装：sudo apt-get install libpcap-dev 
  * 离线安装: xxx.tar.gz   tar zxvf  ./configure make make install  
* libpcap 工作原理

```c
1. libpcap的工作原理是基于数据旁路镜像 
2. libpcap 提供了查找网络设备的接口，  char *pcap_lookupdev(char *errbuf)
3. libpcap 提供底层网卡打开的接口 pcap_open_live 打开某块网卡 
4. libpcap 提供获取网卡参数的接口 pcap_lookupnet()
5. libpcap 提供设置过滤器的接口，过滤器的编译pcap_compile() ，设置过滤器pcap_setfilter
6. libpcap 提供了回调函数调用接口，pcap_loop 
7. 关闭网卡 pcap_close 


pcap_lookupnet  bpf_u_int32 *netp
struct sock_addr_in addr 
addr.s_addr = netp
net = inet_ntoa(addr);
```

* bpf

```c
src host 127.0.0.1 
dst port 8000
not tcp 
tcp[13]==0x2 and (dst port 80 or dst port 8080)
icmp[icmptype]==icmp-echoreply or icmp[icmptype]==icmp-echo
ether dsf 12:24:56:78
```

* API

```c
 pcap_setfilter(pcap_t *p, struct bpf_program *fp);
 typedef void (*pcap_handler)(u_char *user, const struct pcap_pkthdr *h,
                                   const u_char *bytes);
 int pcap_loop(pcap_t *p, int cnt,pcap_handler callback, u_char *user);


```

* demo

```c
struct bpf_program filter;
pcap_compile(device,&filter,"dsp port 443",1,0);
pcap_setfilter(device,&filter);
```

* 流程

```
1. 查找网络设备
2. 打开网络设备
3. 查找设备信息
4. 输入过滤规则
5. 编译过滤规则
6. 设置过滤规则
7. 启动捕获数据包
8. 对捕获的数据包进行分析 
9. 解析后的结果发送或存储
```

![1574432864568](C:\Users\chandler\AppData\Roaming\Typora\typora-user-images\1574432864568.png)

