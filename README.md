    ::      #     #  #######                                      ::
    :::     #     #  #        #       #    #     #     ####      :::
    ::::    #     #  #        #       #    #     #    #         ::::
    :::::   #######  #####    #       #    #     #     ####    :::::
    ::::    #     #  #        #       #    #     #         #    ::::
    :::     #     #  #        #        #  #      #    #    #     :::
    ::      #     #  #######  ######    ##       #     ####       ::

            - a clone of vi/ex -
               version 1.8h2

Intro
=====

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

HElvis와 Elvis의 다른점
====================
HElvis는 Elvis를 기반으로 하고 있으므로, Elvis의 대부분의 기능을
수용합니다.  또한, 한글에 대한 처리도 추가되었습니다.  그밖에 달라진
점은 다음과 같습니다.

* 새로운 option의 추가
  * `:set hangulinsert(hi)` 명령은 visual 상태에서 한글이 입력되면
	자동으로 insert 상태로 바뀝니다. default는 hi입니다.
  * `:set mouse(mou)` 명령은 xterm에서 mouse가 click하는 곳으로
	cursor를 움직이도록 합니다. 원래의 select & paste 기능을
	이용하려면 Shift key와 함께 마우스 버튼을 눌러야 합니다.
	Elvis에서는 언제나 이렇게 움직이지만, HElvis에서는 set 명령을
	이용, 사용자의 취향에 맞출 수 있습니다.  ":set mouse" 명령은
	.exrc 화일에만 넣을 수 있습니다.  default는 nomouse입니다.

Package Info
============
```
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
```
