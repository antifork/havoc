/* 
 *  $Id$
 *  <rand module>
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

#include "sys/socket.h"
#include <netinet/in.h>
#include <arpa/inet.h>

char * rand_ip(void)
{
 	struct timeval rnd;
 	struct in_addr rand_in_addr;

  	gettimeofday(&rnd,NULL);
  	srand(rnd.tv_usec);
  	rand_in_addr.s_addr=rand();
 	return (inet_ntoa(rand_in_addr));
}


char * rand_hw(void)
{
 	struct timeval rnd;
 	int rand_hw_addr[ETH_HW_ADDR_LEN], *cont=rand_hw_addr, tmp, i;
 	static char *ret;

  	gettimeofday(&rnd,NULL);
  	srand(rnd.tv_usec);
  	for (i=0;i<3;i++) {
    		tmp=rand();
    		memcpy((int *)cont++,(int *)&tmp,4);
   	}

  	ret=(char*)realloc(ret,13*sizeof(char));
  	snprintf(ret,13,"%.4hx%.4hx%.4hx",rand_hw_addr[0],rand_hw_addr[1],
          	rand_hw_addr[2]);
 	return(ret);
}



char * rand_ip_net(unsigned long address,unsigned long netmask)
{
	unsigned long network=address&netmask;
	static struct in_addr *rnd;

	rnd=(struct in_addr *)realloc(rnd,sizeof(struct in_addr));
	inet_aton(rand_ip(), rnd);		
	rnd->s_addr=network|(rnd->s_addr&(~netmask));	
	return(inet_ntoa(*rnd));
}
