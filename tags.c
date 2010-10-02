/* tags.c - Zedit tag file commands
 * Copyright (C) 1988-2010 Sean MacLennan
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

#ifdef TAGS
#include <sys/stat.h>

char savetag[STRMAX + 1];
static struct buff *Bsave;

static void gotomatch(struct mark *smark);
static Boolean tagfparse(struct buff *);
static Boolean gettagsfile(void);

/* Routines to handle tag files. Zfindtag looks through the tagfile and if
 * the tag is found, goes to the appropriate file and position.
 * If a file called "TAGS" exists in the current directory, it is used, else
 * the file names in the tagfile variable is used.
 * The tag routines can handle the following types of tag files:
 *	<tag> <num>   <fname>	       eg. ctags -x
 *	<tag> <fname> <num>	       eg. calltree
 *	<tag> <fname> <match_ch>[^]<search_string>[$]<match_ch>	eg. ctags
 * A space can be any number of spaces and/or tabs.
 * If a number <num> has a leading 0 it is assumed to be a byte offset, else a
 * line offset. In the ctags format, the ^ and $ are optional and are stripped
 * out if at the begining and end of line respectively.
 */
void Zfindtag(void)
{
	char tag[STRMAX + 1], word[PATHMAX + 1];
	Boolean best, found;
	struct mark tmark, smark;

	/* do BEFORE switching buffer! */
	Arg = 0;
	getbword(tag, STRMAX, bistoken);
	Bsave = Curbuff;
	bmrktopnt(&smark);

	if (!gettagsfile())
		return;
	Argp = 0;

	do {
		best = found = FALSE;
		if (getarg("Tag: ", tag, STRMAX) == 0) {
			Echo("Looking...");
			for (btostart(); !bisend(); bcsearch(NL)) {
				getbword(word, STRMAX, bistoken);
				if (strcasecmp(tag, word) == 0) {
					if (strcmp(tag, word) == 0) {
						gotomatch(&smark);
						return;
					} else if (!best) {
						best  = TRUE;
						found = TRUE;
						bmrktopnt(&tmark);
					} else if (!found &&
						   stristr(word, tag)) {
						found = TRUE;
						bmrktopnt(&tmark);
					}
				}
			}

			if (best) {
				bpnttomrk(&tmark);
				gotomatch(&smark);
				return;
			}

			if (found) {
				strcpy(savetag, tag);
				Nextpart = ZFINDTAG;
				bpnttomrk(&tmark);
				getbword(tag, STRMAX, bistoken);
			} else
				Echo("Not Found");
		}
	} while (found);
	Nextpart = ZNOTIMPL;
	bswitchto(Bsave);			/* go back to original buffer */
	Curwdo->modeflags = INVALID;
}

#ifdef XWINDOWS
void xfindtag(void)
{
	char tag[STRMAX + 1], word[PATHMAX + 1];
	struct mark smark;

	/* do BEFORE switching buffer! */
	Arg = 0;
	getbword(tag, STRMAX, bistoken);
	Bsave = Curbuff;
	bmrktopnt(&smark);

	if (!gettagsfile())
		return;

	Echo("Looking...");
	for (btostart(); !bisend(); bcsearch(NL)) {
		getbword(word, STRMAX, bistoken);
		if (strcmp(tag, word) == 0) {
			/* found a match in the tag file */
			gotomatch(&smark);
			refresh();
			return;
		}
	}
	Echo("Not found");
	tbell();
	bswitchto(Bsave);			/* go back to original buffer */
	Curwdo->modeflags = INVALID;
}
#endif

static void gotomatch(struct mark *smark)
{
	struct mark tmark;

	if (tagfparse(Bsave))
		if (strcmp(Bsave->bname, Curbuff->bname))
			strcpy(Lbufname, Bsave->bname);
	bmrktopnt(&tmark);
	bpnttomrk(smark);
	Zsetbookmrk();
	bpnttomrk(&tmark);
	Nextpart = ZNOTIMPL;
}

/* Parse the line in the tag file and find the correct file and position. */
static Boolean tagfparse(struct buff *bsave)
{
#if ETAGS
#else
	Boolean byte = 0, smatch = 0, ematch = 0, found;	/*shutup*/
	Byte mch;
	struct mark tmark;
	char fname[PATHMAX + 1], path[PATHMAX + 1], str[STRMAX], *ptr;
	int i;
	long num = -1;

	num = -1;
	*str = '\0';

	while (!biswhite() && !bisend())
		bmove1(); /* skip partial match */
	while (biswhite())
		bmove1();
	if (isdigit(Buff())) {
		byte = Buff() == '0';
		num = batoi();
	}

	while (biswhite())
		bmove1();
	for (i = 0; i < PATHMAX && !isspace(Buff()) && !bisend(); bmove1())
		fname[i++] = Buff();
	fname[i] = '\0';

	while (biswhite())
		bmove1();
	if (num == -1) {
		if (isdigit(Buff())) {
			byte = Buff() == '0';
			num = batoi();
		} else {
			mch = Buff();
			bmove1();
			smatch = Buff();
			if (smatch == '^')
				bmove1();
			for (ptr = str;
				 Buff() != mch && Buff() != NL && !bisend();
				 *ptr = Buff(), bmove1(), ++ptr)
				if (Buff() == '\\')
					bmove1();	/* escapes */
			ematch = *(ptr - 1);
			if (ematch == '$')
				--ptr;
			*ptr = '\0';
		}
	}

	if (i && (num != -1 || *str)) {
		bswitchto(bsave);		/* restore correct buffer */
		pathfixup(path, fname);
		findfile(path, FALSE);
		btostart();

		if (num != -1)
			if (byte)
				boffset(num);
			else
				while (--num > 0 && bcsearch(NL))
					;
		else
			for (found = FALSE; bstrsearch(str, FORWARD);) {
				found = TRUE;
				bmrktopnt(&tmark);
				if (smatch) {
					bmove(-1);
					found = (bisstart() || Buff() == NL);
					bpnttomrk(&tmark);
				}
				if (found && ematch) {
					bmove(strlen(str));
					found = (bisend() || Buff() == NL);
					bpnttomrk(&tmark);
				}
				if (found)
					return TRUE;
				if (smatch)
					bcsearch(NL);
				else
					bmove1();
			}
		return TRUE;
	}
	Error("Bad Tag File");
	return FALSE;
#endif
}


/* Load the tags file into a buffer. */
static Boolean gettagsfile(void)
{
	struct buff *tbuff;
	char fname[PATHMAX + 1], *tagfname;

	tbuff = Cfindbuff(TAGBUFNAME);
	if (tbuff) {
		struct stat sb;

		bswitchto(tbuff);
		if (Argp) {
			/* ask user for file to use. */
			strcpy(fname, tbuff->fname);
			if (getfname("Tag File: ", fname))
				return FALSE;

			breadfile(fname);
			if (Curbuff->fname)
				free(Curbuff->fname);
			Curbuff->fname = strdup(fname);
		} else if (stat(tbuff->fname, &sb) == 0 &&
			   sb.st_mtime != tbuff->mtime) {
			/* tags file has been updated */
			Echo("Reloading tags file.");
			breadfile(tbuff->fname);
		}
		return  TRUE;
	}

	/*  No tags buffer - try to create and read in the file.
	 *	First check for "tags" in the current directoy. If it
	 *	dosen't exist, try the the variable TAGFILE.
	 */
	if (Argp) {
		/* ask user for file to use. */
		strcpy(fname, "TAGS");
		if (getfname("Tag File: ", fname))
			return FALSE;
		if (access(fname, 0)) {
			sprintf(PawStr, "%s not found.", fname);
			Error(PawStr);
			return FALSE;
		}
	} else {
		if (access("tags", 0) == 0)
			tagfname = "tags";
		else if (access("TAGS", 0) == 0)
			tagfname = "TAGS";
		else {
			tagfname = (char *)VAR(VTAG);
			if (!tagfname  || access(tagfname, 0)) {
				Error("No tags file found.");
				return FALSE;
			}
		}
		if (pathfixup(fname, tagfname))
			return FALSE;
	}

	tbuff = Cmakebuff(TAGBUFNAME, fname);
	if (!tbuff) {
		Error("Can't create tag buffer.");
		return FALSE;
	}

	breadfile(fname);
	return TRUE;
}

/* Convert the next portion of buffer to integer. Skip leading ws. */
int batoi(void)
{
	int num;

	while (biswhite())
		bmove1();
	for (num = 0; isdigit(Buff()); bmove1())
		num = num * 10 + Buff() - '0';
	return num;
}


void Zref(void)
{
	struct buff *mbuff;
	char tag[STRMAX + 40], *p;

	strcpy(tag, "ref ");
	p = tag + strlen(tag);
	getbword(p, STRMAX, bistoken);
	if (getarg("Ref tag: ", p, STRMAX))
		return;

	mbuff = cmdtobuff(REFBUFF, tag);
	if (mbuff == NULL)
		Error("Unable to execute ref.");
	else
		message(mbuff, tag);
}

#else
void Zfindtag(void) { tbell(); }
void Zref(void) { tbell(); }
#endif /* TAGS */
