/* main.c */

/* Author:
 *	Steve Kirkendall
 *	1500 SW Park #326
 *	Portland OR, 97201
 *	kirkenda@cs.pdx.edu
 */


/* This file contains the main() function of vi */

/* HACK! bcc needs to disable use of precompiled headers for this file,
   or else command line args will not be passed to elvis */
#if __BORLANDC__
#include "borland.h"
#endif

#include "config.h"
#include <setjmp.h>
#include "vi.h"

extern SIGTYPE	trapint(); /* defined below */
jmp_buf		jmpenv;

#ifndef NO_DIGRAPH
static init_digraphs P_((void));
#endif

/*---------------------------------------------------------------------*/

#ifndef NO_GNU
/* This function writes a string out to stderr */
static void errout(str)
	char	*str;	/* the string to be output */
{
	write(2, str, strlen(str));
}
#endif

/*---------------------------------------------------------------------*/

#if AMIGA
# include "amiwild.c"
main (argc, argv)
#else
# if VMS
#  include "vmswild.c"
main (argc, argv)
# else
#  if TURBOC
int main(argc, argv)
#  else
void main(argc, argv)
#  endif
# endif
#endif
	int	argc;
	char	*argv[];
{
	int	i;
	char	*cmd = (char *)0;
	char	*err = (char *)0;
	char	*str;
	char	*tag = (char *)0;

#ifdef __EMX__
	/* expand wildcards a la Unix */
	_wildcard(&argc, &argv);
#endif
#ifndef NO_GNU
	/* check for special GNU options. */
	if (argc == 2 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-?")))
	{
		errout("usage: ");
		errout(argv[0]);
		errout(" [options] [+excmd] [files]...\n");
		errout("options: -R          readonly -- discourage accidental overwrites\n");
#ifndef NO_SAFER
		errout("         -s          safer -- disallow some unsecure commands\n");
#endif
		errout("         -v          start in \"vi\" screen editor mode\n");
		errout("         -e          start in \"ex\" line editor mode\n");
# ifndef NO_EXTENSIONS
		errout("         -i          start in vi's \"input\" mode\n");
# endif
		errout("         -t tagname  begin by looking up the tag \"tagname\"\n");
# ifndef NO_ERRLIST
		errout("         -m errlist  begin at first error listed in file \"errlist\"\n");
# endif
# ifndef CRUNCH
		errout("         -c excmd    begin by executing ex command \"excmd\"\n");
		errout("         -w wsize    set window size to \"wsize\" lines\n");
# endif
		exit(0);
	}
	else if (argc == 2 && !strcmp(argv[1], "--version"))
	{
		errout(VERSION);
		errout("\n");
		exit(0);
	}
#endif /* ndef NO_GNU*/

	/* set mode to MODE_VI or MODE_EX depending on program name */
	switch (argv[0][strlen(argv[0]) - 1])
	{
	  case 'x':			/* "ex" */
		mode = MODE_EX;
		break;

	  case 'w':			/* "view" */
		mode = MODE_VI;
		*o_readonly = TRUE;
		break;
#ifndef NO_EXTENSIONS
	  case 't':			/* "edit" or "input" */
		mode = MODE_VI;
		*o_inputmode = TRUE;
		break;
#endif
	  default:			/* "vi" or "elvis" */
		mode = MODE_VI;
	}

#ifndef DEBUG
# ifdef	SIGQUIT
	/* normally, we ignore SIGQUIT.  SIGINT is trapped later */
	signal(SIGQUIT, SIG_IGN);
# endif
#endif

	/* temporarily ignore SIGINT */
	signal(SIGINT, SIG_IGN);

	/* start curses */
	initscr();
	cbreak();
	noecho();
	scrollok(stdscr, TRUE);

	/* arrange for deadly signals to be caught */
# ifdef SIGHUP
	signal(SIGHUP, deathtrap);
# endif
# ifndef DEBUG
#  ifdef SIGILL
	signal(SIGILL, deathtrap);
#  endif
#  ifdef SIGBUS
	signal(SIGBUS, deathtrap);
#  endif
#  ifdef SIGSEGV
	signal(SIGSEGV, deathtrap);
#  endif
#  ifdef SIGSYS
	signal(SIGSYS, deathtrap);
#  endif
# endif /* !DEBUG */
# ifdef SIGPIPE
	signal(SIGPIPE, deathtrap);
# endif
# ifdef SIGTERM
	signal(SIGTERM, deathtrap);
# endif
# ifdef SIGUSR1
	signal(SIGUSR1, deathtrap);
# endif
# ifdef SIGUSR2
	signal(SIGUSR2, deathtrap);
# endif

	/* initialize the options - must be done after initscr(), so that
	 * we can alter LINES and COLS if necessary.
	 */
	initopts();

	/* map the arrow keys.  The KU,KD,KL,and KR variables correspond to
	 * the :ku=: (etc.) termcap capabilities.  The variables are defined
	 * as part of the curses package.
	 */
	if (has_KU) mapkey(has_KU, "k",    WHEN_VICMD|WHEN_INMV, "<Up>");
	if (has_KD) mapkey(has_KD, "j",    WHEN_VICMD|WHEN_INMV, "<Down>");
	if (has_KL) mapkey(has_KL, "h",    WHEN_VICMD|WHEN_INMV, "<Left>");
	if (has_KR) mapkey(has_KR, "l",    WHEN_VICMD|WHEN_INMV, "<Right>");
	if (has_HM) mapkey(has_HM, "^",    WHEN_VICMD|WHEN_INMV, "<Home>");
	if (has_EN) mapkey(has_EN, "$",    WHEN_VICMD|WHEN_INMV, "<End>");
	if (has_PU) mapkey(has_PU, "\002", WHEN_VICMD|WHEN_INMV, "<PageUp>");
	if (has_PD) mapkey(has_PD, "\006", WHEN_VICMD|WHEN_INMV, "<PageDn>");
	if (has_KI) mapkey(has_KI, "i",    WHEN_VICMD|WHEN_INMV, "<Insert>");
#if MSDOS || OS2
# ifdef RAINBOW
	if (!strcmp("rainbow", o_term))
	{
		mapkey("\033[1~",  "/",		WHEN_VICMD,		"<Find>");
		mapkey("\033[3~",  "x",		WHEN_VICMD|WHEN_INMV,	"<Remove>");
		mapkey("\033[4~",  "v",		WHEN_VICMD|WHEN_INMV,	"<Select>");
		mapkey("\033[17~", ":sh\n",	WHEN_VICMD,		"<Intrpt>");
		mapkey("\033[19~", ":q\n",	WHEN_VICMD,		"<Cancel>");
		mapkey("\033[21~", "ZZ",	WHEN_VICMD,		"<Exit>");
		mapkey("\033[26~", "V",		WHEN_VICMD|WHEN_INMV,	"<AddlOp>");
		mapkey("\033[28~", "\\",	WHEN_VICMD|WHEN_INMV,	"<Help>");
		mapkey("\033[29~", "K",		WHEN_VICMD|WHEN_INMV,	"<Do>");
	}
	else
# endif /* RAINBOW */
	{
		mapkey("#s", "B", WHEN_VICMD|WHEN_INMV,	"^<Left>");
		mapkey("#t", "W", WHEN_VICMD|WHEN_INMV,	"^<Right>");
	}
#else /* not MSDOS */
# if AMIGA
	mapkey("\233?~", "\\",	WHEN_VICMD|WHEN_INMV,	"<Help>");
# endif
	if (has_kD && *has_kD != ERASEKEY)
	{
		mapkey(has_kD, "x", WHEN_VICMD|WHEN_INMV, "<Del>");
	}
/*** Comment out by cdpark
	else if (ERASEKEY != '\177')
	{
		mapkey("\177", "x", WHEN_VICMD|WHEN_INMV, "<Del>");
	}
***/

#ifndef NO_DIGRAPH
	init_digraphs();
#endif /* NO_DIGRAPH */

	/* process any flags */
	for (i = 1; i < argc && *argv[i] == '-'; i++)
	{
		switch (argv[i][1])
		{
		  case 'R':	/* readonly */
			*o_readonly = TRUE;
			break;

		  case 'L':
		  case 'r':	/* recover */
			msg("Use the `elvrec` program to recover lost files");
			endmsgs();
			refresh();
			endwin();
			exit(1);
			break;

		  case 't':	/* tag */
			if (argv[i][2])
			{
				tag = argv[i] + 2;
			}
			else
			{
				tag = argv[++i];
			}
			break;

		  case 'v':	/* vi mode */
			mode = MODE_VI;
			break;

		  case 'e':	/* ex mode */
			mode = MODE_EX;
			break;
#ifndef NO_EXTENSIONS
		  case 'i':	/* input mode */
			*o_inputmode = TRUE;
			break;
#endif
#ifndef NO_ERRLIST
		  case 'm':	/* use "errlist" as the errlist */
			if (argv[i][2])
			{
				err = argv[i] + 2;
			}
			else if (i + 1 < argc)
			{
				err = argv[++i];
			}
			else
			{
				err = "";
			}
			break;
#endif
#ifndef NO_SAFER
		  case 's':
			*o_safer = TRUE;
			break;
#endif
#ifndef CRUNCH
		  case 'c':	/* run the following command, later */
			if (argv[i][2])
			{
				cmd = argv[i] + 2;
			}
			else
			{
				cmd = argv[++i];
			}
			break;

		  case 'w':	/* set the window size */
			if (argv[i][2])
			{
				*o_window = atoi(argv[i] + 2);
			}
			else
			{
				*o_window = atoi(argv[++i]);
			}
			break;
#endif
		  default:
			msg("Ignoring unknown flag \"%s\"", argv[i]);
		}
	}

	/* if we were given an initial ex command, save it... */
	if (i < argc && *argv[i] == '+')
	{
		if (argv[i][1])
		{
			cmd = argv[i++] + 1;
		}
		else
		{
			cmd = "$"; /* "vi + file" means start at EOF */
			i++;
		}
	}

	/* the remaining args are file names. */
	if (i < argc)
	{
		unsigned char *p = (unsigned char *)args - 1;

		strcpy(args, argv[i]);
		/* convert spaces in filenames to CRs so can use space as
		 * delimiter for multiple filenames */
		while (*++p)
		{
			if (*p == ' ')
			{
				*p = SPACEHOLDER;
			}
		}
		while (++i < argc && strlen(args) + 1 + strlen(argv[i]) < sizeof args)
		{
			*p = ' ';
			strcpy(p+1, argv[i]);
			while (*++p)
			{
				if (*p == ' ')
				{
					*p = SPACEHOLDER;
				}
			}
		}
#ifndef __EMX__
# if MSDOS || TOS || OS2
		/* expand wildcard characters, if necessary */
		if (strchr(args, '*') || strchr(args, '?'))
		{
			strcpy(args, wildcard(args));
		}
# endif
#endif
		strcpy(tmpblk.c, args);
		cmd_args(MARK_UNSET, MARK_UNSET, CMD_ARGS, TRUE, tmpblk.c);
	}
	else
	{
		/* empty args list */
		args[0] = '\0';
		nargs = 1;
		argno = -1;
	}

	/* perform the .exrc files and EXINIT environment variable */
#ifdef SYSEXRC
	doexrc(SYSEXRC);
#endif
#ifdef EXINIT
	str = getenv(EXINIT);
	if (str)
	{
	  	if (*str == '"')
		  	str++;
		if (str[strlen(str) - 1] == '"')
		  	str[strlen(str) - 1] = 0;
		doexcmd(str, ctrl('V'));
	}
	else
#endif
#ifdef HMEXRC
	if ((str = gethome(argv[0]))	/* yes, ASSIGNMENT! */
		&& *str)
	{
		strcpy(tmpblk.c, str);
		str = tmpblk.c + strlen(tmpblk.c);
#if !VMS
# if AMIGA	/* Don't SLASH a device. "Elvis:.exrc" */
		if (str[-1] != COLON && str[-1] != SLASH)
# else
		if (str[-1] != SLASH)
# endif
		{
			*str++ = SLASH;
		}
#endif
		strcpy(str, HMEXRC);
		doexrc(tmpblk.c);
	}
#else
		; /* marks end of EXINT's "else" */
#endif
#ifndef CRUNCH
	if (*o_exrc)
#endif
	{
#ifndef NO_SAFER
		i = *o_safer;
		*o_safer = TRUE;
		doexrc(EXRC);
		*o_safer = i;
#else
		doexrc(EXRC);
#endif
	}

# if ANY_UNIX
#  ifndef NO_MOUSE
	if (*o_mouse && !strcmp(o_term, "xterm"))
	{
		write(1, "\033[?9h", 5); /* xterm: enable mouse events */
		mapkey("\033[M", "\021", WHEN_VICMD, "<Mouse>");
	}
#  endif
# endif
#endif
	/* search for a tag (or an error) now, if desired */
	blkinit();
	if (tag)
	{
		cmd_tag(MARK_UNSET, MARK_FIRST, CMD_TAG, 0, tag);
	}
#ifndef NO_ERRLIST
	else if (err)
	{
		cmd_errlist(MARK_FIRST, MARK_FIRST, CMD_ERRLIST, 0, err);
	}
#endif

	/* if no tag/err, or tag failed, then start with first arg */
	if (tmpfd < 0)
	{
		/* start with first arg */
		cmd_next(MARK_UNSET, MARK_UNSET, CMD_NEXT, FALSE, "");

		/* pretend to do something, just to force a recoverable
		 * version of the file out to disk
		 */
		ChangeText
		{
		}
		clrflag(file, MODIFIED);
	}

	/* now we do the immediate ex command that we noticed before */
	if (cmd)
	{
		doexcmd(cmd, '\\');
	}

	/* repeatedly call ex() or vi() (depending on the mode) until the
	 * mode is set to MODE_QUIT
	 */
	while (mode != MODE_QUIT)
	{
		if (setjmp(jmpenv))
		{
			/* Maybe we just aborted a change? */
			abortdo();
		}
		signal(SIGINT, trapint);

		switch (mode)
		{
		  case MODE_VI:
			if (canvi)
			{
				vi();
			}
			else
			{
				mode = MODE_EX;
				msg("This termcap entry doesn't support visual mode");
			}
			break;

		  case MODE_EX:
			ex();
			break;
#ifdef DEBUG
		  default:
			msg("mode = %d?", mode);
			mode = MODE_QUIT;
#endif
		}
	}

	/* free up the cut buffers */
	cutend();

	/* end curses */
#ifndef	NO_CURSORSHAPE
	if (has_CQ)
		do_CQ();
#endif
#if ANY_UNIX
# ifndef NO_MOUSE
	if (*o_mouse && !strcmp(o_term, "xterm"))
	{
		write(1, "\033[?9r", 5); /* xterm: disable mouse events */
	}
# endif
#endif
	endmsgs();
	move(LINES - 1, 0);
	clrtoeol();
	refresh();
	endwin();

	exit(exitcode);
	/*NOTREACHED*/
#if TURBOC
	return 0;
#endif
}


/*ARGSUSED*/
SIGTYPE trapint(signo)
	int	signo;
{
	beep();
	resume_curses(FALSE);
	abortdo();
#if OSK
	sigmask(-1);
#endif
	signal(signo, trapint);
	doingglobal = FALSE;

	longjmp(jmpenv, 1);
	/*NOTREACHED*/
}



#ifndef NO_DIGRAPH

/* This stuff us used to build the default digraphs table. */
static char	digtable[][4] =
{
# ifdef CS_IBMPC
	"C,\200",	"u\"\1",	"e'\2",		"a^\3",
	"a\"\4",	"a`\5",		"a@\6",		"c,\7",
	"e^\10",	"e\"\211",	"e`\12",	"i\"\13",
	"i^\14",	"i`\15",	"A\"\16",	"A@\17",
	"E'\20",	"ae\21",	"AE\22",	"o^\23",
	"o\"\24",	"o`\25",	"u^\26",	"u`\27",
	"y\"\30",	"O\"\31",	"U\"\32",	"a'\240",
	"i'!",		"o'\"",		"u'#",		"n~$",
	"N~%",		"a-&",		"o-'",		"~?(",
	"~!-",		"\"<.",		"\">/",
#  ifdef CS_SPECIAL
	"2/+",		"4/,",		"^+;",		"^q<",
	"^c=",		"^r>",		"^t?",		"pp]",
	"^^^",		"oo_",		"*a`",		"*ba",
	"*pc",		"*Sd",		"*se",		"*uf",
	"*tg",		"*Ph",		"*Ti",		"*Oj",
	"*dk",		"*Hl",		"*hm",		"*En",
	"*No",		"eqp",		"pmq",		"ger",
	"les",		"*It",		"*iu",		"*/v",
	"*=w",		"sq{",		"^n|",		"^2}",
	"^3~",		"^_\377",
#  endif /* CS_SPECIAL */
# endif /* CS_IBMPC */
# ifdef CS_LATIN1
	"~!!",		"a-*",		"\">+",		"o-:",
	"\"<>",		"~??",

	"A`@",		"A'A",		"A^B",		"A~C",
	"A\"D",		"A@E",		"AEF",		"C,G",
	"E`H",		"E'I",		"E^J",		"E\"K",
	"I`L",		"I'M",		"I^N",		"I\"O",
	"-DP",		"N~Q",		"O`R",		"O'S",
	"O^T",		"O~U",		"O\"V",		"O/X",
	"U`Y",		"U'Z",		"U^[",		"U\"\\",
	"Y'_",

	"a``",		"a'a",		"a^b",		"a~c",
	"a\"d",		"a@e",		"aef",		"c,g",
	"e`h",		"e'i",		"e^j",		"e\"k",
	"i`l",		"i'm",		"i^n",		"i\"o",
	"-dp",		"n~q",		"o`r",		"o's",
	"o^t",		"o~u",		"o\"v",		"o/x",
	"u`y",		"u'z",		"u^{",		"u\"|",
	"y'~",
# endif /* CS_LATIN1 */
	""
};

static int init_digraphs()
{
	int	i;

	for (i = 0; *digtable[i]; i++)
	{
		do_digraph(FALSE, digtable[i]);
	}
	do_digraph(FALSE, (char *)0);
	return 0;
}
#endif /* NO_DIGRAPH */
