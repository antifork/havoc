struct arp_packet {
        u_char targ_hw_addr[ETH_HW_ADDR_LEN];
        u_char src_hw_addr[ETH_HW_ADDR_LEN];
        u_short frame_type;
        u_short hw_type;
        u_short prot_type;
        u_char hw_addr_size;
        u_char prot_addr_size;
        u_short op;
        u_char sndr_hw_addr[ETH_HW_ADDR_LEN];
        u_char sndr_ip_addr[IP_ADDR_LEN];
        u_char rcpt_hw_addr[ETH_HW_ADDR_LEN];
        u_char rcpt_ip_addr[IP_ADDR_LEN];
        u_char padding[18]; };
