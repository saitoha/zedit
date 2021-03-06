/* srch.c - Zedit search functions
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


static int promptsearch(const char *prompt, int type);
static void promptreplace(int type);
static bool dosearch(void);
static bool replaceone(int, bool *, bool *, Byte *, bool);


bool Insearch;	/* set by nocase, reset by getarg */

static char olds[STRMAX + 1];	/* Search string */
static char news[STRMAX + 1];	/* Replace string */
static int searchdir[2];	/* Current direction for Again. */
static struct mark *Gmark;	/* used by global search routines */

#define QHELP	\
"Options: ' ' 'y'=change; 'n'=don't; '.'=change & quit; 'u'=undo; '^G'=quit"

static void doincsrch(const char *prompt, bool forward)
{
	bool go = true;
	char str[STRMAX + 1], promptstr[40];
	int cmd, i = 0, count = 0;
	struct mark marks[STRMAX];

	bmrktopnt(&marks[0]); /* make sure this is set */
	memset(str, '\0', sizeof(str));
	strcpy(promptstr, nocase(prompt));
	while (go) {
		zrefresh();
		putpaw("%s: %s", promptstr, str);
		cmd = tgetcmd();

		if (cmd == TC_UNKNOWN || cmd == TC_MOUSE) {
			tbell();
			continue;
		}

		if (cmd == 0x13) { /* CTRL-S */
			struct mark tmark;

			if (*str == '\0') {
				/* use last search */
				int n = strlen(olds);
				if (n == 0)
					continue;
				strcpy(str, olds);
				for (i = 0; i < n; ++i)
					bmrktopnt(&marks[i]);
			}

			bmrktopnt(&tmark); /* save in case search fails */
again:
			if (bstrsearch(str, forward))
				bmove(i);
			else if (++count == 2) {
				if (strstr(promptstr, "Wrap") == NULL)
					strcat(promptstr, " (Wrapped)");
				count = 0;
				btostart();
				goto again;
			} else {
				bpnttomrk(&tmark);
				tbell();
			}

			continue;
		}

		if (isprint(cmd)) {
			if (i < STRMAX) {
				bmrktopnt(&marks[i]);
				str[i++] = cmd;
				bmove(-i);
				if (bstrsearch(str, forward)) {
					bmove(i);
				} else {
					bpnttomrk(&marks[--i]);
					str[i] = '\0';
					tbell();
				}
			} else
				tbell();
		} else if (Keys[cmd] == ZDELETE_PREVIOUS_CHAR) {
			if (i > 0) {
				bpnttomrk(&marks[--i]);
				str[i] = '\0';
			} else
				tbell();
		} else if (Keys[cmd] != ZNOTIMPL) {
			if (cmd != CR)
				Cmdpushed = cmd;
			go = false;
		}
		count = 0;
	}
	clrpaw();
	if (Keys[cmd] == ZABORT)
		bpnttomrk(&marks[0]);
	else
		strcpy(olds, str);
	searchdir[0] = forward;
	searchdir[1] = 0;
}

void Zincremental_search(void)
{
	Arg = 0;
	if (Argp)
		doincsrch("Reverse I-search", BACKWARD);
	else
		doincsrch("I-search", FORWARD);
}

void Zsearch(void)
{
	promptsearch("Search: ", FORWARD);
}

void Zreverse_search(void)
{
	promptsearch("Reverse Search: ", BACKWARD);
}

void Zword_search(void)
{
	getbword(olds, sizeof(olds), bisword);

	searchdir[0] = FORWARD;
	searchdir[1] = 0;
	promptsearch(NULL, AGAIN);
}

void Zglobal_search(void)
{
	if (getarg(nocase("Global Search: "), olds, STRMAX))
		return;
	cswitchto(Bufflist);
	btostart();
	searchdir[0] = FORWARD;
	searchdir[1] = SGLOBAL;
	Zagain();
}

void Zre_search(void)
{
	promptsearch("RE Search: ", REGEXP);
}

void Zglobal_re_search(void)
{
	if (getarg(nocase("Global RE Search: "), olds, STRMAX))
		return;
	cswitchto(Bufflist);
	btostart();
	searchdir[0] = REGEXP;
	searchdir[1] = SGLOBAL;
	Zagain();
}

void Zagain(void)
{
	if (searchdir[1] == SGLOBAL) {
		if (!Gmark)
			Gmark = bcremrk(); /* set here in case exit/reload */
		while (!promptsearch("", AGAIN)) {
			Curbuff = Curbuff->next;
			if (Curbuff) {
				cswitchto(Curbuff);
				btostart();
				Arg = 1;
			} else {
				bpnttomrk(Gmark);
				unmark(Gmark);
				Gmark = NULL;
				Curwdo->modeflags = INVALID;
				searchdir[1] = 0;
				return;
			}
		}
	} else {
		if (searchdir[0] & AGAIN_WRAP) {
			searchdir[0] &= ~AGAIN_WRAP;
			btostart();
			putpaw("Wrapped....");
			dosearch();
		} else if (promptsearch("Search: ", AGAIN) == 0)
			searchdir[0] |= AGAIN_WRAP;
	}
}

void Zreplace(void)
{
	promptreplace(FORWARD);
}

void Zquery_replace(void)
{
	promptreplace(QUERY);
}

void Zre_replace(void)
{
	promptreplace(REGEXP);
}

static void doreplace(int type)
{
	bool exit = false, crgone, query;
	Byte ebuf[ESIZE];
	struct mark *pmark, tmark;
	struct buff *tbuff;
	int rc = 0;

	Arg = 0;

	query = type == FORWARD ? false : true;

	crgone = *olds && *(olds + strlen(olds) - 1) == '\n';
	pmark = bcremrk();

	if (type == REGEXP)
		rc = compile((Byte *)olds, ebuf, &ebuf[ESIZE]);
	if (rc)
		regerr(rc);
	else if (Argp) {
		for (tbuff = Bufflist; tbuff && !exit; tbuff = tbuff->next) {
			cswitchto(tbuff);
			bmrktopnt(&tmark);
			btostart();
			while (replaceone(type, &query, &exit, ebuf, crgone) &&
			       !exit)
				;
			bpnttomrk(&tmark);
		}
		clrpaw();
		cswitchto(pmark->mbuff);
	} else if (!replaceone(type, &query, &exit, ebuf, crgone) && !exit)
		putpaw("Not Found");
	else
		clrpaw();

	bpnttomrk(pmark);
	unmark(pmark);
}

static void promptreplace(int type)
{
	const char *prompt = NULL;
	int rc;

	switch (type) {
	case FORWARD:
		prompt = "Replace: ";		break;
	case QUERY:
		prompt = "QReplace: ";		break;
	case REGEXP:
		prompt = "RE Replace: ";	break;
	}
	if (getarg(nocase(prompt), olds, STRMAX))
		return;
	rc = getarg(nocase("with: "), news, STRMAX);
	if (rc == ABORT)
		return;
	if (rc)
		*news = '\0';

	doreplace(type);
}

static bool replaceone(int type, bool *query, bool *exit, Byte *ebuf,
			  bool crgone)
{
	bool found = false;
	char tchar = ',', *ptr;
	int dist, changeprev = 0;
	struct mark *prevmatch;

	prevmatch = bcremrk();
	putpaw("Searching...");
	while (!*exit &&
	       (type == REGEXP ? step(ebuf) : bstrsearch(olds, FORWARD))) {
		found = true;
		if (*query) {
replace:
			putpaw("Replace? ");
			zrefresh();
input:
			switch (tchar = tgetcmd()) {
			case ' ':
			case ',':	/* handled later */
			case 'Y':
			case 'y':
				break;	/* do the change */

			case '.':
				*exit = true; /* change, then abort */
				break;

			case '!':
				*query = false; /* global change */
				putpaw("Replacing...");
				break;

			case 'U': /* goto prev match */
			case 'u':
				if (type == REGEXP)
					tbell();
				else {
					bpnttomrk(prevmatch);
					if (changeprev) {
						bdelete(strlen(news));
						binstr(olds);
						bpnttomrk(prevmatch);
					}
				}
				goto replace;

			case '?':
				putpaw(QHELP);
				goto input;

			case 'S': /* skip file */
			case 's':
				unmark(prevmatch);
				return false;

			case 'q': /* abort */
			case 'Q':
				*exit = true;
				continue;

			default:
				if (Keys[(int)tchar] == ZABORT)
					*exit = true;
				else {
					bmrktopnt(prevmatch);
					changeprev = 0;
					/* skip and continue */
					bmove1();
				}
				continue;
			}
		}
		bmrktopnt(prevmatch);
		changeprev = 1;

		/* change it! */
		if (type == REGEXP) {
			/* force killtomrk to delete previous kill */
			Lfunc = ZNOTIMPL;
			killtomrk(REstart);
			for (ptr = news; *ptr; ++ptr)
				switch (*ptr) {
				case '\\':
					binsert(*(++ptr) ? *ptr : '\\'); break;
				case '&':
					Zyank(); break;
				default:
					binsert(*ptr);
				}
		} else {
			bdelete(strlen(olds));
			binstr(news);
		}
		if (*query && tchar == ',') {
			zrefresh();
			if (ask("Confirm? ") != YES) {
				/* change it back */
				if (type == REGEXP) {
					for (dist = 0;
					     !bisatmrk(REstart);
					     ++dist, bmove(-1))
						;
					bdelete(dist);
					Zyank();
				} else {
					int len = strlen(news);
					bmove(-len);
					bdelete(len);
					binstr(olds);
				}
			}
		}
		/* special case for "^" && "$" search strings */
		if (type == REGEXP && (ISNL(Buff()) || circf) && !crgone)
			bmove1();
		if (*query)
			putpaw("Searching...");
		else if (tkbrdy())
			*exit = true;
	}
	unmark(prevmatch);
	return found;
}

static int promptsearch(const char *prompt, int type)
{
	if (*olds == '\0' || type != AGAIN) {
		if (getarg(nocase(prompt), olds, STRMAX)) {
			Arg = 0;
			return ABORT;
		}
		searchdir[0] = type == AGAIN ? FORWARD : type;
		searchdir[1] = 0;
	}
	return dosearch();
}

static bool dosearch(void)
{
	int fcnt = 0, rc;
	struct mark save, fmark;

	bmrktopnt(&save);
#if 0
	/* This causes problems with RE search. You can't match the first line.
	 * Why is it needed?
	 */
	bmove(searchdir[0] == BACKWARD ? -1 : 1);
#endif
	if (searchdir[0] == REGEXP) {
		Byte ebuf[ESIZE];
		rc = compile((Byte *)olds, ebuf, &ebuf[ESIZE]);
		if (rc)
			regerr(rc);
		else
			while (Arg-- > 0)
				if (step(ebuf)) {
					bmrktopnt(&fmark);
					++fcnt;
				} else
					break;
	} else
		while (Arg-- > 0)
			if (bstrsearch(olds, searchdir[0])) {
				bmove(strlen(olds));
				bmrktopnt(&fmark);
				++fcnt;
			} else
				break;
	if (fcnt)
		bpnttomrk(&fmark);
	else {
		bpnttomrk(&save);
		putpaw("Not Found");
	}
	Arg = 0;
	return fcnt;
}

/* This is an implementation of the Boyer-Moore Search.
 * It uses the delta1 only with the fast/slow loops.
 * It searchs for the string 'str' starting at the current buffer location. If
 * forward is true, it searches forward, else it searches backwards.
 * The search will be case insensitive if the buffers current mode is so set.
 */
static bool bm_search(const char *str)
{
	int delta[NUMASCII], len, i, shift;
	bool exact = Curbuff->bmode & EXACT;

	len = strlen(str) - 1;

	/* Init the delta table to str length.  For each char in the
	 * str, store the offset from the str start in the delta
	 * table.  If we are in case insensitive mode - lower case the
	 * match string and mark both the upper case version and the
	 * lower case version of the match string chars in the delta
	 * array.
	 */
	for (i = 0; i < NUMASCII; ++i)
		delta[i] = len ? len : 1;
	if (exact)
		for (i = 0; i <= len;  ++i)
			delta[(int)str[i]] = len - i;
	else
		for (i = 0; i <= len;  ++i) {
			delta[toupper(str[i])] = len - i;
			delta[tolower(str[i])] = len - i;
		}

	/* search forward*/
	bmove(len);
	while (!bisend()) {
		/* fast loop - delta will be 0 if matched */
		while (!bisend() && delta[Buff()])
			bmove(delta[Buff()]);
		/* slow loop */
		for (i = len;
		     (char)Buff() == str[i] ||
			     (!exact && tolower(Buff()) == tolower(str[i]));
		     bmove(-1), --i)
			if (i == 0)
				return true;
		/* compute shift. shift must be forward! */
		if (i + delta[Buff()] > len)
			shift = delta[Buff()];
		else
			shift = len - i + 1;
		bmove(shift);
	}

	return false;
}

static bool bm_rsearch(const char *str)
{
	int delta[NUMASCII], len, i;
	bool exact = Curbuff->bmode & EXACT;

	len = strlen(str) - 1;

	/* Init the delta table to str length.  For each char in the
	 * str, store the negative offset from the str start in the
	 * delta table.
	 */
	for (i = 0; i < NUMASCII; ++i)
		delta[i] = len ? -len : -1;
	if (exact)
		for (i = len; i >= 0; --i)
			delta[(int)str[i]] = -i;
	else
		for (i = len; i >= 0; --i) {
			delta[toupper(str[i])] = -i;
			delta[tolower(str[i])] = -i;
		}

	/* reverse search */
	bmove(-len);
	while (!bisstart()) {
		/* fast loop - delta will be 0 if matched */
		while (delta[Buff()] && !bisstart())
			bmove(delta[Buff()]);
		/* slow loop */
		for (i = 0;
		     i <= len &&
			     ((char)Buff() == str[i] ||
			      (!exact &&
			       tolower(Buff()) == tolower(str[i])));
		     ++i, bmove1())
			;
		if (i > len) {
			/* we matched! */
			bmove(-len - 1);
			return true;
		}
		/* compute shift. shift must be backward! */
		bmove(delta[Buff()] + i < 0 ? delta[Buff()] : -i - 1);
	}

	return false;
}

bool bstrsearch(const char *str, bool forward)
{
	if (forward)
		return bm_search(str);
	else
		return bm_rsearch(str);
}

char *nocase(const char *prompt)
{
	static char is[20], upper[20];

	if (prompt) {
		const char *p;
		char *u;

		strcpy(is, prompt);
		for (p = prompt, u = upper; *p; ++p, ++u)
			*u = toupper(*p);
		Insearch = true;
	}
	return (Curbuff->bmode & EXACT) ? is : upper;
}
