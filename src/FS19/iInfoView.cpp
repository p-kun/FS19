/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iInfoView.cpp
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
#include "ssnprintf.h"
#include "iIcon.h"
#include "xDcBase.h"
#include "iBaseView.h"
#include "logf.h"
#include "iShortcutView.h"
#include "timerid.h"
#include "ipt.h"
#include "iInfoView.h"

/* 定義値一覧です */

#define OFFSET_1          4
#define OFFSET_2          6
#define OFFSET_H          2
#define OFFSET_T          8
#define OFFSET_B          6
#define OFFSET_LEFT       16
#define SPLIT             1
#define LIMIT_LEN_CAPA    36
#define ICON_SIZE         32
#define OFFSET_LEFT1      ( OFFSET_1 + DPIX(ICON_SIZE) + OFFSET_1 )
#define OFFSET_LEFT2      ( OFFSET_LEFT1 + SPLIT + OFFSET_2 )
#define GETHEIGHT(h)      ( OFFSET_T + OFFSET_TOP + (h) + OFFSET_H + OFFSET_OFFSET2 + (h) + OFFSET_OFFSET2 + OFFSET_BTM + OFFSET_B )
#define GETCAPAWIDTH(w)   ( SPLIT + OFFSET_2 + LIMIT_LEN_CAPA * (w) + OFFSET_1 )
#define TYPE_NAME_LEN     80

#define ICON_KIND_ERR     -1
#define ICON_KIND_FOLDER  -2

#define HENABLE           ((void*)-3)
#define OFFSET_TOP        5
#define OFFSET_BTM        4
#define OFFSET_OFFSET     8
#define OFFSET_OFFSET2    3
#define OFFSET_INTERVAL   4
#define ICON_SIZE16       16

#define WIN10_LINES       1

// ==========================================================================
// --
// --------------------------------------------------------------------------
static DWORD GetLocalTimeD( void )
{
  SYSTEMTIME  sys;

  ::GetLocalTime( &sys );

  return sys.wMinute * 60000 + sys.wSecond * 1000 + sys.wMilliseconds;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iInfoView::iInfoView( int base ) : iBaseView( base )
{
  ZeroMemory( &m_entry, sizeof( m_entry ) );
  ZeroMemory( m_capa, sizeof( m_capa ) );
  ZeroMemory( m_used, sizeof( m_used ) );
  ZeroMemory( m_free, sizeof( m_free ) );
  ZeroMemory( m_info, sizeof( m_info ) );
  ZeroMemory( m_target, sizeof( m_target ) );

  m_mode   = 0;

  m_drive_handle = NULL;
  m_drive_rate   = 0;

  m_prev_id = -1;
  m_action  = -1;

  m_timer_on = 0;

  mCopyEnable = FALSE;
  mPastEnable = FALSE;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iInfoView::Draw( HWND hWnd, HDC hDC, int x, int y, int cx, int cy )
{
  xTmpDC  dc( hDC );
  xBmpDC  bmp;
  iPt     pt;

  mhWnd = hWnd;
  mhDC  = dc;
  m_x   = x;
  m_y   = y;
  m_cx  = cx;
  m_cy  = cy;

  dc.SetSelectFont( m_fontname, m_fontsize );

  SetMiRect( dc, 0, cx, cy );

  bmp.Create( dc, cx + ICON_SIZE, cy );
  bmp.SetSelectFont( pt.fontname4, pt.fontsize4 );
  bmp.FillRect( 0, 0, cx + ICON_SIZE, cy, pt.COL_BASE_BAR );

  /* Upper line  */

#if not WIN10_LINES
  bmp.Line( 1, 0, cx - 1, 0, pt.COL_SPLIT_LINE );
#endif

  /* Right line  */

  bmp.Line( cx - 2, 1, cx - 2, cy - 1, pt.COL_SPLIT_LINE );
  bmp.SetBkMode( TRANSPARENT );

  if ( m_target[ 0 ] != '\0' )
    {
      if ( m_mode )
        {
          DrawMainB( bmp, cx, cy, dc.tm.tmAveCharWidth, dc.tm.tmHeight );
        }
      else
        {
          DrawMainA( bmp, cx, cy, dc.tm.tmAveCharWidth, dc.tm.tmHeight );
        }
    }

  dc.BitBlt( x, y, m_cx, cy, bmp, 0, 0, SRCCOPY );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iInfoView::Redraw( void )
{
  if ( mhDC )
    {
      xDC   dc( mhWnd );
      Draw( mhWnd, dc, m_x, m_y, m_cx, m_cy );
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iInfoView::DrawMainA( HDC hDC, int cx, int cy, int w, int h )
{
  xTmpDC  dc( hDC );
  TCHAR   file_name[ MAX_PATH * 2 ];
  TCHAR   type_name[ TYPE_NAME_LEN ];
  SIZE    size;
  int     capa_w;
  int     top_1 = ( ( cy + 1 ) - h * 2 ) / 4 + 4;
  int     top_2 = top_1 + h + 4;
  iPt     pt;
  HANDLE  handle;
  iico    ico(hDC, IMG_MARK);
  int     idx = -1;

  if ( m_focus )
    {
      dc.SetTextColor( pt.COL_TITLE );
    }
  else
    {
      dc.SetTextColor( Brend( pt.COL_TITLE, 50 ) );
    }

  handle = GetFileIconAndType( m_target, type_name);

  _stprintf_s( file_name, MAX_PATH * 2, L"%s  <%s>", PathFindFileName( m_target ), type_name );

  dc.TextOut( OFFSET_LEFT + OFFSET_LEFT2, top_1, file_name );
  dc.TextOut( OFFSET_LEFT + OFFSET_LEFT2, top_2, m_info );

  capa_w = ( GETCAPAWIDTH( w ) + 2 );

  ::GetTextExtentPoint32( dc, m_drive_name, ( int )_tcslen( m_drive_name ), &size );
  size.cx += ( SPLIT + OFFSET_2 + OFFSET_1 + 2 + ICON_SIZE );

  if ( size.cx > capa_w )
    {
      capa_w = size.cx;
    }

  DrawCapaInfo( dc, cx - capa_w, h, capa_w, cy );

  /* アイコン */

  if (handle)
    {
      DrawIconEx(dc, OFFSET_LEFT + OFFSET_1, ( cy - DPIX(32) ) / 2, (HICON)handle, DPIX(32), DPIX(32), 0, 0, DI_NORMAL);
      DestroyIcon((HICON)handle);

      if (m_entry.bookMark)
        {
          idx = 1;
        }
      else if (m_entry.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
        {
          idx = DISPLAY_SCALE < 150 ? 3 : 8;
        }
      else if ( m_entry.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN )
        {
          idx = DISPLAY_SCALE < 150 ? 2 : 7;
        }
      else if ( m_entry.dwFileAttributes & FILE_ATTRIBUTE_READONLY )
        {
          idx = DISPLAY_SCALE < 150 ? 4 : 9;
        }
    }

  if (idx != -1)
    {
      ico.draw(OFFSET_LEFT + OFFSET_1, OFFSET_2, idx);
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iInfoView::DrawMainB( HDC hDC, int cx, int cy, int w, int h )
{
  int     i;
  int     y;
  int     offset;
  xTmpDC  dc( hDC );
  int     id = m_prev_id;
  int     height;
  SIZE    size;
  int     capa_w;
  BOOL    enable;
  iPt     pt;
  iico    ico(hDC, IMG_INFOVIEW);

  for ( i = 0; i < TB_MAX_LINE && mi_lines[ i ].handle; i++ )
    {
      y = mi_lines[ i ].rc.top;

      if ( mi_lines[ i ].id == ISHORTCUTID_NULL )
        {
          offset = 0;
        }
      else
        {
          if ( ( m_action == -1 && id == i ) || m_action == i )
            {
              iBaseView::FillRect( dc, &mi_lines[ i ].rc, pt.COL_SELECTED, pt.COL_SELECTED_WAKU );

              if ( m_action == i )
                {
                  y += 2;
                }
            }

          offset = OFFSET_OFFSET;
        }

      height = mi_lines[ i ].rc.bottom - mi_lines[ i ].rc.top;

      if ( i == 0 )
        {
          enable = mPastEnable;
        }
      else if ( i == 1 )
        {
          enable = mCopyEnable;
        }
      else
        {
          enable = m_focus;
        }

      if ( enable )
        {
          dc.SetTextColor( 0 );
        }
      else
        {
          dc.SetTextColor( Brend( 0, 50 ) );
        }

      if ( mi_lines[ i ].handle != HENABLE )
        {
          ico.draw(mi_lines[ i ].rc.left + offset, ( height - ICON_SIZE16 ) / 2 + y, mi_lines[ i ].index, enable);
          offset += ( ICON_SIZE16 + OFFSET_INTERVAL );
        }

      if ( mi_lines[ i ].name[ 0 ] )
        {
          dc.TextOut( mi_lines[ i ].rc.left + offset, ( height - dc.tm.tmHeight ) / 2 + y, mi_lines[ i ].name );
        }
    }

  capa_w = ( GETCAPAWIDTH( w ) + 2 );

  ::GetTextExtentPoint32( dc, m_drive_name, ( int )_tcslen( m_drive_name ), &size );
  size.cx += ( SPLIT + OFFSET_2 + OFFSET_1 + 2 + ICON_SIZE );

  if ( size.cx > capa_w )
    {
      capa_w = size.cx;
    }

  if ( i > 0 && mi_lines[ i - 1 ].rc.right + capa_w < cx )
    {
      DrawCapaInfo( dc, cx - capa_w, h, capa_w, cy );
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iInfoView::~iInfoView()
{
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iInfoView::SetElement( HDC hDC, int no, int x, int height, WORD id, const TCHAR *str )
{
  iico    ico(hDC, IMG_INFOVIEW);

  mi_lines[ no ].index  = id;
  mi_lines[ no ].handle = ico;

  if ( str )
    {
      x = SetMiiSubRect( hDC, &mi_lines[ no ], x, height, id, OFFSET_OFFSET, str );
    }
  else
    {
      x = SetMiiSubRect( hDC, &mi_lines[ no ], x, height, id );
    }

  return x;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iInfoView::SetMiRect( HDC hDC, int width, int cx, int cy )
{
  int   x = OFFSET_TOP;
  int   i = 0;

  x = SetElement( hDC, i++, x, cy, ISHORTCUTID_PAST, L"Paste" );
  x = SetElement( hDC, i++, x, cy, ISHORTCUTID_COPY, L"Copy" );
  x = SetElement( hDC, i++, x, cy, ISHORTCUTID_DELETE, L"Erase" );
  x = SetElement( hDC, i++, x, cy, ISHORTCUTID_RENAME, L"Rename" );
  x = SetElement( hDC, i++, x, cy, ISHORTCUTID_NULL );
  x = SetElement( hDC, i++, x, cy, ISHORTCUTID_REFRESH, L"Update" );
  x = SetElement( hDC, i++, x, cy, ISHORTCUTID_PROPERTIES, L"Property" );
  x = SetElement( hDC, i++, x, cy, ISHORTCUTID_NULL );
  x = SetElement( hDC, i++, x, cy, ISHORTCUTID_EJECT, L"Take out" );
  x = SetElement( hDC, i++, x, cy, ISHORTCUTID_NULL );
  x = SetElement( hDC, i++, x, cy, ISHORTCUTID_SETTING, L"Configuration.."  );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iInfoView::TargetSet( dirent *entry, BOOL redraw )
{
  TCHAR buf1[ MAX_PATH ];
  TCHAR buf2[ MAX_PATH ];

  do
    {
      if (entry == NULL)
        {
          break;
        }

      if (entry->d_name[ 0 ] == '\0')
        {
          break;
        }

      if (_tcslen( entry->d_name ) < 3)
        {
          break;
        }

      if ( StrCmp( PathFindFileName( entry->d_name ), _T( ".." ) ) == 0 )
        {
          mCopyEnable = FALSE;
        }
      else
        {
          mCopyEnable = TRUE;
        }

      if ( !IsClipboardFormatAvailable( CF_HDROP ) )
        {
          mPastEnable = FALSE;
        }
      else
        {
          mPastEnable = TRUE;
        }

      _tcscpy_s( buf1, MAX_PATH, entry->d_name );
      _tcscpy_s( buf2, MAX_PATH, m_target );

      if ( _tcsicmp( buf1, buf2 ) == 0 )
        {
          break;
        }

      PathStripToRoot( buf1 );
      PathStripToRoot( buf2 );

      if ( _tcsicmp( buf1, buf2 ) )
        {
          ChangeDrive( buf1 );
        }

      if ( PathIsRoot( entry->d_name ) )
        {
          wsprintf( m_info, _T( "---" ) );
        }
      else
        {
          MakeFileInfo( entry );
        }

      _tcscpy_s( m_target, MAX_PATH, entry->d_name );
      m_entry = *entry;

      if ( redraw )
        {
          Redraw();
        }
    }
  while(0);
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iInfoView::GetHeight( HWND hWnd )
{
  xDC   dc( hWnd );

  return GetHeight( dc );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iInfoView::GetHeight( HDC hDC )
{
  xTmpDC  dc( hDC );

  dc.SetSelectFont( m_fontname, m_fontsize );

  return GETHEIGHT( dc.tm.tmHeight );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- ファイル情報の作成
// --------------------------------------------------------------------------
void iInfoView::MakeFileInfo( dirent *entry )
{
  TCHAR*        cp;
  SYSTEMTIME    st;
  FILETIME      local;
  TCHAR         attrs[ 6 ] = _T( "-----" );
  LARGE_INTEGER tempLong;
  static TCHAR  hs[] = _T( " KMGT" );
  int           i = 0;
  ULONGLONG     tmp;
  DWORD         poi;
  DWORD         mod;

  /* Create other file size */

  FileTimeToLocalFileTime( &entry->ftLastWriteTime, &local );
  FileTimeToSystemTime( &local, &st );

  st.wYear -= 2000;

  if ( entry->dwFileAttributes & FILE_ATTRIBUTE_SYSTEM )
    {
      attrs[ 0 ] = 'S';
    }

  if ( entry->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN )
    {
      attrs[ 1 ] = 'H';
    }

  if ( entry->dwFileAttributes & FILE_ATTRIBUTE_READONLY )
    {
      attrs[ 2 ] = 'R';
    }

  if ( entry->dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE )
    {
      attrs[ 3 ] = 'A';
    }

  cp = entry->cAlternateFileName;

  if ( !( entry->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
    {
      tempLong.HighPart = entry->nFileSizeHigh;
      tempLong.LowPart  = entry->nFileSizeLow;

      /* ファイル総表示のフォーマット */

      tmp = tempLong.QuadPart * 10;

      while ( tmp >= 10000 )
        {
          tmp /= 1024;
          i++;
        }

      poi = ( DWORD )( tmp / 10 );
      mod = ( DWORD )( tmp % 10 );

      if ( i == 0 )
        {
          ssnprintf( m_info, MAX_LEN, _T( "%-12s %,16U         %02d-%02d-%02d %2d:%02d:%02d %s" ),
            cp,
            tempLong.QuadPart,
            st.wYear,
            st.wMonth,
            st.wDay,
            st.wHour,
            st.wMinute,
            st.wSecond,
            attrs );
        }
      else
        {
          ssnprintf( m_info, MAX_LEN, _T( "%-12s %,16U(%3d.%1d%c) %02d-%02d-%02d %2d:%02d:%02d %s" ),
            cp,
            tempLong.QuadPart,
            poi,
            mod,
            hs[ i ],
            st.wYear,
            st.wMonth,
            st.wDay,
            st.wHour,
            st.wMinute,
            st.wSecond,
            attrs );
        }
    }
  else
    {
      attrs[ 4 ] = 'D';
      ssnprintf( m_info, MAX_LEN, _T( "%-12s %24s %02d-%02d-%02d %2d:%02d:%02d %s" ),
        cp,
        "",
        st.wYear,
        st.wMonth,
        st.wDay,
        st.wHour,
        st.wMinute,
        st.wSecond,
        attrs );
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- ドライブ容量を文字列へ
// --------------------------------------------------------------------------
TCHAR *iInfoView::SizeToStr( ULONGLONG size, TCHAR *pBuf )
{
  static TCHAR  hs[] = _T( " KMGT" );
  int           i    = 0;
  ULONGLONG     tmp  = size * 100;
  DWORD         poi;
  DWORD         mod;

  // ファイル総表示のフォーマット

  while ( tmp >= 100000 )
    {
      tmp /= 1024;
      i++;
    }

  poi = ( DWORD )( tmp / 100 );
  mod = ( DWORD )( tmp % 100 );

  if ( i == 0 )
    {
      ssnprintf( pBuf, 64, _T( "%,21U" ), size );
    }
  else
    {
      ssnprintf( pBuf, 64, _T( "%,21U(%3d.%02d%c)" ), size, poi, mod, hs[ i ] );
    }

  return pBuf;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- ドライブ容量を文字列へ
// --------------------------------------------------------------------------
TCHAR *iInfoView::SizeToStr( ULONGLONG capa, ULONGLONG free, TCHAR *pBuf )
{
  static const TCHAR* hs[] =
  {
    _T( "B" ),
    _T( "KB" ),
    _T( "MB" ),
    _T( "GB" ),
    _T( "TB" ),
  };
  int         i1   = 0;
  int         i2   = 0;
  ULONGLONG   tmp1 = capa * 100;
  ULONGLONG   tmp2 = free * 100;
  DWORD       poi1;
  DWORD       mod1;
  DWORD       poi2;
  DWORD       mod2;

  /* ファイル総表示のフォーマット */

  while ( tmp1 >= 100000 )
    {
      tmp1 /= 1024;
      i1++;
    }

  while ( tmp2 >= 100000 )
    {
      tmp2 /= 1024;
      i2++;
    }

  poi1 = ( DWORD )( tmp1 / 100 );
  mod1 = ( DWORD )( tmp1 % 100 );
  poi2 = ( DWORD )( tmp2 / 100 );
  mod2 = ( DWORD )( tmp2 % 100 );

  if ( i2 == 0 )
    {
      ssnprintf( pBuf, 64, _T( "%d %s %d.%02d %s" ), poi2, hs[ i2 ], poi1, mod1, hs[ i1 ] );
    }
  else
    {
      ssnprintf( pBuf, 64, _T( "%d.%02d %s/ %d.%02d %s" ), poi2, mod2, hs[ i2 ], poi1, mod1, hs[ i1 ] );
    }

  return pBuf;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- ドライブが変更されたときドライブ容量更新
// --------------------------------------------------------------------------
void iInfoView::ChangeDrive( TCHAR *drv )
{
  ULARGE_INTEGER  total;
  ULARGE_INTEGER  used;
  ULARGE_INTEGER  free;
  SHFILEINFO    shfi;

  total.QuadPart = 0;
  used.QuadPart  = 0;
  free.QuadPart  = 0;

  if ( ::GetDiskFreeSpaceEx( drv, &free, &total, NULL ) )
    {
      used.QuadPart = total.QuadPart - free.QuadPart;

      /* サイズを文字列に変換 */

      SizeToStr( total.QuadPart, m_capa );
      SizeToStr( used.QuadPart,  m_used );
      SizeToStr( free.QuadPart,  m_free );
      SizeToStr( total.QuadPart, free.QuadPart, m_usedcapa );
    }
  else
    {
      ZeroMemory( m_capa, sizeof( m_capa ) );
      ZeroMemory( m_used, sizeof( m_used ) );
      ZeroMemory( m_free, sizeof( m_free ) );
      ZeroMemory( m_usedcapa, sizeof( m_usedcapa ) );
    }

  m_drive_handle = ( HANDLE )::SHGetFileInfo(
    drv, FILE_ATTRIBUTE_ARCHIVE, &shfi, sizeof( SHFILEINFO ), SHGFI_DISPLAYNAME /*SHGFI_TYPENAME*/ | SHGFI_SYSICONINDEX );

  if ( total.QuadPart )
    {
      m_drive_rate = ( int )( used.QuadPart * 1000 / total.QuadPart );
    }
  else
    {
      m_drive_rate = 0;
    }

  _tcscpy_s( m_drive_name, 80, shfi.szDisplayName/*szTypeName*/ );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- ファイルに関連づけられたアイコンの表示
// --------------------------------------------------------------------------
HANDLE iInfoView::GetFileIconAndType( const TCHAR *target, TCHAR *type_name )
{
  SHFILEINFO    shfi = {0};
  const TCHAR*  cp;

  if (_tcscmp(PathFindFileName(target), _T("..")) == 0)
    {
      cp = _T("Parent directory");
    }
  else
    {
      ::SHGetFileInfo(target, FILE_ATTRIBUTE_ARCHIVE, &shfi, sizeof( SHFILEINFO ), SHGFI_TYPENAME | SHGFI_ICON | SHGFI_ADDOVERLAYS);

      if ( shfi.hIcon )
        {
          cp = shfi.szTypeName;
        }
      else
        {
          cp = _T("unknown");
        }
    }

  _tcscpy_s(type_name, TYPE_NAME_LEN, cp);

  return shfi.hIcon;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iInfoView::EndCopyInfo( void )
{
  m_mode = 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iInfoView::FillRect( HDC hDC, const RECT *pRC, COLORREF fg, int rate )
{
  xTmpDC  dc( hDC );

  dc.Rectangle( pRC, fg, fg );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iInfoView::FillRect( HDC hDC, int x, int y, int cx, int cy, COLORREF fg, int rate )
{
  xTmpDC  dc( hDC );
  int     w = cx * rate / 1000 + 1;

  dc.Rectangle( x, y, cx, cy, RGB( 128, 128, 128 ), RGB( 128, 128, 128 ) );
  dc.Rectangle( x, y,  w, cy,                   fg,                   fg );
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iInfoView::Refresh( void )
{
  ZeroMemory( m_target, sizeof( m_target ) );
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iInfoView::DrawCapaInfo( HDC hDC, int x, int h, int cx, int cy )
{
  xTmpDC  dc( hDC );
  RECT    rc;
  int     top;
  iPt     pt;
  SIZE    size;

  if ( !m_drive_handle )
  {
    return;
  }

  rc.left   = x + ICON_SIZE + 8;
  rc.top    = OFFSET_T + OFFSET_B + OFFSET_TOP + h + OFFSET_H - 1;
  rc.right  = cx - ( ICON_SIZE + 12 );
  rc.bottom = OFFSET_OFFSET2 + h + OFFSET_OFFSET2 + 1;

  if ( m_drive_rate > 900 )
    {
      FillRect( dc, rc.left, rc.top, rc.right, rc.bottom, pt.COL_REMAIN_BAR_RED, m_drive_rate );
    }
  else
    {
      FillRect( dc, rc.left, rc.top, rc.right, rc.bottom, pt.COL_REMAIN_BAR_BLUE, m_drive_rate );
    }

  top = OFFSET_T + OFFSET_B + OFFSET_TOP;

  dc.TextOut( x + ICON_SIZE + 8 + OFFSET_OFFSET2, top, m_drive_name );

  dc.SetTextColor( pt.COL_BASE_BAR );

  GetTextExtentPoint32( dc, m_usedcapa, ( int )_tcslen( m_usedcapa ), &size );

  dc.TextOut( rc.left + ( rc.right - size.cx ) / 2, rc.top + ( rc.bottom - size.cy ) / 2, m_usedcapa );

}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iInfoView::OnMouseMove( int x, int y )
{
  int   id;

  if ( ThePtInRect( x, y ) )
    {
      if ( m_mode == 0 )
        {
          m_mode = 1;
          Redraw();
        }

      m_timer_on = 0;
    }
  else
    {
      OnMouseLeave();
    }

  if ( ( id = GetToolActionID( x, y ) ) != m_prev_id )
    {
      m_prev_id = id;

      switch ( id )
        {
          case 0:
            if ( mPastEnable == FALSE )
              {
                m_prev_id = -1;
              }
            break;
          case 1:
            if ( mCopyEnable == FALSE )
              {
                m_prev_id = -1;
              }
            break;
          default:
            break;
        }

      Redraw();
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iInfoView::OnMouseLeave( void )
{
  if ( m_mode == 1 && m_timer_on == 0 )
    {
      m_timer_on = 1;
      ::SetTimer( mhWnd, IINFO_LEAVE_TIMERID, 2000, NULL );   // タイマー設定
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iInfoView::OnMouseHover( void )
{
  if ( m_mode == 1 )
    {
      m_mode = 0;
      Redraw();
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iInfoView::OnLButtonDown( int x, int y )
{
  m_action = GetToolActionID( x, y );

  switch ( m_action )
    {
      case 0:
        if ( mPastEnable == FALSE )
          {
            m_action = -1;
          }
        break;
      case 1:
        if ( mCopyEnable == FALSE )
          {
            m_action = -1;
          }
        break;
      default:
        break;
    }

  Redraw();
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iInfoView::OnLButtonUp( int x, int y )
{
  if ( m_action == GetToolActionID( x, y ) && m_action >= 0 )
    {
      SendID( mi_lines[ m_action ].id );
    }

  m_action = -1;
  Redraw();
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iInfoView::SetMiiSubRect( HDC hDC, ICONICON2 *lines, int x, int cy, WORD id, int offset, const TCHAR *str )
{
  SIZE  size;
  int   icon_size;

  size.cx = 0;
  icon_size = 0;

  if ( str )
    {
      _tcscpy_s( lines->name, MAX_PATH, str );
      ::GetTextExtentPoint32( hDC, str, ( int )_tcslen( str ), &size );
    }

  if ( lines->handle != HENABLE )
    {
      icon_size = ICON_SIZE16;

      if ( size.cx )
        {
          icon_size += OFFSET_INTERVAL;
        }
    }

  lines->rc.left   = x;
  lines->rc.top    = OFFSET_TOP;
  lines->rc.right  = x + offset + icon_size + size.cx + offset;
  lines->rc.bottom = cy - OFFSET_BTM;
  lines->id        = id;

  return lines->rc.right;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iInfoView::GetToolActionID( int x, int y )
{
  int   i;
  int   target = -1;
  POINT pos;

  pos.x = x - m_x;
  pos.y = y - m_y;

  for ( i = 0; i < TB_MAX_LINE; i++ )
    {
      if ( mi_lines[ i ].handle == NULL )
        {
          continue;
        }

      if ( PtInRect( &mi_lines[ i ].rc, pos ) )
        {
          target = i;
          break;
        }
    }

  return target;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iInfoView::OnTimer( UINT id )
{
  if ( m_timer_on )
    {
      m_mode = 0;
      Redraw();
    }
}
// --------------------------------------------------------------------------
