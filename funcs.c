/* funcs.c - func lists
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
#include "cnames.h"

void (*Cmds[][2])() = {
	{ Zone_window,		Znotimpl },
	{ Zsplit_window,		Znotimpl },
	{ Zabort,		Zabort },
	{ Zagain,		Zpart },
	{ Zarg,			Znotimpl },
	{ Zbeginning_of_line,		Zbeginning_of_line },
	{ Zbpara,		Zbpara },
	{ Zprevious_word,		Zprevious_word },
	{ Zcalc,		Znotimpl },
	{ Zcapitalize_word,	Zcapitalize_word },
	{ Zcase,		Zcase },
	{ Zcenter,		Znotimpl },
	{ Zcopy_region,		Zcopy_region },
	{ Zctrl_x,		Zctrl_x },
	{ Zdelete_char,		Zdelete_char },
	{ Zdelete_to_eol,		Zdelete_to_eol },
	{ Zdelete_line,		Zdelete_line },
	{ Zdelete_region,		Zdelete_region },
	{ Zdelwhite,		Zdelwhite },
	{ Zdelete_word,		Zdelete_word },
	{ Zend_of_line,		Zend_of_line },
	{ Zexit,		Znotimpl },
	{ Zsave_and_exit,	Znotimpl },
	{ Zread_file,		Znotimpl },
	{ Zsave_file,		Znotimpl },
	{ Zwrite_file,		Znotimpl },
	{ Zmode,		Znotimpl },
	{ Zfillpara,		Znotimpl },
	{ Zfind_file,		Znotimpl },
	{ Zfindtag,		Znotimpl },
	{ Zfpara,		Zfpara },
	{ Znext_word,		Znext_word },
	{ Zcopy_word,		Zcopy_word },
	{ Zgrow_window,		Znotimpl },
	{ Zhexout,		Znotimpl },
	{ Zinsert,		pinsert },
	{ Zkillbuff,		Znotimpl },
	{ Zlife,		Znotimpl },
	{ Zgoto_line,		Znotimpl },
	{ Zlowercase_word,	Zlowercase_word },
	{ Zlstbuff,		Znotimpl },
	{ Zappend_kill,		Zappend_kill },
	{ Zmeta,		Zmeta },
	{ Znext_char,		Znext_char },
	{ Znext_line,		Znext_line },
	{ Znext_page,		Znotimpl },
	{ Znotimpl,		Znotimpl },
	{ Znext_bookmark,		Znotimpl },
	{ Zother_next_page,		Znotimpl },
	{ Zopen_line,		Znotimpl },
	{ Zoverin,		Zoverin },
	{ Zprevious_char,		Zprevious_char },
	{ Zprevious_line,		Zprevious_line },
	{ Zother_previous_page,	Znotimpl },
	{ Zprevious_page,		Znotimpl },
	{ Zprintpos,		Znotimpl },
	{ Zquery_replace,		Znotimpl },
	{ Zquote,		Zquote },
	{ Zdelete_previous_char,		Zdelete_previous_char },
	{ Zdelete_previous_word,		Zdelete_previous_word },
	{ Zre_replace,		Znotimpl },
	{ Zre_search,		Znotimpl },
	{ Zreverse_search,		Znotimpl },
	{ Zsearch,		Znotimpl },
	{ Zsetavar,		Znotimpl },
	{ Zset_bookmark,		Znotimpl },
	{ Zsetmrk,		Zsetmrk },
	{ Zshell,		Znotimpl },
	{ Zswap_chars,		Zswap_chars },
	{ Zswap_mark,		Zswap_mark },
	{ Znext_window,		Znotimpl },
	{ Zswap_words,		Zswap_words },
	{ Zswitchto,		Znotimpl },
	{ Ztab,			pinsert },
	{ Zend_of_buffer,		Zend_of_buffer },
	{ Zbeginning_of_buffer,	Zbeginning_of_buffer },
	{ Zuppercase_word,	Zuppercase_word },
	{ Zyank,		Zyank },
	{ Zc_indent,		pnewline },
	{ Zc_insert,		pinsert },
	{ Zfillchk,		Zfillchk },
	{ Znewline,		pnewline },
	{ Zredisplay,		Znotimpl },
	{ Zmeta_x,		Znotimpl },
	{ Zbind,		Znotimpl },
	{ Zsavebind,		Znotimpl },
	{ Zdispbinds,		Znotimpl },
	{ Znextbuff,		Znotimpl },
	{ Zkeybind,		Znotimpl },
	{ Zcount,		Znotimpl },
	{ Zincremental_search,		Znotimpl },
	{ Zreverse_inc_search,		Znotimpl },
	{ Zunmodf,		Znotimpl },
	{ Zdelete_blanks,		Znotimpl },
	{ Zmrkpara,		Znotimpl },
	{ Zupregion,		Zupregion },
	{ Zlowregion,		Zlowregion },
	{ Zview_line,		Znotimpl },
	{ Zcmd_to_screen,	Znotimpl },
	{ Zjoin,		Znotimpl },
	{ Zout_to,		Znotimpl },
	{ Zmake,		Znotimpl },
	{ Znexterr,		Znotimpl },
	{ Zcmd_to_buffer,	Znotimpl },
	{ Zkill,		Znotimpl },
	{ Zprevious_window,		Znotimpl },
	{ Zhelp,		Znotimpl },
	{ Zglobal_search,		Znotimpl },
	{ Zindent,		Znotimpl },
	{ Zundent,		Znotimpl },
	{ Zempty_buffer,		Zempty_buffer },
	{ Zreplace,		Znotimpl },
	{ Zsave_all_files,		Znotimpl },
	{ Zdelete_window,		Znotimpl },
	{ Zshrink_window,		Znotimpl },
	{ Zsize_window,		Znotimpl },
	{ Zspell,		Znotimpl },
	{ Zrevert_file,		Znotimpl },
	{ Zscroll_down,		Znotimpl },
	{ Zscroll_up,		Znotimpl },
	{ Zcwd,			Znotimpl },
	{ Zbound_to,		Znotimpl },
	{ Zshowconfig,		Znotimpl },
	{ Zsetenv,		Znotimpl },
	{ Zref,			Znotimpl },
	{ Zglobal_re_search,		Znotimpl },
	{ Zgrep,		Znotimpl },
	{ Zundo,		Znotimpl },
	/* only in the PAW you say? pity... */
	{ Znotimpl,		Zfname },
};
int Curcmds;
