[toc]

## 前言

本文介绍[dpdk flow](https://doc.dpdk.org/guides/prog_guide/rte_flow.html) api的简单使用。

dpdk flow 的使用过程和 [iptables](https://linux.die.net/man/8/iptables) 差不多。即，数据包匹配上指定的规则后，执行对应的动作。

本文示例代码来自[Basic RTE Flow Filtering Sample Application](https://doc.dpdk.org/guides/sample_app_ug/flow_filtering.html)。

## 基本概念

总的来说：dpdk flow 可以对数据包的协议头等进行匹配。可能执行的动作是丢弃流量，将流量转移到指定队列等。

dpdk flow 有这些基本概念：

1. `Attributes`: 数据流规则的属性，如其方向（入口或出口）和优先级。(这里面还有个group属性。 group 0是默认必经过的。如果想到其他group，需要执行JUMP操作。其他group中有不同的action。这些group可能被物理设备支持，或者以虚拟的方式支持。示例代码中没有显示使用group，可以暂时不管)
2. `Pattern item`: 描述匹配的样式。即，什么样的包会命中样式。
3. `Actions`: 当数据包与样式匹配时，执行的操作。

## 示例代码说明

下面这个示例代码来自“前言”中的链接。含义是，当`port_id`端口进入的数据包，满足`src_ip/mask -> dst_ip/mask`的pattern时，将其转移到`rx_q`队列上。

代码还是比较简单的。我简单介绍下。

1. `Attributes` 在dpdk中的结构体是`rte_flow_attr`。`attr.ingress = 1;` 表示 Pattern 将应用在入站流量上。
2. `Pattern` 在dpdk中的结构是`rte_flow_item`。代码中填充了一个`src_ip/mask -> dst_ip/mask`样式。
3. `Actions` 在dpdk中的结构是`rte_flow_action`。代码中填充了一个将流量转移到指定队列的动作。
4. 相关函数：`rte_flow_create`则是在指定端口上,创建一个flow rule。

```c

/**
 * create a flow rule that sends packets with matching src and dest ip
 * to selected queue.
 *
 * @param port_id
 *   The selected port.
 * @param rx_q
 *   The selected target queue.
 * @param src_ip
 *   The src ip value to match the input packet.
 * @param src_mask
 *   The mask to apply to the src ip.
 * @param dest_ip
 *   The dest ip value to match the input packet.
 * @param dest_mask
 *   The mask to apply to the dest ip.
 * @param[out] error
 *   Perform verbose error reporting if not NULL.
 *
 * @return
 *   A flow if the rule could be created else return NULL.
 */

/* Function responsible for creating the flow rule. 8< */
struct rte_flow *generate_ipv4_flow(uint16_t port_id, uint16_t rx_q,
                                    uint32_t src_ip, uint32_t src_mask,
                                    uint32_t dest_ip, uint32_t dest_mask,
                                    struct rte_flow_error *error) {
  /* Declaring structs being used. 8< */
  struct rte_flow_attr attr;
  struct rte_flow_item pattern[MAX_PATTERN_NUM];
  struct rte_flow_action action[MAX_ACTION_NUM];
  struct rte_flow *flow = NULL;
  struct rte_flow_action_queue queue = {.index = rx_q};
  struct rte_flow_item_ipv4 ip_spec;
  struct rte_flow_item_ipv4 ip_mask;
  /* >8 End of declaring structs being used. */
  int res;

  memset(pattern, 0, sizeof(pattern));
  memset(action, 0, sizeof(action));

  /* Set the rule attribute, only ingress packets will be checked. 8< */
  memset(&attr, 0, sizeof(struct rte_flow_attr));
  attr.ingress = 1;
  /* >8 End of setting the rule attribute. */

  /*
   * create the action sequence.
   * one action only,  move packet to queue
   */
  action[0].type = RTE_FLOW_ACTION_TYPE_QUEUE;
  action[0].conf = &queue;
  action[1].type = RTE_FLOW_ACTION_TYPE_END;

  /*
   * set the first level of the pattern (ETH).
   * since in this example we just want to get the
   * ipv4 we set this level to allow all.
   */

  /* Set this level to allow all. 8< */
  pattern[0].type = RTE_FLOW_ITEM_TYPE_ETH;
  /* >8 End of setting the first level of the pattern. */

  /*
   * setting the second level of the pattern (IP).
   * in this example this is the level we care about
   * so we set it according to the parameters.
   */

  /* Setting the second level of the pattern. 8< */
  memset(&ip_spec, 0, sizeof(struct rte_flow_item_ipv4));
  memset(&ip_mask, 0, sizeof(struct rte_flow_item_ipv4));
  ip_spec.hdr.dst_addr = htonl(dest_ip);
  ip_mask.hdr.dst_addr = dest_mask;
  ip_spec.hdr.src_addr = htonl(src_ip);
  ip_mask.hdr.src_addr = src_mask;
  pattern[1].type = RTE_FLOW_ITEM_TYPE_IPV4;
  pattern[1].spec = &ip_spec;
  pattern[1].mask = &ip_mask;
  /* >8 End of setting the second level of the pattern. */

  /* The final level must be always type end. 8< */
  pattern[2].type = RTE_FLOW_ITEM_TYPE_END;
  /* >8 End of final level must be always type end. */

  /* Validate the rule and create it. 8< */
  res = rte_flow_validate(port_id, &attr, pattern, action, error);
  if (!res)
    flow = rte_flow_create(port_id, &attr, pattern, action, error);
  /* >8 End of validation the rule and create it. */

  return flow;
}
/* >8 End of function responsible for creating the flow rule. */
```

## 附录

### 最后

这个示例我在虚拟机上跑不起来。因为我的虚拟网卡的驱动是virtio。

我们现在已经了解了 dpdk flow 最简单的使用了。难度大概是 “1+1=2”。

如果想要在日常工作中使用dpdk flow, 难度大概是解二元一次方程，需要看更多的示例代码。