/* move3.c */

/* Author:
 *	Steve Kirkendall
 *	1500 SW Park #326
 *	Portland OR, 97201
 *	kirkenda@cs.pdx.edu
 */


/* This file contains movement functions that perform character searches */

#include "config.h"
#include "vi.h"

#ifndef NO_CHARSEARCH
static MARK	(*prevfwdfn) P_((MARK,long,int));	/* function to search in same direction */
static MARK	(*prevrevfn) P_((MARK,long,int));	/* function to search in opposite direction */
static char	prev_key;	/* sought cvhar from previous [fFtT] */
static char	prev_key2;	/* second byte of sought char if it is HANGUL */
static char	samekey = 0;	/* boolean: is command ';' or ','? */

MARK	m__ch(m, cnt, cmd)
	MARK	m;	/* current position */
	long	cnt;
	int	cmd;	/* command: either ',' or ';' */
{
	MARK	(*tmp)();

	if (!prevfwdfn)
	{
		msg("No previous f, F, t, or T command");
		return MARK_UNSET;
	}

	samekey = 1;

	if (cmd == ',')
	{
		m =  (*prevrevfn)(m, cnt, prev_key);

		/* Oops! we didn't want to change the prev*fn vars! */
		tmp = prevfwdfn;
		prevfwdfn = prevrevfn;
		prevrevfn = tmp;

		return m;
	}
	else
	{
		return (*prevfwdfn)(m, cnt, prev_key);
	}
}

/* move forward within this line to next occurrence of key */
MARK	m_fch(m, cnt, key)
	MARK	m;	/* where to search from */
	long	cnt;
	int	key;	/* what to search for */
{
	REG char	*text;
	int	key2;	/* 2nd byte if HANGUL */

	DEFAULT(1);

	prevfwdfn = m_fch;
	prevrevfn = m_Fch;
	prev_key = key;

	if (samekey)
	{
		key2 = prev_key2;
	}
	else if (IsHiBitOn(key))
	{
		prev_key2 = key2 = getkey(0);
	}
	samekey = 0;

	pfetch(markline(m));
	text = ptext + markidx(m);
	while (cnt-- > 0)
	{
		do
		{
			m++;
			text++;
		} while (*text
			 && (*text != key
			     || (IsHiBitOn(key) && *(text+1) != key2 )
			     ||  IsHIdx(ptext, markidx(m)) == HAN_SECOND));
	}
	if (!*text)
	{
		return MARK_UNSET;
	}
	return m;
}

/* move backward within this line to previous occurrence of key */
MARK	m_Fch(m, cnt, key)
	MARK	m;	/* where to search from */
	long	cnt;
	int	key;	/* what to search for */
{
	REG char	*text;
	int	key2;	/* 2nd byte if HANGUL */

	DEFAULT(1);

	prevfwdfn = m_Fch;
	prevrevfn = m_fch;
	prev_key = key;

	if (samekey)
	{
		key2 = prev_key2;
	}
	else if (IsHiBitOn(key))
	{
		prev_key2 = key2 = getkey(0);
	}
	samekey = 0;

	pfetch(markline(m));
	text = ptext + markidx(m);
	while (cnt-- > 0)
	{
		do
		{
			m--;
			text--;
		} while (text >= ptext
			 && (*text != key
			     || (IsHiBitOn(key) && *(text+1) != key2 )
			     || IsHIdx(ptext, markidx(m)) == HAN_SECOND));
	}
	if (text < ptext)
	{
		return MARK_UNSET;
	}
	return m;
}

/* move forward within this line almost to next occurrence of key */
MARK	m_tch(m, cnt, key)
	MARK	m;	/* where to search from */
	long	cnt;
	int	key;	/* what to search for */
{
	m = m_fch(m, cnt, key);
	if (m != MARK_UNSET)
	{
		if (IsHIdx( ptext, markidx(m - 1) ) == HAN_SECOND)
		{
			m -= 2;
		}
		else
		{
			m--;
		}
	}
	prevfwdfn = m_tch;
	prevrevfn = m_Tch;
	return m;
}

/* move backward within this line almost to previous occurrence of key */
MARK	m_Tch(m, cnt, key)
	MARK	m;	/* where to search from */
	long	cnt;
	int	key;	/* what to search for */
{
	m = m_Fch(m, cnt, key);
	if (m != MARK_UNSET)
	{
		if (IsHIdx( ptext, markidx(m)) == HAN_SECOND)
		{
			m += 2;
		}
		else
		{
			m++;
		}
	}
	prevfwdfn = m_Tch;
	prevrevfn = m_tch;

	return m;
}
#endif
