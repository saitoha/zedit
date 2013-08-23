/* display.c - Zedit main display update
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
#include "assert.h"

static int innerdsp(int from, int to, struct mark *pmark);
static void pawdisplay(struct mark *, struct mark *);

struct mark *Sstart, *Psstart;	/* Screen start and 'prestart' */
struct mark *Send;		/* Screen end */
Boolean Sendp;			/* Screen end set */
struct mark Scrnmarks[ROWMAX + 1];	/* Screen marks - one per line */
int Tlrow;			/* Last row displayed */

static int NESTED;		/* refresh can go recursive... */
Byte tline[COLMAX + 1];

/* Mark screen invalid */
void redisplay(void)
{
	int i;

	tclrwind();
	for (i = 0; i < tmaxrow() - 2; ++i)
		Scrnmarks[i].modf = TRUE;
	Tlrow = -1;
}

/* Do the actual display update from the buffer */
void refresh(void)
{
	int pntrow, col, bcol;
	struct mark *pmark;
	struct wdo *wdo;
	int tsave;
	static struct mark *was;	/* last location of user mark */

	if (was == NULL)
		was = bcremrk();
	pmark = bcremrk();
	if (InPaw) {
		pawdisplay(pmark, was);
		return;
	}
	if (++NESTED > 10)
		hangup(0);

	setmodes(Curbuff);	/* SAM make sure OK */

	if (!mrkatmrk(was, Curbuff->mark)) {
		/* the user mark has moved! */
		vsetmrk(was);
		vsetmrk(Curbuff->mark);
		Tlrow = -1;
		mrktomrk(was, Curbuff->mark);
	}

	if (bisbeforemrk(Sstart) || (Sendp && !bisbeforemrk(Send)) ||
	   Sstart->mbuff != Curbuff)
		/* The cursor has moved before/after the screen marks */
		reframe();
	bpnttomrk(Sstart);
	if (bisatmrk(Psstart) && !bisstart()) {
		/* Deleted first char in window that is not at buffer start */
		bpnttomrk(pmark);
		reframe();
		bpnttomrk(Sstart);
	}
	pntrow = innerdsp(Curwdo->first, Curwdo->last, pmark);
	if (bisbeforemrk(pmark) && !tkbrdy()) {
		bpnttomrk(pmark);
		unmark(pmark);
		reframe();
		refresh();
		--NESTED;
		return;
	}

	/* update the other windows except Curwdo */
	tsave = Tabsize;
	for (wdo = Whead; wdo; wdo = wdo->next)
		if (wdo != Curwdo) {
			struct mark *point;
			bswitchto(wdo->wbuff);
			settabsize(Curbuff->bmode);
			point = bcremrk();
			bpnttomrk(wdo->wstart);
			innerdsp(wdo->first, wdo->last, NULL);
			modeflags(wdo);
			bpnttomrk(point);
			unmark(point);
			bswitchto(Curwdo->wbuff);
		}
	Tabsize = tsave;

	bpnttomrk(pmark);
	unmark(pmark);
	bcol = bgetcol(TRUE, 0);
	/* position the cursor */
	col = bcol % (tmaxcol() - 1 - Tstart);
	/* special case for NL or bisend at column 80 */
	if (col == 0 && bcol && (ISNL(Buff()) || bisend()))
		col = tmaxcol() - 1 - Tstart;
	else if (!bisend() && (col + chwidth(Buff(), col, FALSE) >= tmaxcol()))
		col = 0;
	tgoto(pntrow, col + Tstart);

	/*
	 * If we display the cursor on the mark, they both disappear.
	 * This code checks for this case: if true it removes the mark
	 * and invalidates its position so it will be updated when the
	 * cursor moves on...
	 */
	if (bisatmrk(Curbuff->mark)) {
		tstyle(T_NORMAL);
		tprntchar((bisend() || ISNL(Buff())) ? ' ' : Buff());
		tgoto(pntrow, col + Tstart);
		was->moffset = PSIZE + 1; /* Invalidate it */
	}

	modeflags(Curwdo);
	setmodes(Curbuff);	/* displaying other windows can blow modes */
	tflush();
	tstyle(T_NORMAL);

	--NESTED;
}

/* Test and clear modified flag on screen mark. */
static inline Boolean btstmrk(struct mark *tmark)
{
	Boolean temp = tmark->modf;
	tmark->modf  = FALSE;
	return temp;
}

static inline int buff_col(void)
{	/* Current column after current buffer char */
	return tgetcol() + twidth(Buff());
}

/*
 * Do the acutal screen update.
 * Curwdo is not valid.
 */
static int innerdsp(int from, int to, struct mark *pmark)
{
	int trow;
	 Byte *lptr;


	static int pntrow;
	int needpnt = TRUE, col;

	resetcomments();
	for (trow = from; trow < to; ++trow) {
#ifdef HSCROLL
		bmove(Hshift);
#endif
		if (btstmrk(&Scrnmarks[trow]) || !bisatmrk(&Scrnmarks[trow])) {
			bmrktopnt(&Scrnmarks[trow]); /* Do this before tkbrdy */
			lptr = tline;
			col = Tstart;
			tsetpoint(trow, col);
			while (!bisend() && !ISNL(Buff()) &&
			       (col = buff_col()) < tmaxcol()) {
				if (trow == Tlrow &&
				    Buff() == *lptr &&
				    Buff() != (Byte)'\376')
					tgetcol() = col;
				else {
					if (bisatmrk(Curbuff->mark))
						setmark(TRUE);
					else {
						checkcomment();
						tprntchar(Buff());
					}
					if (trow == Tlrow &&
					    (!ZISPRINT(*lptr) ||
					     !ZISPRINT(Buff())))
						Tlrow = -1;
				}
				*lptr++ = Buff();
				bmove1();
			}
			tcleol();
			if (bisatmrk(Curbuff->mark) &&
				(ISNL(Buff()) || bisstart() || bisend()))
					setmark(FALSE);
#ifdef HSCROLL
			if (!ISNL(Buff()))
				bcsearch(NL);
#else
			if (col >= tmaxcol())
				extendedlinemarker();
#endif
			memset(lptr, '\376', Colmax - (lptr - tline));
			Tlrow = trow;
			if (tgetcol() < tmaxcol()) {
				if (bisend())
					bshoveit();
				else if (ISNL(Buff()))
					bmove1();
			}
		} else
			bpnttomrk(&Scrnmarks[trow + 1]);
		if (pmark && bisaftermrk(pmark) && needpnt) {
			pntrow = trow;
			needpnt = FALSE;
		}
	}
	bmrktopnt(&Scrnmarks[trow]);
	if (pmark) {
		bmrktopnt(Send);
		Sendp = TRUE;
		if (needpnt) {
			/* the user has typed past the end of the screen */
			reframe();
			refresh();
		}
	}

#if COMMENTBOLD
	tstyle(T_NORMAL);
#endif

	return pntrow;
}

/* Work for centering redisplay */
void reframe(void)
{
	int cnt;
	struct mark *pmark;

	pmark = bcremrk();
	for (cnt = prefline(); cnt > 0 && bcrsearch(NL); --cnt)
			cnt -= bgetcol(TRUE, 0) / tmaxcol();
	if (cnt < 0)
		bmakecol((-cnt) * tmaxcol(), FALSE);
	else
		tobegline();
	bmrktopnt(Sstart);
	bmove(-1);
	bmrktopnt(Psstart);
	Sendp = FALSE;
	bpnttomrk(pmark);
	unmark(pmark);
}

/* Set one windows modified flags. */
static void subset(int from, int to, int flag)
{
	struct mark *btmark, *ltmark;

	if (Scrnmarks[from].mbuff != Curbuff)
		return;
	for (btmark = &Scrnmarks[from], ltmark = &Scrnmarks[to];
		 btmark <= ltmark && btmark->mpage != Curpage;
		 ++btmark)
		;
	if (btmark > ltmark) {
		for (btmark = &Scrnmarks[from];
		     btmark <= ltmark &&
			     (btmark->mbuff != Curbuff || bisaftermrk(btmark));
		     ++btmark)
			;
		if (btmark > &Scrnmarks[from]) {
			while ((--btmark)->mbuff != Curbuff)
				;
			btmark->modf = TRUE;
		}
	} else {
		while (btmark->mpage == Curpage && btmark->moffset <= Curchar &&
			   btmark <= ltmark)
			++btmark;
		if (--btmark >= &Scrnmarks[from])
			btmark->modf = TRUE;
		if (flag)
			while (btmark > &Scrnmarks[from] &&
			       btmark->mpage == Curpage &&
			       btmark->moffset == Curchar)
				(--btmark)->modf = TRUE;
	}
}

/* Insert the correct modified flags. */
void vsetmod(int flag)
{
	struct wdo *wdo;

	for (wdo = Whead; wdo; wdo = wdo->next)
		if (wdo->wbuff == Curbuff)
			subset(wdo->first, wdo->last, flag);
}

void vsetmrk(struct mark *mrk)
{
	int row;

	for (row = 0; row < tmaxrow() - 1; ++row)
		if (mrkaftermrk(&Scrnmarks[row], mrk)) {
			if (row > 0)
				Scrnmarks[row - 1].modf = TRUE;
			return;
		}
}

#define SHIFT	(Colmax / 4 + 1)

static void pawdisplay(struct mark *pmark, struct mark *was)
{
	int bcol = 0, i, nested = 0;
	Boolean mrkmoved = !mrkatmrk(was, Curbuff->mark);

	Prow = Rowmax - 1;
pawshift:
	btostart(); bmove(Pshift);
	for (i = 0, Pcol = Pawcol;
	     Pcol < Colmax - 2 && !bisend();
	     bmove1(), ++i) {
		if (bisatmrk(pmark))
			bcol = Pcol;
		if (mrkmoved && (bisatmrk(Curbuff->mark) || bisatmrk(was))) {
			if (bisatmrk(Curbuff->mark))
				tstyle(T_REVERSE);
			tprntchar(Buff());
			tstyle(T_NORMAL);
			tline[i] = Buff();
		} else if (tline[i] == Buff())
			Pcol += chwidth(Buff(), 0, 0);
		else {
			tline[i] = Buff();
			tprntchar(Buff());
		}
	}
	memset(&tline[i], '\376', &tline[COLMAX] - &tline[i]);
	tcleol();

	if (bisend()) {
		if (bisatmrk(Curbuff->mark)) {
			setmark(FALSE);
			--Pcol;		/* space always 1 character! */
		} else if (bisatmrk(pmark))
			bcol = Pcol;
	}

	if (!bcol) {
		if (Pshift) {
			/* shift right */
			Pshift -= SHIFT;
			if (Pshift < 0)
				Pshift = 0;
			if (++nested == 1)
				goto pawshift;
			else
				Dbg("Shift right nested too deep!\n");
		} else if (Pcol >= Colmax - 2) {
			/* shift left */
			Pshift += SHIFT;
			if (++nested == 1)
				goto pawshift;
			else
				Dbg("Shift left nested too deep!\n");
		}
	}

	if (bcol)
		Pcol = bcol;
	bpnttomrk(pmark);
	mrktomrk(was, Curbuff->mark);

	/*
	 * If we display the cursor on the mark, they both disappear.
	 * This code checks for this case: if true it removes the mark
	 * and invalidates its position so it will be updated when the
	 * cursor moves on...
	 */
	if (bisatmrk(Curbuff->mark)) {
		i = Pcol;
		tprntchar(bisend() ? ' ' : Buff());
		Pcol = i;
		was->moffset = PSIZE + 1;		/* Invalidate it */
	}

	unmark(pmark);
	--NESTED;
	tforce();
	tflush();
}

void initscrnmarks(void)
{
	int cnt;

	/* Set the screen marks */
	memset((char *)Scrnmarks, 0, sizeof(Scrnmarks));
	Scrnmarks[0].next = &Scrnmarks[1];
	for (cnt = 1; cnt < ROWMAX; ++cnt) {
		Scrnmarks[cnt].prev  = &Scrnmarks[cnt - 1];
		Scrnmarks[cnt].next  = &Scrnmarks[cnt + 1];
	}
	Scrnmarks[ROWMAX - 1].next = NULL;

	/* init the Mrklist */
	Mrklist = &Scrnmarks[ROWMAX - 1];
}
