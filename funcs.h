/* funcs.h - Zedit function defines
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

#define ZONE_WINDOW		0
#define ZSPLIT_WINDOW		(ZONE_WINDOW + 1)
#define ZABORT		(ZSPLIT_WINDOW + 1)
#define ZAGAIN		(ZABORT + 1)
#define ZARG		(ZAGAIN + 1)
#define ZBEGINNING_OF_LINE	(ZARG + 1)
#define ZPREVIOUS_PARAGRAPH		(ZBEGINNING_OF_LINE + 1)
#define ZPREVIOUS_WORD		(ZPREVIOUS_PARAGRAPH + 1)
#define ZCALC		(ZPREVIOUS_WORD + 1)
#define ZCAPITALIZE_WORD	(ZCALC + 1)
#define ZCASE		(ZCAPITALIZE_WORD + 1)
#define ZCENTER		(ZCASE + 1)
#define ZCOPY_REGION	(ZCENTER + 1)
#define ZCTRL_X		(ZCOPY_REGION + 1)
#define ZDELETE_CHAR	(ZCTRL_X + 1)
#define ZDELETE_TO_EOL		(ZDELETE_CHAR + 1)
#define ZDELETE_LINE	(ZDELETE_TO_EOL + 1)
#define ZDELETE_REGION		(ZDELETE_LINE + 1)
#define ZTRIM_WHITE_SPACE	(ZDELETE_REGION + 1)
#define ZDELETE_WORD	(ZTRIM_WHITE_SPACE + 1)
#define ZEND_OF_LINE	(ZDELETE_WORD + 1)
#define ZEXIT		(ZEND_OF_LINE + 1)
#define ZSAVEEXIT	(ZEXIT + 1)
#define ZREAD_FILE	(ZSAVEEXIT + 1)
#define ZSAVE_FILE	(ZREAD_FILE + 1)
#define ZWRITE_FILE	(ZSAVE_FILE + 1)
#define ZMODE		(ZWRITE_FILE + 1)
#define ZFILL_PARAGRAPH	(ZMODE + 1)
#define ZFIND_FILE	(ZFILL_PARAGRAPH + 1)
#define ZFINDTAG	(ZFIND_FILE + 1)
#define ZNEXT_PARAGRAPH		(ZFINDTAG + 1)
#define ZNEXT_WORD		(ZNEXT_PARAGRAPH + 1)
#define ZCOPY_WORD	(ZNEXT_WORD + 1)
#define ZGROW_WINDOWOW	(ZCOPY_WORD + 1)
#define ZHEXOUT		(ZGROW_WINDOWOW + 1)
#define ZINSERT		(ZHEXOUT + 1) /* Also used for pinsert */
#define ZKILLBUFF	(ZINSERT + 1)
#define ZLIFE		(ZKILLBUFF + 1)
#define ZGOTO_LINE		(ZLIFE + 1)
#define ZLOWWORD	(ZGOTO_LINE + 1)
#define ZLSTBUFF	(ZLOWWORD + 1)
#define ZAPPEND_KILL	(ZLSTBUFF + 1)
#define ZMETA		(ZAPPEND_KILL + 1)
#define ZNEXT_CHAR	(ZMETA + 1)
#define ZNEXT_LINE	(ZNEXT_CHAR + 1)
#define ZNEXT_PAGE	(ZNEXT_LINE + 1)
#define ZNOTIMPL	(ZNEXT_PAGE + 1)
#define ZNEXT_BOOKMARK	(ZNOTIMPL + 1)
#define ZOTHER_NEXT_PAGE	(ZNEXT_BOOKMARK + 1)
#define ZOPEN_LINE	(ZOTHER_NEXT_PAGE + 1)
#define ZOVERIN		(ZOPEN_LINE + 1)
#define ZPREVIOUS_CHAR	(ZOVERIN + 1)
#define ZPREVIOUS_LINE	(ZPREVIOUS_CHAR + 1)
#define ZOTHER_PREVIOUS_PAGE	(ZPREVIOUS_LINE + 1)
#define ZPREVIOUS_PAGE	(ZOTHER_PREVIOUS_PAGE + 1)
#define ZPOSITION	(ZPREVIOUS_PAGE + 1)
#define ZQUERY_REPLACE		(ZPOSITION + 1)
#define ZQUOTE		(ZQUERY_REPLACE + 1)
#define ZDELETE_PREVIOUS_CHAR	(ZQUOTE + 1)
#define ZDELETE_PREVIOUS_WORD	(ZDELETE_PREVIOUS_CHAR + 1)
#define ZRE_REPLACE	(ZDELETE_PREVIOUS_WORD + 1)
#define ZRE_SEARCH		(ZRE_REPLACE + 1)
#define ZREVERSE_SEARCH	(ZRE_SEARCH + 1)
#define ZSEARCH		(ZREVERSE_SEARCH + 1)
#define ZSETAVAR	(ZSEARCH + 1)
#define ZSET_BOOKMARK	(ZSETAVAR + 1)
#define ZSET_MARK		(ZSET_BOOKMARK + 1)
#define ZSHELL		(ZSET_MARK + 1)
#define ZSWAPCHAR	(ZSHELL + 1)
#define ZSWAPMRK	(ZSWAPCHAR + 1)
#define ZNEXT_WINDOW	(ZSWAPMRK + 1)
#define ZSWAPWORD	(ZNEXT_WINDOW + 1)
#define ZSWITCHTO	(ZSWAPWORD + 1)
#define ZTAB		(ZSWITCHTO + 1)
#define ZEND_OF_BUFFER		(ZTAB + 1)
#define ZBEGINNING_OF_BUFFER	(ZEND_OF_BUFFER + 1)
#define ZUPWORD		(ZBEGINNING_OF_BUFFER + 1)
#define ZYANK		(ZUPWORD + 1)
#define ZC_INDENT	(ZYANK + 1)
#define ZC_INSERT	(ZC_INDENT + 1)
#define ZFILL_CHECK	(ZC_INSERT + 1)
#define ZNEWLINE	(ZFILL_CHECK + 1)
#define ZREDISPLAY	(ZNEWLINE + 1)
#define ZMETA_X		(ZREDISPLAY + 1)
#define ZBIND		(ZMETA_X + 1)
#define ZSAVEBIND	(ZBIND + 1)
#define ZDISPBINDS	(ZSAVEBIND + 1)
#define ZNEXTBUFF	(ZDISPBINDS + 1)
#define ZKEYBIND	(ZNEXTBUFF + 1)
#define ZCOUNT		(ZKEYBIND + 1)
#define ZINCREMENTAL_SEARCH	(ZCOUNT + 1)
#define ZREVERSE_INC_SEARCH	(ZINCREMENTAL_SEARCH + 1)
#define ZUNMODF		(ZREVERSE_INC_SEARCH + 1)
#define ZDELETE_BLANKS	(ZUNMODF + 1)
#define ZMRKPARA	(ZDELETE_BLANKS + 1)
#define ZUPREGION	(ZMRKPARA + 1)
#define ZLOWREGION	(ZUPREGION + 1)
#define ZVIEW_LINE	(ZLOWREGION + 1)
#define ZCMD_TO_SCREEN	(ZVIEW_LINE + 1)
#define ZJOIN		(ZCMD_TO_SCREEN + 1)
#define ZOUT_TO		(ZJOIN + 1)
#define ZMAKE		(ZOUT_TO + 1)
#define ZNEXTERR	(ZMAKE + 1)
#define ZCMD_TO_BUFFER	(ZNEXTERR + 1)
#define ZKILL		(ZCMD_TO_BUFFER + 1)
#define ZPREVIOUS_WINDOW	(ZKILL + 1)
#define ZHELP		(ZPREVIOUS_WINDOW + 1)
#define ZGLOBAL_SEARCH	(ZHELP + 1)
#define ZINDENT		(ZGLOBAL_SEARCH + 1)
#define ZUNDENT		(ZINDENT + 1)
#define ZEMPTY_BUFFER		(ZUNDENT + 1)
#define ZREPLACE	(ZEMPTY_BUFFER + 1)
#define ZSAVE_ALL_FILES	(ZREPLACE + 1)
#define ZDELETE_WINDOW	(ZSAVE_ALL_FILES + 1)
#define ZSHRINK_WINDOW	(ZDELETE_WINDOW + 1)
#define ZSIZE_WINDOW	(ZSHRINK_WINDOW + 1)
#define ZSPELL		(ZSIZE_WINDOW + 1)
#define ZREVERT_FILE	(ZSPELL + 1)
#define ZSCROLL_DOWN	(ZREVERT_FILE + 1)
#define ZSCROLL_UP	(ZSCROLL_DOWN + 1)
#define ZCWD		(ZSCROLL_UP + 1)
#define ZBOUND_TO	(ZCWD + 1)
#define ZSHOWCONFIG	(ZBOUND_TO + 1)
#define ZSETENV		(ZSHOWCONFIG + 1)
#define ZREF		(ZSETENV + 1)
#define ZGLOBAL_RE_SEARCH	(ZREF + 1)
#define ZGREP		(ZGLOBAL_RE_SEARCH + 1)
#define ZUNDO		(ZGREP + 1)
#define NUMFUNCS	(ZUNDO + 1)

/* this is used by the getfname command in the PAW for command completion */
#define ZFNAME		NUMFUNCS
