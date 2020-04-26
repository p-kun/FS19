/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: ssnprintf.cpp
 *  Created  : 08/06/12(木) 17:56:03
 *
 *  Function
 *    Printf function
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 *  $Revision: 1.15 $
 *  $Date: 2008/10/08 06:53:28 $
 *  $Author: uj03760 $
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 */
#include <stdio.h>
#include <windows.h>
#include <stdarg.h>
#include <assert.h>
#include <malloc.h>
#include <tchar.h>
#include "ssnprintf.h"

/* Private */

#ifdef _DEBUG
#define CHECK_BUFFER( cur, top )  if( (cur) < (top) ) { assert( 0 ); }
#else
#define CHECK_BUFFER( cur, top )
#endif


// ==========================================================================
// --
// --------------------------------------------------------------------------
static int strlenw( const TCHAR *str, int width, int *o_w, int *o_t )
{
  int   len;
  int   lend;
  int   cnt;
  int   i;
  int   total;
  int   res = 0;
  WORD* types;

  total = ( int )_tcslen( str );

  types = ( WORD* )alloca( sizeof( WORD ) * ( total + 1 ) );

  GetStringTypeEx( LOCALE_SYSTEM_DEFAULT, CT_CTYPE3, str, -1, types );

  for ( lend = 0, len = 0, cnt = 0, i = 0; i < total; i++ )
    {
      if ( ( types[ i ] & C3_HALFWIDTH ) )
        {
          len++;
        }
      else
        {
          len += 2;
        }

      if ( width - 3 >= len )
        {
          cnt++;
          lend = len;
        }
      else if ( width < len )
        {
          i   = cnt;
          len = lend;
          res = 1;
          break;
        }
    }

  *o_w = len;
  *o_t = i;

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- CheckFileName
// --------------------------------------------------------------------------
TCHAR *strncpy_sc( TCHAR *cp, const TCHAR *src, int len )
{
  TCHAR*  src1;
  int     i;
  TCHAR   b;
  int     w;
  int     t;

  if ( strlenw( src, len, &w, &t ) )
    {
      b = _T( '.' );
    }
  else
    {
      b = _T( ' ' );
    }

  for ( i = 0; i < t; i++ )
    {
      src1 = CharNext( src );
      while ( src1 > src )
        {
          *cp++ = *src++;
        }
    }

  for ( i = 0; i < 3 && w < len; w++, i++ )
    {
      *cp++ = b;
    }

  for ( ; w < len; w++ )
    {
      *cp++ = _T( ' ' );
    }

  *cp = '\0';

  return cp;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- Copy string
// [R] Next buffer position (returns the position of '0' after copying)
// --------------------------------------------------------------------------
TCHAR *strncpy_sc(
    TCHAR*        buf,    // Destination buffer location
    int           max,    // Capacity of the above buffer
    const TCHAR*  src,    // Source string
    int           len,    // Maximum number of copies
    int           left    // 1: Left justified
  )
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
{
  if ( len == 0 )
    {
      while ( *src )
        {
          *buf++ = *src++;
        }
      *buf = '\0';
    }
  else
    {
      buf = strncpy_sc( buf, src, len );
    }

  return buf;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- Convert from number to string
// [R] Next buffer position (returns the position of '0' after copying)
// --------------------------------------------------------------------------
TCHAR *int2str(
    TCHAR*    buf,        // Buffer position
    int       max,        // Buffer capacity
    int       len,        // Number of digits
    long long val,        // Target to convert
    int       base,       // radix
    int       is_big,     // Uppercase display (hexadecimal)
    TCHAR     space,      // Characters to fill with digits
    int       interv,     // , Strike interval
    int       is_left,    // Store left justified
    int       sign        // With sign
  )
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
{
  static TCHAR        tipx[] = _T( "0123456789abcdef0123456789ABCDEF" );
  TCHAR*              tip;
  TCHAR*              ret;
  unsigned long long  tmp;
  int                 l = 0;
  int                 i = 0;
#ifdef _DEBUG
  TCHAR*              top = buf;
#endif

  /* Processing with sign! */

  if ( sign )
    {
      /* With a sign! */

      if ( val < 0 )
        {
          val = -val;
          l++;
          sign = 1;
        }
      else
        {
          sign = 0;
        }
    }

  len++;

  /* Limit the interval to hit ',' */

  if ( interv < 1 )
    {
      /* Only when there are two or more ',' intervals! */

      interv = 10000;
    }

  /* Calculate number of digits */

  tmp = val;

  do
    {
      tmp /= base;
      l++;
    }
  while ( tmp > 0 );

  /* Calculate interval for hitting ',' */

  l += ( ( l - 1 - sign ) / interv );

  /* When no digit is specified */

  if ( len <= 1 )
    {
      len = l + 1;
    }

  if ( len > max )
    {
      len = max;
    }

  if ( len > 0 )
    {
      len--;                      // Consider '\ 0'

      ret = buf + len;

      /* If there are more digits than the buffer, fill with '#' */

      if ( len < l )
        {
          space = '#';

          l = 0;
          buf += len;

          sign = 0;               // No sign!
        }
      else
        {
          /* Left-justified and right-justified processing */

          if ( is_left )
            {
              /* Left justified! */

              buf += l;           // Use buffer only for the number of digits

              /* Fill the right part too much */

              for ( i = 0; len > l; len--, i++ )
                {
                  CHECK_BUFFER( ( buf + i ), top );
                  *( buf + i ) = space;
                }
            }
          else
            {
              /* Right justified! */

              buf += len;         // Use full buffer
            }
          len -= l;               // Calculation of unused buffers
        }
    }
  else
    {
      buf += l;                   // Use buffer only for the number of digits
      len = 0;
      
      ret = buf;
    }

  /* Converts numbers to strings */

  CHECK_BUFFER( buf, top );
  *ret = '\0';
  buf--;

  tip = tipx + ( is_big > 0 ) * 16;

  tmp = val;

  i = 0;

  while ( l > sign )
    {
      if ( i >= interv )
        {
          /* Interval to hit ',' */

          CHECK_BUFFER( buf, top );
          *buf-- = ',';
          i = 0;
        }
      else
        {
          CHECK_BUFFER( buf, top );
          *buf-- = *( tip + ( tmp % base ) );
          tmp /= base;
          i++;
        }
      l--;
    }

  /* Fill remaining buffer with specified character */

  if ( space == '0' )
    {
      while ( len-- > 0 )
        {
          if( i >= interv )
            {
              /* Interval to hit ',' */

              CHECK_BUFFER( buf, top );
              *buf-- = ',';
              i = 0;
            }
          else
            {
              CHECK_BUFFER( buf, top );
              *buf-- = space;
              i++;
            }
        }

      /* When the value is-when signed, minus */

      if ( sign )
        {
          CHECK_BUFFER( buf, top );
          *buf-- = '-';
        }
    }
  else
    {
      /* When the value is-when signed, minus */

      if ( sign )
        {
          CHECK_BUFFER( buf, top );
          *buf-- = '-';
        }

      while ( len-- > 0 )
        {
          CHECK_BUFFER( buf, top );
          *buf-- = space;
        }
    }

  return ret;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- Simulating vsnprintf
// --------------------------------------------------------------------------
int vssnprintf(
  TCHAR*        buf,    // Destination buffer location
  int           max,    // Capacity of the above buffer
  const TCHAR*  fmt,    // Format string
  va_list       ap
  )
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
{
  TCHAR     c;
  TCHAR*    cpTop = buf;
  TCHAR*    cpEnd;
  TCHAR     space;
  int       maxx;
  int       left;
  int       tokn;
  int       base;
  int       sign;
  int       xbig;
  long long l;

  max--;
  cpEnd = buf + max;

  while ( ( c = *fmt++ ) != NULL && cpEnd > buf )
    {
      if ( c != '%' )
        {
          *buf++ = c;
          continue;
        }
      maxx  = 0;
      left  = 0;
      tokn  = 0;
      sign  = 0;
      xbig  = 0;
      base  = 10;
      space = ' ';

      while ( ( c = *fmt++ ) != NULL )
        {
          if ( c == '-' )
            {
              left = 1;
              continue;
            }
          else if ( c == ',' )
            {
              tokn = 4;
              continue;
            }
          else if ( c >= '0' && c <= '9' )
            {
              if( c == '0' && maxx == 0 )
                {
                  space = '0';
                }
              else
                {
                  maxx *= 10;
                  maxx += ( c - '0' );
                }
              continue;
            }

          max = ( int )( cpEnd - buf + 1 );

          if ( c == 'd' )
            {
              l = va_arg( ap, int );
              
              tokn = tokn ? 3 : 0;
              sign = 1;
            }
          else if ( c == 'u' )
            {
              l = va_arg( ap, unsigned int );

              tokn = tokn ? 3 : 0;
            }
          else if ( c == 'x' )
            {
              l = va_arg( ap, unsigned int );
              
              base = 16;
            }
          else if ( c == 'X' )
            {
              l = va_arg( ap, unsigned int );

              base = 16;
              xbig = 1;
            }
          else if ( c == 'w' )
            {
              l = va_arg( ap, unsigned long long );

              base = 16;
            }
          else if ( c == 'W' )
            {
              l = va_arg( ap, unsigned long long );

              base = 16;
              xbig = 1;
            }
          else if ( c == 'D' )
            {
              l = va_arg( ap, long long );

              tokn = tokn ? 3 : 0;
              sign = 1;
            }
          else if ( c == 'U' )
            {
              l = va_arg( ap, unsigned long long );

              tokn = tokn ? 3 : 0;
            }
          else if ( c == 'o' )
            {
              l = va_arg( ap, unsigned int );

              base = 8;
            }
          else if ( c == 'b' )
            {
              l = va_arg( ap, unsigned int );

              base = 2;
            }
          else if ( c == 's' )
            {
              buf = strncpy_sc( buf, max, va_arg( ap, TCHAR* ), maxx, left );
              break;
            }
          else if ( c == 'c' )
            {
              *buf++ = ( TCHAR )va_arg( ap, int );
              break;
            }
          else
            {
              *buf++ = c;
              break;
            }

          buf = int2str( buf, max, maxx, l, base, xbig, space, tokn, left, sign );

          break;
        }
    }

  *buf = '\0';

  return ( int )( buf - cpTop );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- snprintf simulation
// --------------------------------------------------------------------------
int ssnprintf(
  TCHAR*        buf,    // Destination buffer location
  int           max,    // Capacity of the above buffer
  const TCHAR*  fmt,    // Format string
  ...                   // Variable arguments
  )
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
{
  va_list   ap;
  int       len;

  va_start( ap, fmt );

  len = vssnprintf( buf, max, fmt, ap );

  va_end( ap );

  return len;
}
// --------------------------------------------------------------------------
