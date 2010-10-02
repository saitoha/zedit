/* spell.c - spell command
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

#ifdef SPELL
#include <signal.h>

#define SPELLSTRING "<space>(skip)  A(ccept)  I(nsert)  R(eplace)  #"
#define PROMPT		"Replace with: "

static void Mclear(void);
static int ispell(FILE *, FILE *, char *, char *);

void Zspell(void)
{
	static char *argv[] = { "ispell", "-a", NULL };
	struct buff *was;
	struct buff *sbuff;
	struct mark *emark;
	struct mark *point, *mark;
	Byte cmd;
	char send[STRMAX + 2], buff[1024], *p, *e;
	char word[6][STRMAX + 1];
	int m, n, rc;
	FILE *in, *out;

	/* save current Buff, Point, and Mark */
	was = Curbuff;
	point = bcremrk();
	mark = bcremrk();
	Mrktomrk(mark, Curbuff->mark);

	/* set Point and emark */
	if (Argp) {
		/* use Region */
		emark = bcremrk();
		Mrktomrk(emark, Curbuff->mark);
		if (bisaftermrk(emark))
			bswappnt(emark);
	} else {
		/* use entire buffer */
		btoend();
		bmove(-1);
		emark = bcremrk();
		btostart();
	}

	sbuff = Cmakebuff(SPELLBUFF, NULL);
	if (!sbuff || !Invoke(sbuff, argv))
		return;

	in = fdopen(sbuff->in_pipe, "r");
	out = sbuff->out_pipe;

	/* make the paw 3 lines */
	bswitchto(was);
	Resize(-2);
	Refresh();
	Mclear();
	Echo("Checking...");
	while (bisbeforemrk(emark)) {
		/* get the next word */
		Moveto(Isalpha, FORWARD);
		if (Bisend())
			break;	/* incase no alphas left */
		bmrktopnt(Curbuff->mark);
		for (p = send + 1; Isalpha() && !Bisend(); ++p, bmove1())
			*p = Buff();
		*p++ = '\n'; *p = '\0';

		Mclear();

		/* process the word */
		n = ispell(in, out, send + 1, buff);
		if (n > 0) {
			m = -1;
			switch (*buff) {
			case '*':		/* ok */
			case '+':		/* ok with suffix removed */
				break;

			case '&':		/* close matches */
				Pset(Tmaxrow(), 0, PNUMCOLS);
				for (m = 0, p = buff + 2; m < 6; ++m) {
					e = strchr(p, ' ');
					if (!e)
						e = strchr(p, '\n');
					if (!e || e == p)
						break;
					*e++ = '\0';
					word[m][0] = m + '0';
					word[m][1] = ' ';
					strcpy(&word[m][2], p);
					Pout(word[m], FALSE);
					p = e;
				}
				/* 'm' is now the number of matches -
				 * drop thru */

			case '#':		/* no match */
				Echo(SPELLSTRING);
				Refresh();		/* update mark */
				cmd = Tgetcmd();
				switch (cmd) {
				case ' ':		/* skip it */
					break;

				case 'A':		/* accept */
				case 'a':
					*send = '@';
					ispell(in, out, send, buff);
					break;

				case 'I':	/* insert in dictionary */
				case 'i':
					*send = '*';
					ispell(in, out, send, buff);
					break;

				case 'R':		/* replace */
				case 'r':
					*word[0] = '\0';
					rc = Getarg(PROMPT, word[0], STRMAX);
					if (rc == ABORT) {
						bswappnt(Curbuff->mark);
						continue;
					}
					sreplace(word[0]);
					break;

				default:
					if (isdigit(cmd)) {
						n = cmd - '0';
						if (n < m)
							sreplace(word[n] + 2);
					} else {
						Tbell();
						if (Keys[cmd] == ZABORT)
							goto abort;
					}
					break;
				}
				break;

			default:		/* invalid */
				Error("Unable to start ispell");
				goto abort;
			}
		}
	}
abort:
	fclose(in);		/* from fdopen */
	Delbuff(sbuff);
	bswitchto(was);
	bpnttomrk(point);
	Mrktomrk(Curbuff->mark, mark);
	unmark(point);
	unmark(mark);
	unmark(emark);
	Resize(2);
}


static void Mclear(void)
{
	/* clear the matches area */
	Clrcol[Rowmax] = Clrcol[Rowmax + 1] = COLMAX + 1;
	Tsetpoint(Rowmax, 0);
	Tcleol();
	Tsetpoint(Rowmax + 1, 0);
	Tcleol();
}

static int ispell(FILE *in, FILE *out, char *send, char *receive)
{
	fputs(send, out);
	fflush(out);
	return fgets(receive, 1024, in) ? strlen(receive) : 0;
}

/* replace with new, assumes Region contains old word */
void sreplace(char *new)
{
	bdeltomrk(Curbuff->mark);
	binstr(new);
}

int Isalpha(void)
{
	return isalpha(Buff());
}
#else
void Zspell(void) { Tbell(); }
#endif
