
0A. BUG FIX
===========
+ HElvis 1.8h1 -> 1.8h1+
      몇몇 상황에서 한글에 대한 처리가 잘못되는 것을 막았습니다.
+ HElvis 1.8h1+ -> 1.8h2
      한글에 대한 처리를 좀 더 철저히 했습니다.
      (visible, paste, substitute 시에도 제대로 동작합니다.)
+ HElvis 1.8h2 -> 1.8h2-
      paste 부분의 문제가 있어, 예전 부분으로 되돌렸습니다.

0B. KNOWN BUG
=============
+ 가끔 Segmentation fault를 내고 죽습니다.
+ 가끔 입력 중 다음 줄과 하나로 붙습니다.
  * 정확히 이 버그가 나타나는 상황을 포착하시면 제게 메일주세요.

I. Intro
========

   HElvis는 Elvis의 hangul patch version입니다.

   HElvis가 지원하는 한글 코드는 KS완성형 code와 대부분의 조합형 code
   등입니다.  한글이 두 바이트로 이루어져 있고, 첫번째 바이트의 최상위
   bit가 1로 설정되어 있다면, 문제없이 지원될 수 있습니다.
   그밖의 EUC codeset을 쓰는 2 byte 체계의 환경에서도 무리없이 사용될
   수 있습니다.

   Elvis는 Steve Kirkendall이 만든 vi/ex 호환 편집기입니다. 대부분의
   vi/ex 명령을 지원하며, 몇몇 확장된 기능도 제공합니다.

   Elvis는 BSD UNIX, AT&T SysV UNIX, SCO Xenix, Minix, Linux, Coherent,
   MS-DOS, Atari TOS, OS9/68000, VMS, AmigaDos, OS/2 등을 지원합니다.
   HElvis도 아마(?) 이들 환경에서 문제없이 지원될 것입니다.

II. HElvis와 Elvis의 다른점
===========================
   HElvis는 Elvis를 기반으로 하고 있으므로, Elvis의 대부분의 기능을
   수용합니다.  또한, 한글에 대한 처리도 추가되었습니다.  그밖에 달라진
   점은 다음과 같습니다.

+ 새로운 option의 추가
	":set hangulinsert(hi)" 명령은 visual 상태에서 한글이 입력되면
	자동으로 insert 상태로 바뀝니다. default는 hi입니다.

	":set mouse(mou)" 명령은 xterm에서 mouse가 click하는 곳으로
	cursor를 움직이도록 합니다. 원래의 select & paste 기능을
	이용하려면 Shift key와 함께 마우스 버튼을 눌러야 합니다.
	Elvis에서는 언제나 이렇게 움직이지만, HElvis에서는 set 명령을
	이용, 사용자의 취향에 맞출 수 있습니다.  ":set mouse" 명령은
	.exrc 화일에만 넣을 수 있습니다.  default는 nomouse입니다.

+ 버그의 수정
	Elvis 1.8에서 발견된 몇몇 bug를 제거하였습니다. 그밖에 기존의
	HElvis 1.7에 비해 bug를 제거하는 등, 많은 기능이 향상되었습니다.

III. Elvis와 BSD VI/EX 간의 다른점
==================================

   Elvis는 vi/ex와 100% 호환은 아닙니다. Elvis는 vi에 비해 여러 확장된 기능을
   제공하며, 몇몇 빠진 기능도 있고, 어떤 기능은 조금 다른 방식으로 제공됩니다.
   (자세한 내용은 doc/differ.ms를 보세요)

III.1 확장 기능(중 일부)
------------------------

+ Save Configuration
	:mkexrc란 명령은 현재의 여러 setting을 현재 directory의 ".exrc"
	file에 저장합니다.

+ Previous File
	:N 명령과 :prev 명령은 args list에 있는 이전 화일로 이동합니다.

+ Keyword Lookup
	visual 명령 상태에서, shift-K를 누르면, Elvis는 refrence program을
	수행합니다. default는 ref 명령을 수행합니다. ref는 ctags로
	만들어진 화일에서 함수의 prototype을 보여줍니다.

+ Input Mode
	입력 상태에서 backspace로 이전 행으로 돌아갈 수 있습니다.

	입력 상태에서 화살표 key가 동작합니다.

	입력 상태에서 control-Z 를 두번 누르면, 화일을 저장하고 바로 나갑니다.

+ Compiler Interface
	Elvis는 대부분의 compiler에서 나오는 error message를 이해합니다.
	이 기능을 이용하기 위해서는 error message를 "errlist"란 file에
	저장하면 됩니다. Elvis는 ":err" 명령으로 error가 난 곳에
	cursor를 이동시키고 error message를 상태줄에 표시해줍니다.
	":cc"나 ":make" 명령은 자동으로 errlist란 화일로 에러를
	보냅니다.

+ Visible Text Selection
	visual 명령 상태에서 'v'는 글자 선택을, 'V'는 줄 선택을
	시작합니다.  선택된 내용은 역상으로 밝게 표시됩니다. 선택된
	부분에 대해서 여러 vi 명령(c/d/y/</>/!/=/\)을 수행할 수 있습니다.

+ Pop-up Menu Operator
	'\' key는 화면에 간단한 pop-up window를 보여줍니다.

+ keepanon
	보통 Elvis는 새로운 화일을 읽을 때, 실제 vi에서와 같이 anonymous
	cut buffer의 내용을 지웁니다. ":set keepanon"은 이 anonymous cut
	buffer의 내용을 유지하도록 합니다.

+ Move to a Given Percentage
	그냥 '%' key를 누르면 vi에서와 마찬가지로 대응되는 괄호쌍으로
	움직입니다.  하지만, 1에서 100까지의 숫자와 함께 '%'를 누르면,
	file의 해당하는 위치로 움직입니다. 예를 들어, "50%"는 cursor를
	화일의 중간 부분으로 움직입니다.

+ Regular Expressions
	\{n}, \{n,m}, \+, \? 등의 새로운 operator를 제공합니다.

+ Exfilerc
	새로운 화일을 읽을 때마다 home directory의 ".exfilerc"를
	읽어 수행합니다. 이를 이용하여 화일의 종류에 따른 처리를 할 수
	있습니다. ".exfilerc"의 예는 다음과 같습니다.

	set noautoindent
	if *.c
	or *.h
	then set autoindent

	if *.c
	or *.h
	and newfile
	then 1!mkskel %

III.2 빠진 기능
---------------
	화면의 폭보다 긴 줄에 대한 표시가 다릅니다.  실제 vi는 긴 줄을
	wrap시켜 보여주지만, Elvis는 이의 일부만을 보여주고, 나머지를
	보기 위해 옆으로 scroll됩니다.

	LISP에 대한 지원이 빠져 있습니다.

	Autoindent가 조금 다르게 동작합니다.

IV. Package Info
================

Package-Name:	helvis-1.8h2
Title:		Hangul Elvis Package
Version:	1.8h2
Description:	elvis 1.8p4 hangul version h2
Author:		Park Chong-Dae (cdpark@baram.kaist.ac.kr)
Maintained-by:	Park Chong-Dae
Maintained-at:	SPARCS, KAIST, Rep. Korea
Platforms:	Tested on ...
			SunOS 4.1.3 KLE with cc, sysV cc, gcc
			IRIX Release 4.0.5
			IRIX Release 5.3
		Tested on ... (helvis 1.8h1)
			SunOS 4.1.3 KLE with cc, sysV cc, gcc
			SunOS/MP 4.1A.2
			IRIX Release 4.0.5
			IRIX Release 5.2
			IRIX Release 5.3
			MIPS RISC/os (UMIPS) 4.52
			HP PA-RISC HP-UX 9.0
			Linux with gcc
Copying-policy: Copyright (c) 1995 Park Chong-Dae (cdpark@baram.kaist.ac.kr)
		Copyright (c) 1994 Steve Kirkendall (kirkenda@cs.pdx.edu)
Keywords:	elvis, helvis, hangul, editor, vi, vi-clone, hvi
