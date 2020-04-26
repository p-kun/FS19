/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iHistory.cpp
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
#include <windows.h>
#include <shlwapi.h>
#include <TCHAR.H>
#include <malloc.h>
#include "iHistory.h"

/* Private */

int           iHistory::mCount = 0;
int           iHistory::mHistMark[ iHistory::HISTORY_DEPTH + 1 ];
TCHAR         iHistory::mHistBuf [ iHistory::HISTORY_DEPTH + 1 ][ MAX_PATH ];
unsigned int  iHistory::mHistDate[ HISTORY_DEPTH + 1 ];


/* DEFINE */

#define HISTORY_DEPTH     300

typedef union __HISTORY_DATE_TIME_T__ {
  unsigned int  dat;
  struct {
  unsigned int  s  : 6;
  unsigned int  m  : 6;
  unsigned int  h  : 5;
  unsigned int  dd : 5;
  unsigned int  mm : 4;
  unsigned int  yy : 6;
  }
  dt;
}
HISTORY_DATE_TIME;

struct __HISTORY_DATA_T__ {
  int   mark;
  TCHAR path[ MAX_PATH * 3 / 2 + HISTORY_PATH_OFFSET + 1 ];
  DWORD date;
}
_hisdats[ HISTORY_DEPTH + 1 ];

typedef struct __HISTORY_DATA_T__ H_DATA;

#define LARGE_INT LARGE_INTEGER
#define ULARGE_INT  ULARGE_INTEGER


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iHistory::iHistory( const TCHAR *path )
{
  if ( path )
    {
      Push( path );
    }
  else
    {
      Clean();
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iHistory::~iHistory()
{
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iHistory::Push( const TCHAR *path, int mark, DWORD date )
{
  int     i;
  int     j;
  int     idx;
  TCHAR*  buf;
  H_DATA  tmp;
  size_t  len;

  if ( PathIsRoot( path ) )
    {
      return;
    }
  
  if ( date == 0 )
    {
      date = GetLocalDateTime();
    }

  len = _tcslen( path ) + 1;

  buf = ( TCHAR* )alloca( len * sizeof( TCHAR ) );

  _tcscpy_s( buf, len, path );

  PathRemoveBackslash( buf );

  for ( i = 0; i < mCount; i++ )
    {
      if (_tcsicmp(_hisdats[ i ].path + HISTORY_PATH_OFFSET, buf ) != 0)
        {
          continue;
        }

      mCount--;

      if ( mark == 0 )
        {
          mark = _hisdats[ i ].mark;
        }

      for ( ; i < mCount; i++ )
        {
          _hisdats[ i ] = _hisdats[ i + 1 ];
        }

      break;
    }

  for ( i = HISTORY_DEPTH - 1; i > 0; i-- )
    {
      _hisdats[ i ] = _hisdats[ i - 1 ];
    }

  _stprintf_s(_hisdats[ 0 ].path, MAX_PATH, L"%c%08x %s", ( mark ? '*' : ' ' ), date, buf);
  _hisdats[ 0 ].date = date;
  _hisdats[ 0 ].mark = mark;

  if ( mCount < HISTORY_DEPTH )
    {
      mCount++;
    }

  for ( i = 1, idx = 0; i < mCount; i++ )
    {
      if ( _hisdats[ i ].mark )
        {
          tmp = _hisdats[ i ];

          for ( j = 0; j < idx; j++ )
            {
              _hisdats[ i - j ] = _hisdats[ i - j - 1 ];
            }

          _hisdats[ i - j ] = tmp;
        }
      else
        {
          idx++;
        }
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iHistory::GetCount( void )
{
  return mCount;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
TCHAR *iHistory::GetAt( int no )
{
  return _hisdats[ no ].path;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iHistory::GetMark( int no )
{
  return _hisdats[ no ].mark;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iHistory::Clean( void )
{
  mCount = 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iHistory::SetMark( const TCHAR *path )
{
  int   i;
  
  for ( i = 0; i < mCount; i++ )
    {
      if ( _tcsicmp( _hisdats[ i ].path + HISTORY_PATH_OFFSET, path ) != 0 )
        {
          continue;
        }

      if ( _hisdats[ i ].mark )
        {
          _hisdats[ i ].mark = 0;
          _hisdats[ i ].path[ 0 ] = _T( ' ' );
        }
      else
        {
          _hisdats[ i ].mark = 1;
          _hisdats[ i ].path[ 0 ] = _T( '*' );
        }

      break;
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iHistory::DeleteLog( const TCHAR *path )
{
  for ( int i = 0; i < mCount; i++ )
    {
      if ( _tcsicmp( mHistBuf[ i ] + 1, path ) != 0 )
        {
          continue;
        }

      mCount--;

      for ( ; i < mCount; i++ )
        {
          _tcscpy_s(mHistBuf[ i ], MAX_PATH, mHistBuf[ i + 1 ]);
        }

      break;
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
DWORD iHistory::GetLocalDateTime( void )
{
  SYSTEMTIME  tm;
  ULARGE_INT  ttt;

  ::GetLocalTime( &tm );

  SystemTimeToFileTime( &tm, ( LPFILETIME )&ttt );

  ttt.QuadPart /= 10000000;

  return ( DWORD )ttt.QuadPart;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iHistory::CmpLocalDateTime( const TCHAR *path, DWORD date_time, TCHAR *date_str, size_t len )
{
  if ( date_time == 0 )
    {
      _stprintf_s( date_str, len, L"?" );
      return;
    }

  date_time -= _tcstoul( path + 1, NULL, 16 );

  if ( date_time == 0 )
    {
      _stprintf_s( date_str, len, L"now" );
    }
  else if ( date_time < 60 )
    {
      if (date_time == 1)
        {
          _stprintf_s( date_str, len, L"%d second ago", date_time );
        }
      else
        {
          _stprintf_s( date_str, len, L"%d seconds ago", date_time );
        }
    }
  else if ( date_time < 3600 )
    {
      date_time /= 60;

      if (date_time == 1)
        {
          _stprintf_s( date_str, len, L"%d minute ago", date_time );
        }
      else
        {
          _stprintf_s( date_str, len, L"%d minutes ago", date_time );
        }
    }
  else if ( date_time < 86400 )
    {
      date_time /= 3600;

      if (date_time == 1)
        {
          _stprintf_s( date_str, len, L"%d hour ago", date_time );
        }
      else
        {
          _stprintf_s( date_str, len, L"%d hours ago", date_time );
        }
    }
  else if ( date_time < 2592000 )
    {
      date_time /= 86400;

      if (date_time == 1)
        {
          _stprintf_s( date_str, len, L"%d day ago", date_time );
        }
      else
        {
          _stprintf_s( date_str, len, L"%d days ago", date_time );
        }
    }
  else
    {
      _stprintf_s( date_str, len, L"More than a month ago" );
    }
}
// --------------------------------------------------------------------------
