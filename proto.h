/* function prototypes begin */
void fatalerr(char *,...);
void usage(char *);
void get_ip_addr(struct in_addr *,char*);
void get_hw_addr(char*,char*);
void send_arp(char *src_ip_addr,char *src_hw_addr,char *targ_ip_addr,
        char *targ_hw_addr,char *device);
char * rand_ip(void);
char * rand_hw(void);
int get_if(struct sockaddr_in *ip,struct sockaddr_in *nm,char *device);
char * rand_ip_net(unsigned long address,unsigned long netmask);
/* function prototypes end */
