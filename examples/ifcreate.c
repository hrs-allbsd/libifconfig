/*
 * Copyright (c) 2016, Marie Helene Kvello-Aune
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * thislist of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <err.h>
#include <errno.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libifconfig/libifconfig.h>


int main(int argc, char *argv[])
{
	if (argc != 2) {
		errx(EINVAL, "Invalid number of arguments."
		    " Only one argument is accepted, and it should be the name"
		    " of the interface to be created.");
	}

	char *ifname, *ifactualname;

	/* We have a static number of arguments. Therefore we can do  it simple. */
	ifname = strdup(argv[1]);

	printf("Requested interface name: %s\n", ifname);

	libifc_handle_t *lifh = libifc_open();
	if (libifc_create_interface(lifh, ifname, &ifactualname) == 0) {
		printf("Successfully created interface '%s'\n", ifactualname);
		libifc_close(lifh);
		lifh = NULL;
		free(ifname);
		free(ifactualname);
		return (0);
	} else {
		switch (libifc_err_errtype(lifh)) {
		case SOCKET:
			warnx("couldn't create socket. This shouldn't happen.\n");
			break;
		case IOCTL:
			if (libifc_err_ioctlreq(lifh) == SIOCIFCREATE2) {
				warnx(
					"Failed to create interface (SIOCIFCREATE2)\n");
			}
			break;
		default:
			warnx(
				"This is a thorough example accomodating for temporary"
				" 'not implemented yet' errors. That's likely what happened"
				" now. If not, your guess is as good as mine. ;)"
				" Error code: %d\n", libifc_err_errno(
					lifh));
			break;
		}

		libifc_close(lifh);
		lifh = NULL;
		free(ifname);
		free(ifactualname);
		return (-1);
	}
}
