
# service core

## 前言

参考文档：
- [25. Service Cores Sample Application — Data Plane Development Kit 24.11.0-rc0 documentation](https://doc.dpdk.org/guides/sample_app_ug/service_cores.html)
- [4. Service Cores — Data Plane Development Kit 24.11.0-rc0 documentation](https://doc.dpdk.org/guides/prog_guide/service_cores.html)

文档中写道，service core 提供了两个功能：(1) 有的调度程序在硬件中完成，有的调度程序是在软件中完成，service core 可以屏蔽这种差异。-- 不清楚，暂时没遇到过。(2) 在 n 个 core 上，运行 m 个 service。-- 这个我基本知道是咋回事，示例代码中演示的也是这个功能。

## service core的使用

第一步是注册并启用service，API 的调用顺序如下：
1. `rte_service_component_register`: 注册一个服务。最多可以注册64个服务
2. `rte_service_component_runstate_set`: 设置component状态
3. `rte_service_runstate_set`: 设置app状态 (可能会好奇，这两个状态有什么区别。区别是层次不同，具体未知，顺着调用即可)

第二步是建立service 和 core 的映射：
1. `rte_service_lcore_add` : 设置(覆盖)这个核为serice使用，标志位 `ROLE_SERVICE`
2. `rte_service_map_lcore_set`: 设置 core 与 servce 的映射。一个 core 可以对应多个 service。
3. `rte_service_lcore_start`: 每个 core 循环迭代执行，映射给自己的 service。如果 core 上没有 serice，core 会空转。

第三步是选择是否启用记录统计信息，记录 service 执行的 cycle 数：
1. `rte_service_set_stats_enable`: 启用/停止记录统计信息
2. `rte_service_dump`: 打印统计信息