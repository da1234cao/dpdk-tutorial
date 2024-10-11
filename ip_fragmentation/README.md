# 简介

- [9. IP Fragmentation Sample Application — Data Plane Development Kit 24.11.0-rc0 documentation](https://doc.dpdk.org/guides/sample_app_ug/ip_frag.html)
- [11. Longest Prefix Match (LPM) Library — Data Plane Development Kit 24.11.0-rc0 documentation](https://doc.dpdk.org/guides/prog_guide/lpm_lib.html?highlight=lpm)

1. 实现了一个路由功能。使用 LPM 库，将匹配的流量转发到指定 port。(匹配不上，送回流量来的端口)
    - 添加规则
    - 查找规则
    - 删除规则
2. 数据包过大(和MTU比较，MTU 只计算 IP 层的数据，也就是 网络层 负载的最大大小)，数据包切片发送。

这些都是 API 封装好的。如果全部手动实现，那是非常非常麻烦。