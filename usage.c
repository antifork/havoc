/* 
 *  $Id$
 *  <usage>
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


#include <stdio.h>

#include "config.h"


void usage(char *name)
{
	fprintf (stderr, "usage: %s [option]\n", name);

#if HAVE_GETOPT_LONG
	fprintf (stderr,
		"     -i  --if device. ie: eth0, ne0..\n"
		"     -r  --random. use totally random ip address\n"
		"     -v  --version\n"
		"     -h  --help\n");
#else
	fprintf (stderr,
                "     -i  ie: eth0, ne0..\n"
                "     -r  use totally random ip addres\n"
                "     -v  print version\n"
                "     -h  this help\n");
#endif

	exit (0);
}
