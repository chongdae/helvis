/* hangul.c */

/* Author:
 *      Park, Chong-dae
 *      KAIST, Korea
 *      cdpark@baram.kaist.ac.kr
 */


/*
 * 0 : ASCII
 * 1 : First byte of hangul
 * 2 : second byte of hangul
 */

#include "config.h"
#include "vi.h"

int	IsHIdx(text, index)
	char	*text;
	int	index;
{
	int	i;
	register int state = 0;

	if (index < 0 || index >= strlen(text))
		return 0;

	for(; index >= 0 && *text; text++, index--)
	{
		if (state == 1)
			state = 2;
		else if ( IsHiBitOn(*text) )
			state = 1;
		else
			state = 0;
	}

	return state;
}

int	FindNCharToMark( m )
	MARK    m ;
{
	int     idx;
	long    nByte ;
	int     n ;

	idx = markidx( m );
	pfetch(markline(m));

	/* Find actual num of char from beginning to mark m */
	for ( n = 0, nByte = 0 ; nByte < idx ; n++ )
	{
		if ( IsHiBitOn( ptext[nByte] ) )
		{
			nByte += 2 ;
		}
		else
		{
			nByte ++ ;
		}
	}
	return n ;
}

int	FindNBytesOfP2CharFromMarkP1( m, cnt )
	MARK    m ;
	int     cnt ;
{
	int     idx;
	unsigned char *bp ;
	long    nBytes ;
	int     n ;

	idx = markidx( m );
	pfetch(markline(m));
	bp = (unsigned char *) ptext + idx ;

	/* Find actual no. of bytes to be deleted;
	 * HZ-char is 2 bytes; else 1 byte.
	 */
	nBytes = 0 ;
	for ( n = cnt ; --n >= 0 ; )
	{
		if ( IsHiBitOn( *( bp + nBytes ) ) )
		{
			nBytes += 2 ;
		}
		else
		{
			nBytes ++ ;
		}
		if (nBytes > plen - idx)
			break;
	}
	return nBytes ;
}

int     FindNBytesOfP2CharBackFromMarkP1( m, cnt )
	MARK	m ;
	int	cnt ;
{
	int	idx;
	unsigned char *bp ;
	long	nBytes ;
	int	n ;

	idx = markidx( m );

	/* find actual # of chars from line beginning to mark m */
	n = FindNCharToMark (m) - cnt;
	if ( n < 0 )
		n = 0;

	/* find actual byte # of n chars from line beginning */
	nBytes = FindNBytesOfP2CharFromMarkP1( m - idx, n );

	return (idx - nBytes);
}

int AdjunstHLen(line, llen)
	char	*line;
	int	llen;
{
	int newlen;
	register int state = 0;

	newlen = 0;

	for(; llen >= 0 && *line; line++, llen--)
	{
		if (state == 1)
			state = 2, newlen++;
		else if ( IsHiBitOn(*line) )
			state = 1;
		else
			state = 0, newlen++;
	}

	return newlen;
}
