/****************************************************************************
 *																			*
 *				 The software found in this file is the						*
 *					  Copyright of Sean MacLennan							*
 *						  All rights reserved.								*
 *																			*
 ****************************************************************************/
#include "z.h"
#include "help.h"

/* the following include file contains the definition of the Cnames array */
#include "cnames.h"

Proc (*Cmds[])() =
{
	Z1wind,
	Z2wind,
	Zabort,
	Zagain,
	Zarg,
	Zbegline,
	Zbpara,
	Zbword,
	Zcalc,
	Zcapword,
	Zcase,
	Zcenter,
	Zcopyrgn,
	Zctrlx,
	Zdelchar,
	Zdeleol,
	Zdelline,
	Zdelrgn,
	Zdelwhite,
	Zdelword,
	Zendline,
	Zexit,
	Zfileread,
	Zfilesave,
	Zfilewrite,
	Zmode,
	Zfillpara,
	Zfindfile,
	Zfindtag,
	Zfpara,
	Zfword,
	Zgetbword,
	Zgrowwind,
	Zhexout,
	Zinsert,
	Zkillbuff,
	Zlgoto,
	Zlowword,
	Zlstbuff,
	Zmakedel,
	Zmeta,
	Znextchar,
	Znextline,
	Znextpage,
	Znotimpl,
	Znxtbookmrk,
	Znxtothrwind,
	Zquit,
	Zopenline,
	Zoverin,
	Zprevchar,
	Zprevline,
	Zprevothrwind,
	Zprevpage,
	Zprintpos,
	Zquery,
	Zquote,
	Zrdelchar,
	Zrdelword,
	Zrereplace,
	Zresrch,
	Zrsearch,
	Zsearch,
	Zsetavar,
	Zsetbookmrk,
	Zsetmrk,
	Zshell,
	Zstat,
	Zswapchar,
	Zswapmrk,
	Znextwind,
	Zswapword,
	Zswitchto,
	Ztab,
	Ztoend,
	Ztostart,
	Zupword,
	Zyank,
	Zcindent,
	Zcinsert,
	Zfillchk,
	Znewline,
	Zredisplay,
	Zmetax,
	Zbind,
	Zsavebind,
	Zdispbinds,
	Znextbuff,
	Zkeybind,
	Zcount,
	Zincsrch,
	Zrincsrch,
	Zunmodf,
	Zispace,
	Zprint,
	Zdelblanks,
	Zmrkpara,
	Zupregion,
	Zlowregion,
	Zviewline,
	Zstartmacro,
	Zendmacro,
	Zdomacro,
	Znamemacro,
	Zmacron, /* 104 */
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron, /* 119 */
	Zreadmacro,
	Zwritemacro,
	Zcmd,
	Zjoin,
	Zdate,
	Zcgoto,
	Zmake,
	Znexterr,
	Zcmdtobuff,
	Zkill,
	Zprevwind,
	Zbegwind,
	Zendwind,
	Zhelp,
	Zgsearch,
	Zindent,
	Zundent,
	Zempty,
	Zmail,
	Zman,
	Zreplace,
	Zsaveall,
	Zbeauty,
	Zviewfile,
	Zdelwind,
	Zshrinkwind,
	Zsizewind,
	Zspell,
	Zeditfile, /* 150 */
	Zscrolldown,
	Zscrollup,
	Zformtab,
	Zcwd,
	Zcmdbind,
	Zsaveconfig,
	Zsetenv,
	Zref,
	Zzoom,
	Zgresrch,
	Zgrep,
	Zundo,
};


Proc (*Vcmds[])() =
{
	Z1wind,
	Z2wind,
	Zabort,
	Zagain,
	Zarg,
	Zbegline,
	Zbpara,
	Zbword,
	Zcalc,
	Znotimpl,
	Zcase,
	Znotimpl,
	Zcopyrgn,
	Zctrlx,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Zendline,
	Zexit,
	Znotimpl,
	Znotimpl,
	Zfilewrite,
	Zmode,
	Znotimpl,
	Zfindfile,
	Zfindtag,
	Zfpara,
	Zfword,
	Zgetbword,
	Zgrowwind,
	Zhexout,
	Znotimpl,
	Zkillbuff,
	Zlgoto,
	Znotimpl,
	Zlstbuff,
	Zmakedel,
	Zmeta,
	Znextchar,
	Znextline,
	Znextpage,
	Znotimpl,
	Znxtbookmrk,
	Znxtothrwind,
	Zquit,
	Znotimpl,
	Znotimpl,
	Zprevchar,
	Zprevline,
	Zprevothrwind,
	Zprevpage,
	Zprintpos,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Zresrch,
	Zrsearch,
	Zsearch,
	Zsetavar,
	Zsetbookmrk,
	Zsetmrk,
	Zshell,
	Zstat,
	Znotimpl,
	Zswapmrk,
	Znextwind,
	Znotimpl,
	Zswitchto,
	Znotimpl,
	Ztoend,
	Ztostart,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Zredisplay,
	Zmetax,
	Zbind,
	Zsavebind,
	Zdispbinds,
	Znextbuff,
	Zkeybind,
	Zcount,
	Zincsrch,
	Zrincsrch,
	Zunmodf,
	Znotimpl,
	Zprint,
	Znotimpl,
	Zmrkpara,
	Znotimpl,
	Znotimpl,
	Zviewline,
	Zstartmacro,
	Zendmacro,
	Zdomacro,
	Znamemacro,
	Zmacron, /* 104 */
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron, /* 119 */
	Zreadmacro,
	Zwritemacro,
	Zcmd,
	Znotimpl,
	Zdate,
	Zcgoto,
	Zmake,
	Znexterr,
#if PIPESH
	Znotimpl,
	Zkill,
#else
	Zcmdtobuff,
	Znotimpl,
#endif
	Zprevwind,
	Zbegwind,
	Zendwind,
	Zhelp,
	Zgsearch,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Zmail,
	Zman,
	Znotimpl,
	Zsaveall,
	Znotimpl,
	Zviewfile,
	Zdelwind,
	Zshrinkwind,
	Zsizewind,
	Znotimpl,
	Zeditfile,	/* 150 */
	Zscrolldown,
	Zscrollup,
	Znotimpl,
	Zcwd,
	Zcmdbind,
	Zsaveconfig,
	Zsetenv,
	Zref,
	Zzoom,
	Zgresrch,
	Zgrep,
	Znotimpl,
};


Proc (*Pawcmds[])() =
{
	Znotimpl,
	Znotimpl,
	Zabort,
	Zpart,			/* redefined for PAW */
	Znotimpl,
	Zbegline,
	Zbpara,
	Zbword,
	Znotimpl,
	Zcapword,
	Zcase,
	Znotimpl,
	Zcopyrgn,
	Zctrlx,
	Zdelchar,
	Zdeleol,
	Zdelline,
	Zdelrgn,
	Zdelwhite,
	Zdelword,
	Zendline,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Zfpara,
	Zfword,
	Zgetbword,
	Znotimpl,
	Znotimpl,
	Pinsert,
	Znotimpl,
	Znotimpl,
	Zlowword,
	Znotimpl,
	Zmakedel,
	Zmeta,
	Znextchar,
	Znextline,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Zoverin,
	Zprevchar,
	Zprevline,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Zquote,
	Zrdelchar,
	Zrdelword,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Zsetmrk,
	Znotimpl,
	Znotimpl,
	Zswapchar,
	Zswapmrk,
	Znotimpl,
	Zswapword,
	Znotimpl,
	Pinsert,	/* override Zinsert */
	Ztoend,
	Ztostart,
	Zupword,
	Zyank,
	Pnewline,
	Pinsert,
	Zfillchk,
	Pnewline,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Zispace,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Zupregion,
	Zlowregion,
	Znotimpl,
	Zstartmacro,
	Zendmacro,
	Zdomacro,
	Znotimpl,
	Zmacron, /* 104 */
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron,
	Zmacron, /* 119 */
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Zdate,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Zempty,
	Zmail,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl, /* 150 */
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Zzoom,
	Znotimpl,
	Znotimpl,
	Znotimpl,
	Zfname,			/* only in the PAW you say? pity... */
	Zmatch,			/* put these AFTER all others */
};

Proc (**Funcs)() = Cmds;


#if DBG
static int Testit ARGS((Proc (*)(), Proc (*)(), int, char));

void Fcheck()
{
	int s1, s2, s3, s4;
	int error = 0;

	/* check the TOLOWER macro */
	if(TOLOWER('c') != 'c') Error("OLDLOWER set wrong in config.h");

	/* check sizes of various stuff */
	if(sizeof(Byte)  != 1) Error("Byte  size wrong!");
	if(sizeof(Short) != 2) Error("Short  size wrong!");
	if(sizeof(Word)  != 4) Error("Word size wrong!");
	if(sizeof(struct cnames) % sizeof(char *))
	{
		++error;
		Dbg("cnames struct size bad: %d\n", sizeof(struct cnames));
	}
	s1 = sizeof(Cnames)	/ sizeof(struct cnames);
	s2 = sizeof(Cmds)	/ sizeof(Proc *);
	s3 = sizeof(Vcmds)	/ sizeof(Proc *);
	s4 = sizeof(Pawcmds)/ sizeof(Proc *) - 2;	/* Paw has 2 extra */
	if((s1 - s2 + s3 - s4) || s1 != NUMFUNCS)
	{
		++error;
		Dbg("Cnames: %d Cmds: %d Vcmds: %d Pawcmds: %d NUMFUNCS: %d\n",
			 s1, s2, s3, s4, NUMFUNCS);
	}

	/* validate the Cnames array the best we can */
	for( s1 = 1; s1 < NUMFUNCS; ++s1 )
	{
		if( Stricmp(Cnames[s1].name, Cnames[s1 - 1].name) <= 0 )
		{
			++error;
			Dbg("Problem: (%d) %s and %s\n",
				s1, Cnames[s1 - 1].name, Cnames[s1].name );
		}
		if( strlen(Cnames[s1].name) > (size_t)30 )
	 	{
			++error;
			Dbg("%s too long\n", Cnames[s1].name );
		}
		if( strncmp(Cnames[s1].name, "Top", 3) == 0 )
		{
			++error;
			Dbg("Zhelp() Top: %s\n", Cnames[s1].name );
		}
		error += Testit( Cmds[s1], Vcmds[s1], s1, 'V' );
		if( Pawcmds[s1] != Pinsert && Pawcmds[s1] != Zpart &&
			Pawcmds[s1] != Pnewline )
				error += Testit( Cmds[s1], Pawcmds[s1], s1, 'P' );
	}

	if(error) Error("INTERNAL ERRORS: check z.out file");
}


#ifdef __STDC__
static int Testit(Proc (*p1)(), Proc (*p2)(), int num, char type)
#else
static int Testit(p1, p2, num, type)
Proc (*p1)(), (*p2)();
int num;
char type;
#endif
{
	if( p1 != Znotimpl && p2 != Znotimpl && p1 != p2 )
	{
		Dbg("Alignment %c: %d\n", type, num);
		return 1;
	}
	return 0;
}
#endif
