/*
 *  $Id$
 *  <if.c>
 *
 *  Copyright (c) 2000 raptor <raptor@antifork.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */


#include "symbols.h"
#include "config.h"
#include "std.h"
#include "if.h"
#include "local.h"
#include "proto.h"


int get_if(struct sockaddr_in *ip,struct sockaddr_in *nm,char *device) 
{
        char buffer[10240];
        int sd;
        struct ifreq *ifr, *iflast, ifreq_io;
        struct ifconf ifc;

        memset(buffer,0,10240);

        if ((sd=socket(AF_INET,SOCK_DGRAM,0))==-1) 
			fatalerr("err: %s",strerror(errno));
        ifc.ifc_len=sizeof(buffer);
        ifc.ifc_buf=buffer;

        if (ioctl(sd,SIOCGIFCONF,&ifc)<0) 
			fatalerr("err: %s",strerror(errno));
        close(sd);

        ifr=(struct ifreq *)buffer;
        iflast=(struct ifreq *)((char *)buffer + ifc.ifc_len);

        if ((sd=socket(AF_INET,SOCK_DGRAM,0))==-1) 
			fatalerr("err: %s",strerror(errno));

#if HAVE_SOCKADDR_SALEN
        for (;ifr<iflast;
        (char *)ifr += sizeof(ifr->ifr_name)+ifr->ifr_addr.sa_len)
#else
        for (;ifr<iflast;
	(char *)ifr += sizeof(ifr->ifr_name)+sizeof(struct sockaddr_in))
#endif

        {
                if ((*(char *)ifr)&&(!strcmp(ifr->ifr_name,device))) {
			/* get the ip address */
			memcpy(ip,(void *)&ifr->ifr_addr,
				sizeof(struct sockaddr_in));

                        memcpy(&ifreq_io, ifr, sizeof(ifr->ifr_name) 
				+ sizeof(struct sockaddr_in));

                        if (ioctl(sd,SIOCGIFNETMASK,&ifreq_io)<0) 
				fatalerr("err: %s",strerror(errno));	
                        else /* get the netmask */
				memcpy(nm,(void *)&ifreq_io.ifr_addr,
					sizeof(struct sockaddr_in));
			return(0);
                }
        }
        return(1);
}


void send_arp(char *src_ip_addr, char *src_hw_addr, char *targ_ip_addr, 
            char *targ_hw_addr, char *device)
{
 	struct in_addr src_in_addr,targ_in_addr;
 	struct arp_packet pkt;
 	struct sockaddr sa;
 	int sock;

 	sock=socket(AF_INET,SOCK_PACKET,htons(ETH_P_RARP));

 	if (sock<0) 
   		fatalerr("err-socket:%s",strerror(errno));

 	pkt.frame_type = htons(ARP_FRAME_TYPE);
 	pkt.hw_type = htons(ETHER_HW_TYPE);
 	pkt.prot_type = htons(IP_PROTO_TYPE);
 	pkt.hw_addr_size = ETH_HW_ADDR_LEN;
 	pkt.prot_addr_size = IP_ADDR_LEN;
 	pkt.op=htons(OP_ARP_REQUEST);

 	get_hw_addr(pkt.targ_hw_addr,targ_hw_addr);
 	get_hw_addr(pkt.rcpt_hw_addr,targ_hw_addr);
 	get_hw_addr(pkt.src_hw_addr,src_hw_addr);
 	get_hw_addr(pkt.sndr_hw_addr,src_hw_addr);

 	get_ip_addr(&src_in_addr,src_ip_addr);
 	get_ip_addr(&targ_in_addr,targ_ip_addr);

 	memcpy(pkt.sndr_ip_addr,&src_in_addr,IP_ADDR_LEN);
 	memcpy(pkt.rcpt_ip_addr,&targ_in_addr,IP_ADDR_LEN);

 	bzero(pkt.padding,18);

 	strcpy(sa.sa_data,device);

 	if (sendto(sock,&pkt,sizeof(pkt),0,&sa,sizeof(sa)) < 0) 
   		fatalerr("err sendto:%s",strerror(errno));

	close(sock);
 	return;
}


void get_ip_addr(struct in_addr* in_addr,char* str)
{
 	struct hostent *hostp;

 	in_addr->s_addr=inet_addr(str);
 	if (in_addr->s_addr == -1) {

   		if ( (hostp = gethostbyname(str)))
      			bcopy(hostp->h_addr,in_addr,hostp->h_length);
   		else 
     			fatalerr("send_arp: unknown host %s",str);
  	}
}

	
void get_hw_addr(char* buf,char* str)
{
 	int  i=0;
 	char c='\0',val='\0';

 	for (i=0;i<ETH_HW_ADDR_LEN;i++) {
   		if ( !(c = tolower(*str++))) 	
				fatalerr("Invalid hardware address");
   		if (isdigit(c)) val = c-'0';
    		else if (c >= 'a' && c <= 'f') val = c-'a'+10;
     			else fatalerr("Invalid hardware address");

   		*buf = val << 4;
   		if ( !(c = tolower(*str++))) 
			fatalerr("Invalid hardware address");
   		if (isdigit(c)) val = c-'0';
    		else if (c >= 'a' && c <= 'f') val = c-'a'+10;
     			else fatalerr("Invalid hardware address");

   		*buf++ |= val;

   		if (*str == ':') str++;
  	}
}
