/* bcmds.c - buffer oriented commands
 * Copyright (C) 1988-2013 Sean MacLennan
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

char Lbufname[BUFNAMMAX + 1];

static char **Bnames;			/* array of ptrs to buffer names */
static int Numbuffs;			/* number of buffers */
static int maxbuffs;			/* max buffers Bnames can hold */


static void switchto_part(void)
{
	char word[STRMAX + 1];
	struct buff *tbuff;

	getbtxt(word, STRMAX);
	tbuff = cfindbuff(word);
	if (!tbuff)
		tbuff = Curbuff;
	if (tbuff->next)
		tbuff = tbuff->next;
	else
		tbuff = Bufflist;
	makepaw(tbuff->bname, true);
}

void Zswitch_to_buffer(void)
{
	int rc;
	char *was = Curbuff->bname;

	Arg = 0;
	Nextpart = switchto_part;
	rc = getplete("Buffer: ", Lbufname, Bnames, sizeof(char *), Numbuffs);
	Nextpart = NULL;
	if (rc == -1)
		return;
	strcpy(Lbufname, was);
	cswitchto(cfindbuff(Bnames[rc]));
	uncomment(Curbuff);
}

void Znext_buffer(void)
{
	struct buff *next = Curbuff->prev;


	if (!next && Curbuff->next)
		for (next = Curbuff->next; next->next; next = next->next)
			;
	if (next) {
		strcpy(Lbufname, Curbuff->bname);
		cswitchto(next);
		reframe();
	} else
		tbell();
}

void Zdelete_buffer(void)
{
	struct buff *tbuff;
	char bname[BUFNAMMAX + 1];

	if (Argp) {
		strcpy(bname, Lbufname);
		do
			if (getarg("Buffer: ", bname, BUFNAMMAX))
				return;
		while ((tbuff = cfindbuff(bname)) == NULL);
		bswitchto(tbuff);
	}
	if (Curbuff->bmodf)
		switch (ask("save Changes? ")) {
		case ABORT:
			return;
		case YES:
			Zsave_file();
		}
	delbuff(Curbuff);
}


void delbuff(struct buff *buff)
{
	char bname[BUFNAMMAX + 1];
	int wascur;
	struct wdo *wdo;

	wascur = buff == Curbuff;
	strcpy(bname, buff->bname);	/* save it for delbname */
	if (strcmp(Lbufname, bname) == 0)
		*Lbufname = '\0';
	if (buff->fname) {
		free(buff->fname);
		buff->fname = NULL;
	}
	if (bdelbuff(buff)) {
		delbname(bname);
		if (wascur && *Lbufname) {
			struct buff *tbuff = cfindbuff(Lbufname);
			if (tbuff)
				bswitchto(tbuff);
		}
		cswitchto(Curbuff);

		/* make sure all windows pointed to deleted buff are updated */
		foreachwdo(wdo)
			if (wdo->wbuff == buff) {
				wdo->wbuff = Curbuff;
				bmrktopnt(wdo->wpnt);
				if (Curbuff->umark) {
					mrktomrk(wdo->wmrk, Curbuff->umark);
					wdo->umark_set = 1;
				} else
					wdo->umark_set = 0;
				bmrktopnt(wdo->wstart);
				wdo->modeflags = INVALID;
			}
	}
}

#define WASTED		(BUFNAMMAX + 14)

static void lstbuff(struct buff *tbuff)
{
	sprintf(PawStr, "%-*s %c%c %8lu %s ", BUFNAMMAX, tbuff->bname,
		(tbuff->bmode & SYSBUFF) ? 'S' : ' ',
		tbuff->bmodf ? '*' : ' ',
		blength(tbuff),
		tbuff->fname ? limit(tbuff->fname, WASTED) : UNTITLED);
	binstr(PawStr);
	binsert('\n');
}

void Zlist_buffers(void)
{
	struct wdo *was = Curwdo;
	int i;

	if (wuseother(LISTBUFF)) {
		for (i = 0; i < Numbuffs; ++i) {
			struct buff *tbuff = cfindbuff(Bnames[i]);
			if (tbuff)
				lstbuff(tbuff);
			else {
				sprintf(PawStr, "%-*s Problem\n",
					BUFNAMMAX, Bnames[i]);
				binstr(PawStr);
			}
		}
		Curbuff->bmodf = false;
		wswitchto(was);
	} else
		tbell();
	Arg = 0;
}

void Zunmodify(void)
{
	Curbuff->bmodf = Argp;
}

/* Add the new bname to the Bname array.
 * If we hit maxbuffs, try to enlarge the Bnames array.
 * Note that the compare MUST be insensitive for the getplete!
 */
static char *addbname(const char *bname)
{
	int i;

	if (Numbuffs == maxbuffs) {
		/* increase Bnames array */
		char **ptr = (char **)realloc(Bnames,
					      (maxbuffs + 10) * sizeof(char *));
		if (!ptr)
			return NULL;

		Bnames = ptr;
		maxbuffs += 10;
	}

	for (i = Numbuffs; i > 0 && strcasecmp(bname, Bnames[i - 1]) < 0; --i)
		Bnames[i] = Bnames[i - 1];
	Bnames[i] = strdup(bname);
	if (strlen(Bnames[i]) > BUFNAMMAX)
		Bnames[i][BUFNAMMAX] = '\0';
	++Numbuffs;

	return Bnames[i];
}

bool delbname(char *bname)
{
	int i;

	for (i = 0; i < Numbuffs && strcmp(bname, Bnames[i]); ++i)
		;
	if (i == Numbuffs)
		return false;

	--Numbuffs;
	free(Bnames[i]);
	Bnames[i] = NULL;

	if (Numbuffs == 0) {
		maxbuffs = 0;
		free(Bnames);
	} else
		for (; i < Numbuffs; ++i)
			Bnames[i] = Bnames[i + 1];

	return true;
}

/* Create a buffer. */
struct buff *cmakebuff(const char *bname, char *fname)
{
	struct buff *bptr, *save = Curbuff;

	bptr = cfindbuff(bname);
	if (bptr) {
		bswitchto(bptr);
		return bptr;
	}

	bptr = bcreate();
	if (!bptr) {
		error("Unable to create buffer");
		return NULL;
	}

	bptr->bname = addbname(bname);
	if (!bptr->bname) {
		error("Out of buffers");
		bdelbuff(bptr);
		bswitchto(save);
		return NULL;
	}

	if (*bname == '*')
		bptr->bmode |= SYSBUFF;

	bswitchto(bptr);
	if (fname)
		bptr->fname = strdup(fname);
	/* add the buffer to the head of the list */
	if (Bufflist)
		Bufflist->prev = bptr;
	bptr->next = Bufflist;
	Bufflist = bptr;

	return bptr;
}

/* Locate a given buffer */
struct buff *cfindbuff(const char *bname)
{
	struct buff *tbuff;

	for (tbuff = Bufflist; tbuff; tbuff = tbuff->next)
		if (strncasecmp(tbuff->bname, bname, BUFNAMMAX) == 0)
			return tbuff;
	return NULL;
}
