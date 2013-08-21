struct cnames Cnames[] = {
	{"abort",			ZABORT,		H_MISC},
	{"again",			ZAGAIN,		H_SEARCH},
	{"append-kill",			ZMAKEDEL,	H_DELETE},
	{"beginning-of-buffer",		ZTOSTART,	H_CURSOR},
	{"beginning-of-line",		ZBEGLINE,	H_CURSOR},
	{"beginning-of-window",		ZBEGWIND,	H_CURSOR},
	{"bind",			ZBIND,		H_BIND},
	{"bound-to",			ZCMDBIND,	H_BIND},
	{"c-beautify",			ZBEAUTY,	H_SHELL},
	{"c-indent",			ZCINDENT,	H_MODE},
	{"c-insert",			ZCINSERT,	H_NONE},
	{"c-x",				ZCTRLX,		H_NONE},
	{"calculator",			ZCALC,		H_HELP},
	{"capitalize-word",		ZCAPWORD,	H_DISP},
	{"center-line",			ZCENTER,	H_DISP},
	{"cmd-to-buffer",		ZCMDTOBUFF,	H_SHELL},
	{"cmd-to-screen",		ZCMD,		H_SHELL},
	{"copy-region",			ZCOPYRGN,	H_DELETE},
	{"copy-word",			ZGETBWORD,	H_DELETE},
	{"count",			ZCOUNT,		H_HELP},
	{"cwd",				ZCWD,		H_MISC},
	{"date",			ZDATE,		H_HELP},
	{"delete-blanks",		ZDELBLANKS,	H_DELETE},
	{"delete-buffer",		ZKILLBUFF,	H_BUFF},
	{"delete-character",		ZDELCHAR,	H_DELETE},
	{"delete-line",			ZDELLINE,	H_DELETE},
	{"delete-previous-character",	ZRDELCHAR,	H_DELETE},
	{"delete-previous-word",	ZRDELWORD,	H_DELETE},
	{"delete-region",		ZDELRGN,	H_DELETE},
	{"delete-to-eol",		ZDELEOL,	H_DELETE},
	{"delete-window",		ZDELWIND,	H_BUFF},
	{"delete-word",			ZDELWORD,	H_DELETE},
	{"display-bindings",		ZDISPBINDS,	H_BIND},
	{"empty-buffer",		ZEMPTY,		H_BUFF},
	{"end-of-buffer",		ZTOEND,		H_CURSOR},
	{"end-of-line",			ZENDLINE,	H_CURSOR},
	{"end-of-window",		ZENDWIND,	H_CURSOR},
	{"exit",			ZEXIT,		H_MISC},
	{"expression-replace",		ZREREPLACE,	H_SEARCH},
	{"expression-search",		ZRESRCH,	H_SEARCH},
	{"extended-command",		ZMETAX,		H_MISC},
	{"fill-check",			ZFILLCHK,	H_NONE},
	{"fill-paragraph",		ZFILLPARA,	H_MODE},
	{"find-file",			ZFINDFILE,	H_FILE},
	{"find-tag",			ZFINDTAG,	H_FILE},
	{"form-tab",			ZFORMTAB,	H_MODE},
	{"global-re-search",		ZGRESRCH,	H_SEARCH},
	{"global-search",		ZGSEARCH,	H_SEARCH},
	{"goto-line",			ZLGOTO,		H_CURSOR},
	{"grep",			ZGREP,		H_SHELL},
	{"grow-window",			ZGROWWINDOW,	H_BUFF},
	{"help",			ZHELP,		H_HELP},
	{"hex-output",			ZHEXOUT,	H_HELP},
	{"incremental-search",		ZINCSRCH,	H_SEARCH},
	{"indent",			ZINDENT,	H_DISP},
	{"insert-character",		ZINSERT,	H_NONE},
	{"insert-overwrite",		ZOVERIN,	H_MODE},
	{"insert-space",		ZISPACE,	H_DISP},
	{"join",			ZJOIN,		H_DELETE},
	{"key-binding",			ZKEYBIND,	H_BIND},
	{"kill",			ZKILL,		H_SHELL},
	{"list-buffers",		ZLSTBUFF,	H_BUFF},
	{"lowercase-region",		ZLOWREGION,	H_DISP},
	{"lowercase-word",		ZLOWWORD,	H_DISP},
	{"mail",			ZMAIL,		H_SHELL},
	{"make",			ZMAKE,		H_SHELL},
	{"man",				ZMAN,		H_SHELL},
	{"mark-paragraph",		ZMRKPARA,	H_DELETE},
	{"meta",			ZMETA,		H_NONE},
	{"mode",			ZMODE,		H_MODE},
	{"newline",			ZNEWLINE,	H_NONE},
	{"next-bookmark",		ZNXTBOOKMRK,	H_CURSOR},
	{"next-buffer",			ZNEXTBUFF,	H_BUFF},
	{"next-character",		ZNEXTCHAR,	H_CURSOR},
	{"next-error",			ZNEXTERR,	H_SHELL},
	{"next-line",			ZNEXTLINE,	H_CURSOR},
	{"next-page",			ZNEXTPAGE,	H_CURSOR},
	{"next-paragraph",		ZFPARA,		H_CURSOR},
	{"next-window",			ZNEXTWIND,	H_DISP},
	{"next-word",			ZFWORD,		H_CURSOR},
	{"null",			ZNOTIMPL,	H_BIND},
	{"one-window",			Z1WIND,		H_BUFF},
	{"open-line",			ZOPENLINE,	H_DISP},
	{"other-next-page",		ZNXTOTHRWIND,	H_BUFF},
	{"other-previous-page",		ZPREVOTHRWIND,	H_BUFF},
	{"out-to",			ZCGOTO,		H_CURSOR},
	{"position",			ZPRINTPOS,	H_HELP},
	{"previous-character",		ZPREVCHAR,	H_CURSOR},
	{"previous-line",		ZPREVLINE,	H_CURSOR},
	{"previous-page",		ZPREVPAGE,	H_CURSOR},
	{"previous-paragraph",		ZBPARA,		H_CURSOR},
	{"previous-window",		ZPREVWIND,	H_DISP},
	{"previous-word",		ZBWORD,		H_CURSOR},
	{"print",			ZPRINT,		H_SHELL},
	{"query-replace",		ZQUERY,		H_SEARCH},
	{"quit",			ZQUIT,		H_MISC},
	{"quote",			ZQUOTE,		H_MISC},
	{"read-file",			ZFILEREAD,	H_FILE},
	{"redisplay",			ZREDISPLAY,	H_DISP},
	{"reference",			ZREF,		H_FILE},
	{"replace",			ZREPLACE,	H_SEARCH},
	{"reverse-inc-search",		ZRINCSRCH,	H_SEARCH},
	{"reverse-search",		ZRSEARCH,	H_SEARCH},
	{"revert-file",			ZREVERTFILE,	H_FILE},
	{"save-all-files",		ZSAVEALL,	H_FILE},
	{"save-bindings",		ZSAVEBIND,	H_BIND},
	{"save-config",			ZSAVECONFIG,	H_HELP},
	{"save-file",			ZFILESAVE,	H_FILE},
	{"scroll-down",			ZSCROLLDOWN,	H_DISP},
	{"scroll-up",			ZSCROLLUP,	H_DISP},
	{"search",			ZSEARCH,	H_SEARCH},
	{"set-bookmark",		ZSETBOOKMRK,	H_CURSOR},
	{"set-mark",			ZSETMRK,	H_DELETE},
	{"set-variable",		ZSETAVAR,	H_HELP},
	{"setenv",			ZSETENV,	H_MISC},
	{"shell",			ZSHELL,		H_SHELL},
	{"shrink-window",		ZSHRINKWIND,	H_BUFF},
	{"size-window",			ZSIZEWIND,	H_BUFF},
	{"spell",			ZSPELL,		H_SHELL},
	{"stats",			ZSTAT,		H_HELP},
	{"swap-mark-and-point",		ZSWAPMRK,	H_CURSOR},
	{"switch-to-buffer",		ZSWITCHTO,	H_BUFF},
	{"tab",				ZTAB,		H_NONE},
	{"toggle-case",			ZCASE,		H_SEARCH},
	{"transpose-characters",	ZSWAPCHAR,	H_DISP},
	{"transpose-words",		ZSWAPWORD,	H_DISP},
	{"trim-white-space",		ZDELWHITE,	H_DELETE},
	{"two-windows",			Z2WIND,		H_BUFF},
	{"undent",			ZUNDENT,	H_DISP},
	{"undo",			ZUNDO,		H_MISC},
	{"universal-argument",		ZARG,		H_MISC},
	{"unmodify",			ZUNMODF,	H_DELETE},
	{"uppercase-region",		ZUPREGION,	H_DISP},
	{"uppercase-word",		ZUPWORD,	H_DISP},
	{"view-file",			ZVIEWFILE,	H_FILE},
	{"view-line",			ZVIEWLINE,	H_DISP},
	{"write-file",			ZFILEWRITE,	H_FILE},
	{"yank",			ZYANK,		H_DELETE},
	{"zoom",			ZZOOM,		H_DISP},
};