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
5. libpcap 提供设置过滤器的接口，过滤器的编译pcap_compile() ，设置过滤器
6. libpcap 提供了回调函数调用接口，pcap_loop 
7. 关闭网卡 pcap_close 


pcap_lookupnet  bpf_u_int32 *netp
struct sock_addr_in addr 
addr.s_addr = netp
net = inet_ntoa(addr);
```

* bpf

```bpf
src host 127.0.0.1 
dst port 8000
not tcp 
tcp[13]==0x2 and (dst port 80 or dst port 8080)
icmp[icmptype]==icmp-echoreply or icmp[icmptype]==icmp-echo
ether dsf 12:24:56:78

```



