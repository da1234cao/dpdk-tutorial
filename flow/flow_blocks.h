#pragma once

struct rte_flow *generate_ipv4_flow(uint16_t port_id, uint16_t rx_q,
                                    uint32_t src_ip, uint32_t src_mask,
                                    uint32_t dest_ip, uint32_t dest_mask,
                                    struct rte_flow_error *error);