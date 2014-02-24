/* zgrep.c - simple standalone grep
 * Copyright (C) 2014 Sean MacLennan
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this project; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "z.h"

#if INTERNAL_GREP

#ifndef WIN32
#ifdef HAVE_DIRECT
#include <sys/dir.h>
#define dirent direct
#else
#include <dirent.h>
#endif
#endif

#include <fnmatch.h>

static void grep_one(char *fname, Byte *ebuf,
		     struct buff *inbuff, struct buff *outbuff)
{
	bswitchto(inbuff);
	bempty();

	if (breadfile(fname))
		return;
		
	while (step(ebuf)) {
		struct mark *start;
		unsigned line;

		blocation(&line);

		bswitchto(outbuff);
		snprintf(PawStr, STRMAX, "%s:%d: ", fname, line);
		binstr(PawStr);
		bswitchto(inbuff);

		tobegline();
		start = bcremrk();
		toendline();
		bmove1(); /* grab NL */
		bcopyrgn(start, outbuff);
		unmark(start);
	}
}

static void grepit(char *input, char *files)
{
	Byte ebuf[ESIZE];
	DIR *dir;
	struct dirent *ent;
	struct buff *inbuff, *outbuff = Curbuff;

	int rc = compile((Byte *)input, ebuf, &ebuf[ESIZE]);
	if (rc) {
		regerr(rc);
		return;
	}

	/* SAM this could be smarter and pull the dir off the files spec */
	dir = opendir(".");
	if (!dir) {
		error("Unable to open directory");
		return;
	}

	inbuff = bcreate();
	if (!inbuff) {
		error("Unable to create tmp file buffer.");
		goto cleanup;
	}

	while ((ent = readdir(dir)))
		if (fnmatch(files, ent->d_name, 0) == 0)
			grep_one(ent->d_name, ebuf, inbuff, outbuff);

cleanup:
	closedir(dir);
	bswitchto(outbuff);
	if (inbuff)
		bdelbuff(inbuff);
}

void Zgrep(void)
{
	char input[STRMAX + 1], files[STRMAX + 1];
	struct wdo *save = Curwdo;

	getbword(input, STRMAX, bistoken);
	if (getarg("Regex: ", input, STRMAX))
		return;

	if (Curbuff->bmode & CMODE)
		strcpy(files, "*.[ch]");
	else if (Curbuff->bmode & SHMODE)
		strcpy(files, "*.sh");
	else
		strcpy(files, "*");
	if (getarg("File(s): ", files, STRMAX))
		return;

	if (wuseother(MAKEBUFF)) {
		grepit(input, files);
		wswitchto(save);
	}
}

#endif