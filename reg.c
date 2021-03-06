#include "z.h"

/* Regular expression compile and match routines.
 * See regexp(5) and ed(1).
 * NOTES:	loc1 is now REstart
 *		loc2 was removed, buffer will be left pointing here!
 *		locs is set externally by ed and sed - removed!
 */

static bool advance(Byte *);
static bool ecmp(struct mark *, int);
static void getrnge(Byte *);

#define	CBRA	2
#define	CCHR	4
#define	CDOT	8
#define	CCL	12
#define	CDOL	20
#define	CCEOF	22
#define	CKET	24
#define	CBACK	36

#define	STAR	01
#define RNGE	03

#define	NBRA	9

#define PLACE(c)	(ep[c >> 3] |= bittab[c & 07])
#define ISTHERE(c)	(ep[c >> 3] & bittab[c & 07])
#define GETC()		(*sp++)
#define PEEKC()		(*sp)
#define UNGETC(c)	(--sp)

struct mark *REstart;		/* assigned in Setup */
static struct mark braslist[NBRA];
static struct mark braelist[NBRA];
static int ebra, nbra;

int	circf;
static int low;
static int size;

static Byte bittab[] = { 1, 2, 4, 8, 16, 32, 64, 128 };


/* Step thru the buffer trying to match the compiled expression.
 * Returns true if matched, else false.
 * The point is left at the end of the matched string or the buffer end and
 * REstart points to the start of the match.
 */
bool step(Byte *ep)
{
	/* ^ must match from start */
	if (circf)
		/* if not at the start of the current line - go to the
		 * next line */
		if (bpeek() != NL)
			bcsearch(NL);	/* goto next line */

	/* regular algorithm */
	while (!bisend()) {
		bmrktopnt(REstart);
		if (advance(ep))
			return true;
		if (circf)
			bcsearch(NL);	/* goto next line */
		else {
			bpnttomrk(REstart);
			bmove1();
		}
	}
	return false;
}

/* Called by step to try to match at current position. */
static bool advance(Byte *ep)
{
	int c, ct;
	struct mark *bbeg, curlp, tmark;

	while (!bisend()) {
		switch (*ep++) {
		case CCHR:
			if (*ep++ == Buff())
				break;
			return false;

		case CDOT:
			if (ISNL(Buff()))
				return false;
			break;

		case CDOL:
			if (ISNL(Buff()))
				continue;
			return false;

		case CCEOF:
			return true;

		case CCL:
			if (ISTHERE(((Byte)Buff()))) {
				ep += 16;
				break;
			}
			return false;

		case CBRA:
			bmrktopnt(&braslist[*ep++]);
			continue;

		case CKET:
			bmrktopnt(&braelist[*ep++]);
			continue;

		case CCHR | RNGE:
			c = Buff();
			bmove1();
			getrnge(ep);
			while (low--)
				if (Buff() != c)
					return false;
				else
					bmove1();
			bmrktopnt(&curlp);
			while (size-- && Buff() == c)
				bmove1();
			if (size < 0 || Buff() != c)
				bmove1();
			ep += 2;
			goto star;

		case CDOT | RNGE:
			getrnge(ep);
			while (low--)
				if (ISNL(Buff()))
					return false;
				else
					bmove1();
			bmrktopnt(&curlp);
			while (size-- && !ISNL(Buff()))
				bmove1();
			if (size < 0 || ISNL(Buff()))
				bmove1();
			ep += 2;
			goto star;

		case CCL | RNGE:
			getrnge(ep + 16);
			while (low--) {
				c = Buff() & 0177;
				bmove1();
				if (!ISTHERE(c))
					return false;
			}
			bmrktopnt(&curlp);
			while (size--) {
				c = Buff() & 0177;
				bmove1();
				if (!ISTHERE(c))
					break;
			}
			if (size < 0)
				bmove1();
			ep += 18;		/* 16 + 2 */
			goto star;

		case CBACK:
			bbeg = &braslist[*ep]; /* start of prev match */
			ct = &braelist[*ep++] - bbeg; /* length */
			if (ecmp(bbeg, ct)) /* same?? */
				continue;
			return false;

		case CBACK | STAR:
			bbeg = &braslist[*ep];
			ct = &braelist[*ep++] - bbeg;
			bmrktopnt(&curlp);
			while (ecmp(bbeg, ct))
				;
			while (bisaftermrk(&curlp) || bisatmrk(&curlp)) {
				if (advance(ep))
					return true;
				bmove(-ct);
			}
			return false;

		case CDOT | STAR:
			bmrktopnt(&curlp); /* save the current position */
			toendline();
			goto star;

		case CCHR | STAR:
			bmrktopnt(&curlp); /* save the current position */
			while (Buff() == *ep)  /* skip over matches */
				bmove1();
			ep++;                       /* go on */
			goto star;

		case CCL | STAR:
			bmrktopnt(&curlp);
			while (!bisend() && ISTHERE(((Byte)Buff())))
				bmove1();
			ep += 16;

star:
			do {
				bmrktopnt(&tmark);
				if (advance(ep)) /* try to match */
					return true;
				bpnttomrk(&tmark);
				bmove(-1); /* go back and try again */
			} while (!bisbeforemrk(&curlp)); /* till back to start */
			bpnttomrk(&curlp); /* Don't slip backwards */
			return false;
		}
		bmove1();
	}
	return *ep == CCEOF;
}

/* Called by advance to match [] type ranges. */
static void getrnge(Byte *str)
{
	low = *str++ & 0377;
	size = (*str == (Byte)'\377') ? 20000 : (*str & 0377) - low;
}

/* Compare the string at buffer mrk start to the string at the current point.
 * Match cnt chars.
 * Returns true if matched.
 * Moves the buffer point and start mrk.
 */
static bool ecmp(struct mark *start, int cnt)
{
	Byte c;

	while (cnt-- > 0 && !bisend()) {
		bswappnt(start);
		c = Buff();
		bmove1();
		bswappnt(start);
		if (Buff() != c)
			return false;
		bmove1();
	}
	return true;
}

/* Compile the match string.
 * Returns 0 if ok, else errnum.
 */
#define EOFCH	('\0')

int compile(Byte *instring, Byte *ep, Byte *endbuf)
{
	Byte *sp = instring;
	int c;
	Byte *lastep = instring;
	int cclcnt;
	char bracket[NBRA], *bracketp;
	int closed;
	int neg;
	int lc;
	int i, cflg;

	lastep = NULL;
	c = GETC();
#ifdef ALLOW_NL
	if (c == EOFCH) {
		if (*ep == NULL)
			return 41;
		return 0;
	}
#else
	if (c == EOFCH || c == '\n') {
		if (c == '\n')
			UNGETC(c);
		if (*ep == 0)
			return 41;
		return 0;
	}
#endif
	bracketp = bracket;
	circf = closed = nbra = ebra = 0;
	if (c == '^')
		circf++;
	else
		UNGETC(c);
	while (1) {
		if (ep >= endbuf)
			return 50;
		c = GETC();
		if (c != '*' && ((c != '\\') || (PEEKC() != '{')))
			lastep = ep;
		if (c == EOFCH) {
			*ep++ = CCEOF;
			return 0;
		}
		switch (c) {

		case '.':
			*ep++ = CDOT;
			continue;

#ifndef ALLOW_NL
		case '\n':
			UNGETC(c);
			*ep++ = CCEOF;
			return 0;
#endif
		case '*':
			if (!lastep || *lastep == CBRA || *lastep == CKET)
				goto defchar;
			*lastep |= STAR;
			continue;

		case '$':
			if (PEEKC() != EOFCH && PEEKC() != '\n')
				goto defchar;
			*ep++ = CDOL;
			continue;

		case '[':
			if (&ep[17] >= endbuf)
				return 50;

			*ep++ = CCL;
			lc = 0;
			for (i = 0; i < 16; i++)
				ep[i] = 0;

			neg = 0;
			c = GETC();
			if (c == '^') {
				neg = 1;
				c = GETC();
			}

			do {
				if (c == '\0' || c == '\n')
					return 49;
				if (c == '-' && lc != 0) {
					c = GETC();
					if (c == ']') {
						PLACE('-');
						break;
					}
					while (lc < c) {
						PLACE(lc);
						lc++;
					}
				}
				lc = c;
				PLACE(c);
			} while ((c = GETC()) != ']');
			if (neg) {
				for (cclcnt = 0; cclcnt < 16; cclcnt++)
					ep[cclcnt] ^= -1;
				ep[0] &= 0376;		/* ignore NULL */
				ep[1] &= 0373;		/* ignore NL */
			}

			ep += 16;

			continue;

		case '\\':
			switch (c = GETC()) {

			case '(':
				if (nbra >= NBRA)
					return 43;
				*bracketp++ = nbra;
				*ep++ = CBRA;
				*ep++ = nbra++;
				continue;

			case ')':
				if (bracketp <= bracket || ++ebra != nbra)
					return 42;
				*ep++ = CKET;
				*ep++ = *--bracketp;
				closed++;
				continue;

			case '{':
				if (lastep == NULL)
					goto defchar;
				*lastep |= RNGE;
				cflg = 0;
nlim:
				c = GETC();
				i = 0;
				do {
					if ('0' <= c && c <= '9')
						i = 10 * i + c - '0';
					else
						return 48;
				} while (((c = GETC()) != '\\') && (c != ','));
				if (i > 255)
					return 47;
				*ep++ = i;
				if (c == ',') {
					if (cflg++)
						return 44;
					c = GETC();
					if (c == '\\')
						*ep++ = 255;
					else {
						UNGETC(c);
						goto nlim;
						/* get 2'nd number */
					}
				}
				if (GETC() != '}')
					return 45;
				if (!cflg)	/* one number */
					*ep++ = i;
				else if ((ep[-1] & 0377) < (ep[-2] & 0377))
					return 46;
				continue;

			case '\n':
				return 40;

			case 'n':
				c = '\n';
				goto defchar;

			default:
				if (c >= '1' && c <= '9') {
					if (--c  >= closed)
						return 51;
					*ep++ = CBACK;
					*ep++ = c;
					continue;
				}
			}
	/* Drop through to default to use \ to turn off special chars */

defchar:
		default:
			lastep = ep;
			*ep++ = CCHR;
			*ep++ = c;
		}
	}
}

void regerr(int errnum)
{
	static const char * const errs[] = {
		/*40*/	"Illegal or missing delimiter.",
		/*41*/	"No search string.",
		/*42*/	"() imbalance.",
		/*43*/	"Too many (.",
		/*44*/	"More than two numbers given in {}.",
		/*45*/	"} expected after .",
		/*46*/	"First number exceeds second in {}.",
		/*47*/	"Range endpoint too large.",
		/*48*/	"Bad number.",
		/*49*/	"[] imbalance.",
		/*50*/	"Regular expression overflow.",
		/*51*/	"\"digit\" out of range.",
	};

	error(errs[errnum - 40]);
}
