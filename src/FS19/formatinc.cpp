/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: formatinc.cpp
 *  Created  : 08/06/04(水) 08:28:31
 *
 *  Function
 *    
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/07/27 10:32:49 $
 *  $Author: poko $
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 */
#include "_box.h"
#include "dirent.h"
#include "SubFuncs.h"
#include "logf.h"
#include "iPt.h"
#include "formatinc.h"

#define IS_KANJI1ST( c )  ( ( unsigned int ) (  ( unsigned char )( c ) ^ 0x20 ) - 0xa1 < 0x3c )
#define EXT_MAX_WIDTH     5


// ==========================================================================
// [R] 0: ファイル名のみでサイズが指定したlenよりも小さくなります
//     1: ファイル名のみ
//     2: サイズ 3:サイズ+日付 4:サイズ+日付+時間 5:サイズ+日付+時間+属性
// --------------------------------------------------------------------------
int CheckNewStringLen( int len, int *max )
{
  int   len_cnt[] = { 0, 7, 9, 6, 6 };
  int   i;

  for ( i = 0; i < 5; i++ )
    {
      if ( len > *max - len_cnt[ i ] )
        {
          break;
        }

      *max -= len_cnt[ i ];
    }

  return i;
}
// --------------------------------------------------------------------------


// ==========================================================================
// [R] 0: ファイル名のみでサイズが指定したlenよりも小さくなります
//     1: ファイル名のみ
//     2: サイズ 3:サイズ+日付 4:サイズ+日付+時間 5:サイズ+日付+時間+属性
// --------------------------------------------------------------------------
int CheckNewStringLen( int len, int *max, int offset )
{
  int   len_cnt[] = { 0, 7, -1, 9, 6, 6 };
  int   i;

  len_cnt[ 2 ] = offset;

  for ( i = 0; i < 6; i++ )
    {
      if ( len > *max - len_cnt[ i ] )
        {
          break;
        }

      *max -= len_cnt[ i ];
    }

  *max += offset;

  return i;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- CheckFileSize
// --------------------------------------------------------------------------
static TCHAR *CheckFileSize( TCHAR *cp, DWORD hi, DWORD lo )
{
  static TCHAR  hs[]  = _T( "BKMGT" );
  static TCHAR  ss1[] = _T( " 123456789" );
  static TCHAR  ss2[] = _T( "0123456789" );

  int           i = 0;
  DWORD         poi;
  DWORD         mod;
  LARGE_INTEGER tmp;

  tmp.HighPart  = hi;
  tmp.LowPart   = lo;
  tmp.QuadPart *= 10;

  while ( tmp.QuadPart >= 10000 )
    {
      tmp.QuadPart /= 1024;
      i++;
    }

  poi = ( DWORD )( tmp.LowPart / 10 );
  mod = ( DWORD )( tmp.LowPart % 10 );

  if ( i == 0 )
    {
      *cp++ = ' ';
      *cp++ = ' ';
    }

  *cp++ = ' ';

  if ( poi < 10 )
    {
      *cp++ = ' ';
      *cp++ = ' ';
      *cp++ = ss2[ poi ];
    }
  else if ( poi < 100 )
    {
      *cp++ = ' ';
      *cp++ = ss1[ poi / 10 ];
      *cp++ = ss2[ poi % 10 ];
    }
  else if ( poi < 1000 )
    {
      *cp++ = ss1[ poi / 100 ];
      *cp++ = ss2[ ( poi /  10 ) % 10 ];
      *cp++ = ss2[ poi %  10 ];
    }

  if ( i != 0 )
    {
      *cp++ = '.';
      *cp++ = ss2[ mod ];
    }

  *cp++ = hs[ i ];

  return cp;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- CheckFileData
// --------------------------------------------------------------------------
static TCHAR *CheckFileData( TCHAR *cp, int dat, int flag = 0 )
{
  static TCHAR  ss[] = _T( "0123456789" );

  if ( dat < 10 )
    {
      if ( flag )
        {
          *cp++ = ' ';
        }
      else
        {
          *cp++ = '0';
        }
      *cp++ = ss[ dat ];
    }
  else if ( dat < 100 )
    {
      *cp++ = ss[ dat / 10 ];
      *cp++ = ss[ dat % 10 ];
    }
  else
    {
      *cp++ = '#';
      *cp++ = '#';
    }

  return cp;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- CheckFileAttr
// --------------------------------------------------------------------------
static TCHAR *CheckFileAttr( TCHAR *cp, DWORD attr )
{
  DWORD attrs[] = {
    0,
    FILE_ATTRIBUTE_SYSTEM,
    FILE_ATTRIBUTE_HIDDEN,
    FILE_ATTRIBUTE_READONLY,
    FILE_ATTRIBUTE_ARCHIVE,
    FILE_ATTRIBUTE_DIRECTORY
  };
  TCHAR marks[] = _T( " SHRAD" );
  int   i;

  for ( i = 0; i < 6; i++ )
    {
      if ( attrs[ i ] == 0 )
        {
          *cp++ = marks[ i ];
        }
      else if ( attr & attrs[ i ] )
        {
          *cp++ = marks[ i ];
        }
      else
        {
          *cp++ = '-';
        }
    }

  return cp;
}
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
static int StrLen( TCHAR* src )
{
  TCHAR*  src1;
  int     len = 0;

  for ( ;; )
    {
      src1 = CharNext( src );

      if ( src1 == src )
        {
          break;
        }

      src = src1;
      len++;
    }

  return len;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int strlenw( TCHAR *str, int width, int *o_w, int *o_t )
{
  int   len;
  int   lend;
  int   cnt;
  int   i;
  int   total;
  int   res = 0;
  WORD* types;

  total = StrLen( str );

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
TCHAR *CheckFileName( TCHAR *cp, TCHAR *src, int len )
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
// -- CheckFileName
// --------------------------------------------------------------------------
TCHAR *CheckFileName( TCHAR *cp, TCHAR *src, int cnt, int len )
{
  TCHAR*  src1;
  int     i;
  TCHAR   b;
  int     w;
  int     t;

  if ( strlenw( src, cnt, &w, &t ) )
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
// -- SplitPath
// --------------------------------------------------------------------------
void SplitPath( TCHAR *d_name, TCHAR *name, TCHAR *ext, TCHAR *dir = NULL )
{
  TCHAR drv[ _MAX_DRIVE ];
  TCHAR tmp[ _MAX_DIR ];

  if ( dir )
    {
      _tsplitpath_s( d_name,
                     drv,  _MAX_DRIVE,
                     dir,  _MAX_DIR,
                     name, _MAX_FNAME,
                     ext,  _MAX_EXT );
    }
  else
    {
      _tsplitpath_s( d_name,
                     drv,  _MAX_DRIVE,
                     tmp,  _MAX_DIR,
                     name, _MAX_FNAME,
                     ext,  _MAX_EXT );
    }

  if ( name[ 0 ] == '\0' )
    {
      _tcscpy_s( name, _MAX_FNAME, ext );
      ext[ 0 ] = '\0';
    }

  if ( _tcscmp( name, _T( "." ) ) == 0 && _tcscmp( ext, _T( "." ) ) == 0 )
    {
      _tcscpy_s( name, _MAX_FNAME, _T( ".." ) );
      ext[ 0 ] = '\0';
    }

  if ( *dir == '\0' )
    {
      _tcscpy_s( dir, _MAX_EXT, _T( "." ) );
    }
  else
    {
      StrTrimRight( dir, _T( "\\" ) );
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 属性別の色を取得する
// --------------------------------------------------------------------------
DWORD FormatColor( DWORD attr )
{
  DWORD color;
  iPt   pt;

  if ( attr & FILE_ATTRIBUTE_SYSTEM )
    {
      color = pt.COL_FILE_ATTR_SYSTEM;
    }
  else if ( attr & FILE_ATTRIBUTE_HIDDEN )
    {
      color = pt.COL_FILE_ATTR_HIDDEN;
    }
  else if ( attr & FILE_ATTRIBUTE_READONLY )
    {
      color = pt.COL_FILE_ATTR_READONLY;
    }
  else if ( attr & FILE_ATTRIBUTE_DIRECTORY )
    {
      color = pt.COL_FILE_ATTR_DIRECTORY;
    }
  else
    {
      /* FILE_ATTRIBUTE_NORMAL */

      color = pt.COL_FILE_ATTR_NORMAL;
    }

  return color;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 属性別の色を取得する
// --------------------------------------------------------------------------
DWORD FormatColor( dirent *entry )
{
  return FormatColor( entry->dwFileAttributes );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- FormatInc
// --------------------------------------------------------------------------
int FormatInc( TCHAR buf[], dirent *entry, int max_len, int str_len, int *str_offset )
{
  TCHAR*      cp;
  TCHAR*      ext;
  TCHAR*      path;
  FILETIME    local;
  SYSTEMTIME  st;
  int         res;
  int         len;
  TCHAR      *name;

  FileTimeToLocalFileTime( &entry->ftLastWriteTime, &local );
  FileTimeToSystemTime( &local, &st );

  len = (int)_tcslen(entry->d_name) + 1;

  name = (TCHAR *)alloca(len * sizeof(TCHAR));

  _tcscpy_s( name, len, entry->d_name );

  path = PathFindFileName( name );
  ext  = PathFindExtension( path );

  if ( path == ext || _tcscmp( path, _T( ".." ) ) == 0 )
    {
      ext += _tcslen( ext );
    }

  res = CheckNewStringLen( str_len, &max_len );

  do
    {
      cp = buf;

      len = ( int )_tcslen( ext );

      if ( len == 0 || len > EXT_MAX_WIDTH || max_len <= 8 )
        {
          cp = CheckFileName( cp, path, max_len );
        }
      else
        {
          ext[ 0 ] = '\0';
          cp = CheckFileName( cp, path, max_len - EXT_MAX_WIDTH );
          ext[ 0 ] = ' ';
          cp = CheckFileName( cp,  ext,  EXT_MAX_WIDTH );
        }

      *str_offset = ( int )( cp - buf );

      if ( res < 2 )
        {
          break;
        }

      if ( entry->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
          *cp++ = ' '; *cp++ = ' ';
          *cp++ = '<'; *cp++ = 'D';
          *cp++ = 'I'; *cp++ = 'R';
          *cp++ = '>';
        }
      else
        {
          cp = CheckFileSize( cp, entry->nFileSizeHigh, entry->nFileSizeLow );
        }

      if ( res < 3 )
        {
          break;
        }

      *cp++ = ' ';
      cp = CheckFileData( cp, st.wYear % 100 );
      *cp++ = '-';
      cp = CheckFileData( cp, st.wMonth );
      *cp++ = '-';
      cp = CheckFileData( cp, st.wDay );

      if ( res < 4 )
        {
          break;
        }

      *cp++ = ' ';
      cp = CheckFileData( cp, st.wHour, 1 );
      *cp++ = ':';
      cp = CheckFileData( cp, st.wMinute );

      if ( res < 5 )
        {
          break;
        }

      cp = CheckFileAttr( cp, entry->dwFileAttributes );
    }
  while( 0 );

  *cp = '\0';

  return max_len;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- FormatInc2
// --------------------------------------------------------------------------
int FormatInc( TCHAR buf[], dirent *entry, int offset, int max_len, int str_len, int lens[] )
{
  TCHAR*      cp;
  TCHAR       dir[ MAX_PATH ];
  TCHAR       dir2[ MAX_PATH ];
  TCHAR*      ext;
  TCHAR*      tmp;
  TCHAR*      path;
  FILETIME    local;
  SYSTEMTIME  st;
  int         res;
  int         len;
  int         i;
  int         dir_len;
  TCHAR       name[ MAX_PATH ];

  _tcscpy_s( name, MAX_PATH, entry->d_name );

  FileTimeToLocalFileTime( &entry->ftLastWriteTime, &local );
  FileTimeToSystemTime( &local, &st );

  tmp  = name + offset;
  path = PathFindFileName( tmp );
  ext  = PathFindExtension( path );

  if ( path == ext || _tcscmp( path,_T( ".." ) ) == 0 )
    {
      ext += _tcslen( ext );
    }

  _tcsncpy_s( dir, MAX_PATH, tmp, path - tmp );

  dir[ path - tmp ] = '\0';
  StrTrimRight( dir, _T( "\\" ) );

  res = CheckNewStringLen( str_len, &max_len );

  dir_len = max_len * 1 / 2;
  max_len = max_len - dir_len;

  if ( *dir == '\0' )
    {
      _tcscpy_s( dir2, MAX_PATH, _T( " ." ) );
    }
  else
    {
      dir2[ 0 ] = ' ';
      _tcsncpy_s( dir2 + 1, MAX_PATH - 1, dir, dir_len - 2 );
    }

  for ( i = ( int )_tcslen( dir2 ); i < dir_len - 2; i++ )
    {
      dir2[ i ] = ' ';
    }

  dir2[ i++ ] = ' ';
  dir2[ i   ] = '\0';

  do
    {
      len = ( int )_tcslen( ext );

      if ( len == 0 || len > EXT_MAX_WIDTH || max_len <= 8 )
        {
          cp = CheckFileName( buf, path, max_len );
        }
      else
        {
          ext[ 0 ] = '\0';
          cp = CheckFileName( buf, path, max_len - EXT_MAX_WIDTH );
          ext[ 0 ] = ' ';
          cp = CheckFileName( cp,  ext,  EXT_MAX_WIDTH );
        }

      lens[ 0 ] = max_len;

      if ( res >= 2 )
        {
          if ( entry->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
            {
              *cp++ = ' '; *cp++ = ' ';
              *cp++ = '<'; *cp++ = 'D';
              *cp++ = 'I'; *cp++ = 'R';
              *cp++ = '>';
            }
          else
            {
              cp = CheckFileSize( cp, entry->nFileSizeHigh, entry->nFileSizeLow );
            }
          lens[ 1 ] = ( int )( cp - buf ) - lens[ 0 ];
        }
      else
        {
          lens[ 1 ] = 0;
        }

      *cp++ = ' ';
      cp = CheckFileName( cp, dir2, dir_len - 1 );

      lens[ 2 ] = ( int )( cp - buf ) - ( lens[ 0 ] + lens[ 1 ] );
      lens[ 3 ] = 0;

      if ( res < 3 )
        {
          break;
        }

      *cp++ = ' ';
      cp = CheckFileData( cp, st.wYear % 100 );
      *cp++ = '-';
      cp = CheckFileData( cp, st.wMonth );
      *cp++ = '-';
      cp = CheckFileData( cp, st.wDay );

      lens[ 3 ] = ( int )( cp - buf ) - ( lens[ 0 ] + lens[ 1 ] + lens[ 2 ] );

      if ( res < 4 )
        {
          break;
        }

      *cp++ = ' ';
      cp = CheckFileData( cp, st.wHour, 1 );
      *cp++ = ':';
      cp = CheckFileData( cp, st.wMinute );

      lens[ 3 ] = ( int )( cp - buf ) - ( lens[ 0 ] + lens[ 1 ] + lens[ 2 ] );

      if ( res < 5 )
        {
          break;
        }

      cp = CheckFileAttr( cp, entry->dwFileAttributes );

      lens[ 3 ] = ( int )( cp - buf ) - ( lens[ 0 ] + lens[ 1 ] + lens[ 2 ] );
    }
  while( 0 );

  *cp = '\0';

  return max_len;
}
// --------------------------------------------------------------------------
