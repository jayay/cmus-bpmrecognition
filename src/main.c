/*
 * Copyright 2016 jayay
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "bpmread.h"
#include "misc.h"
#include "xstrjoin.h"
#include "cache.h"
#include "input.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *id3_default_charset;

int main(int argc, char **argv)
{
	misc_init();
	cache_init();
	ip_load_plugins();

#ifndef BINTOSTDOUT
	ip_dump_plugins();
#endif

	FILE * fp;
	char * file = NULL;
	size_t len = 0;
	ssize_t read;

	char *cmus_lib_file = xstrjoin(cmus_config_dir, "/lib.pl");
	id3_default_charset = "ISO-8859-1";

#ifndef BINTOSTDOUT
	printf("library file: %s\n", cmus_lib_file);
#endif

	fp = fopen(cmus_lib_file, "r");

	if (fp == NULL)
		exit(EXIT_FAILURE);

	while ((read = getline(&file, &len, fp)) != -1) {
		file[read-1] = '\0';

#ifndef BINTOSTDOUT
		printf("scanning %s\n", file);
#endif

		int state = writeBpm(file, 0);

		if  (state < 0) {
			printf("Error!\n");
			continue;
		}
	}

	fclose(fp);
	free(cmus_lib_file);

#ifndef BINTOSTDOUT
	printf("Writing Cache...\n");
#endif
	cache_close();

	exit(EXIT_SUCCESS);
}
