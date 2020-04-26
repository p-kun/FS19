/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: strverscmp.cpp
 *  Created  : 11/02/23(水) 09:46:44
 *
 *  Function
 *    String comparison considering version
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/07/27 10:32:48 $
 *  $Author: poko $
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 */
#include <ctype.h>
#include <tchar.h>


/* The operation of this function is as follows.
 * Returns 0 if both strings are equal.
 * Otherwise, look for a byte boundary where both strings are equal until just before,
 * and there is a difference between them in the immediately following byte.
 * Perform the longest match search for a numeric string (a string consisting of numbers only)
 * that includes the found byte boundary.
 * (The number sequence may start at the boundary or end at the boundary.)
 * If one or both of the numbers obtained from two strings are empty,
 * the result returned by strcmp () is returned as the return value of the function.
 * That is, the result of comparing byte values is returned.
 * Otherwise (both numeric sequences are not empty),
 * compare both numeric sequences in numerical order.
 * In this case, a numeric string preceded by one or
 * more 0's is interpreted as having a decimal point in front.
 * (Number strings with many leading zeros will come before)
 * As a result, the order is as follows: 000 , 00 , 01 , 010 , 09 , 0 , 1 , 9 , 10
 */
static const TCHAR *suutinukidasi( const TCHAR *str, int *moji, int *value, int *p_denominator )
{
  int   denominator = 0;
  int   sign        = 1;

  *moji = *str;

  if ( *str == '-' )
    {
      sign = -1;
      str++;
    }

  *value = 0;

  if ( *str == '0' )
    {
      str++;
      denominator--;
      while ( '0' <= *str && *str <= '9' )
        {
          *value *= 10;
          *value += ( *str++ - '0' );
          denominator--;
        }
      *value *= sign;
    }
  else if ( *str > '0' && *str <= '9' )
    {
      denominator = 1;

      while ( '0' <= *str && *str <= '9' )
        {
          *value *= 10;
          *value += ( *str++ - '0' );
        }
      *value *= sign;
    }
  else
    {
      str++;
      if ( *moji < 0 )
        {
          *moji <<= 8;
          *moji |= *str++;
          *moji &= 0xFFFF;
        }
      else
        {
          *moji = toupper( *moji );
        }
    }

  *p_denominator = denominator;

  return str;
}


static int sub( int param1, int param2 )
{
  int   res = 0;

  if ( param1 > param2 )
    {
      res = 1;
    }
  else if ( param1 < param2 )
    {
      res = -1;
    }

  return res;
}


int strverscmp( const TCHAR* str1, const TCHAR* str2 )
{
  int           res = 0;
  int           moji1;
  int           value1;
  int           denom1;
  int           moji2;
  int           value2;
  int           denom2;
  const TCHAR*  cp1 = str1;
  const TCHAR*  cp2 = str2;

  while ( 1 )
    {
      if ( *cp1 == '\0' || *cp2 == '\0' )
        {
          if ( *cp1 != '\0' )
            {
              res = 1;
            }
          else if ( *cp2 != '\0' )
            {
              res = -1;
            }
          break;
        }

      cp1 = suutinukidasi( cp1, &moji1, &value1, &denom1 );
      cp2 = suutinukidasi( cp2, &moji2, &value2, &denom2 );

      if ( denom1 && denom2 )
        {
          if ( ( res = sub( denom1, denom2 ) ) != 0 )
            {
              break;
            }
          if( ( res = sub( value1, value2 ) ) != 0 )
            {
              break;
            }
        }
      else
        {
          if( ( res = sub( moji1, moji2 ) ) != 0 )
            {
              break;
            }
        }
    }
  return res;
}
