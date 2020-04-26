/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iBaseView.cpp
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
#include "xDcBase.h"
#include "iBaseView.h"

#define FONT_SIZE     12
#define FONT_NAME     _T( "ＭＳ ゴシック" )
#define WIN10_LINES   1

// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iBaseView::iBaseView( int base )
{
  memset( mi_lines, 0, sizeof( ICONICON2 ) * TB_MAX_LINE );

  SetFont( FONT_NAME, FONT_SIZE );

  m_baseid = base;

  mhWnd = NULL;
  mhDC  = NULL;

  m_focus = TRUE;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iBaseView::~iBaseView()
{
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iBaseView::Create( HWND hWnd )
{
  mhWnd = hWnd;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
COLORREF iBaseView::Brend( COLORREF col, int alph )
{
  int   r = GetRValue( col );
  int   g = GetGValue( col );
  int   b = GetBValue( col );

  r *= alph;
  g *= alph;
  b *= alph;
  r /= 100;
  g /= 100;
  b /= 100;
  alph = 100 - alph;
  r += ( 0xFF * alph / 100 );
  g += ( 0xFF * alph / 100 );
  b += ( 0xFF * alph / 100 );

  if ( r > 255 )
    {
      r = 255;
    }

  if ( g > 255 )
    {
      g = 255;
    }

  if ( b > 255 )
    {
      b = 255;
    }

  return RGB( r, g ,b );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iBaseView::FillRect( HDC hDC, const RECT *pRC, COLORREF fg )
{
#if WIN10_LINES
  xTmpDC    dc( hDC );

  dc.Rectangle( pRC, fg, fg );
#else
  int     i;
  int     h;
  int     alph;
  xTmpDC  dc( hDC );

  h = pRC->bottom - pRC->top;

  dc.RoundRect( pRC, 4, 4, fg, Brend( fg, 50 ) );

  for ( i = 0; i < h - 2; i++ )
    {
      alph = i * 16 / ( h - 1 ) + 9;

      dc.Line( pRC->left + 1, i + pRC->top + 1, pRC->right - 1, i + pRC->top + 1, Brend( fg, alph ) );
    }
#endif
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iBaseView::FillRect( HDC hDC, const RECT *pRC, COLORREF fg, COLORREF waku )
{
#if WIN10_LINES
  xTmpDC  dc( hDC );

  dc.Rectangle( pRC, waku, fg );
#else
  int   i;
  int   h;
  int   alph;
  xTmpDC  dc( hDC );

  h = pRC->bottom - pRC->top;

  dc.RoundRect( pRC, 4, 4, fg, waku );

  for ( i = 0; i < h - 2; i++ )
    {
      alph = i * 16 / ( h - 1 ) + 9;

      dc.Line( pRC->left + 1, i + pRC->top + 1, pRC->right - 1, i + pRC->top + 1, Brend( fg, alph ) );
    }
#endif
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iBaseView::FillRect( HDC hDC, int x, int y, int cx, int cy, COLORREF fg )
{
  RECT  rc;

  rc.left   = x;
  rc.top    = y;
  rc.right  = x + cx;
  rc.bottom = y + cy;

  FillRect( hDC, &rc, fg );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iBaseView::FillRect( HDC hDC, int x, int y, int cx, int cy, COLORREF fg, COLORREF waku )
{
  RECT  rc;

  rc.left   = x;
  rc.top    = y;
  rc.right  = x + cx;
  rc.bottom = y + cy;

  FillRect( hDC, &rc, fg, waku );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iBaseView::OnMouseMove( int x, int y )
{
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iBaseView::OnMouseLeave( void )
{
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iBaseView::OnLButtonDown( int x, int y )
{
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iBaseView::OnLButtonUp( int x, int y )
{
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iBaseView::OnKeyDown( UINT vk, BOOL fDown, int cRepeat, UINT flags )
{
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iBaseView::DrawIcon( HDC hDC, ICONICON2* info, int ofst_x, int ofst_y )
{
  DrawIcon( hDC, info->handle, info->index, info->rc.left + ofst_x, info->rc.top + ofst_y );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iBaseView::DrawIcon( HDC hDC, HANDLE handle, int idx, int x, int y, int size )
{
  ::ImageList_DrawEx( ( HIMAGELIST )handle, idx, hDC, x, y, size, size, CLR_NONE, CLR_NONE, ( m_focus ? ILD_NORMAL : ILD_BLEND50 ) );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iBaseView::SetFont( const TCHAR *fname, int size )
{
  _tcscpy_s( m_fontname, MAX_PATH, fname );
  m_fontsize = size;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- ファイルに関連づけられたプロセスの起動
// --------------------------------------------------------------------------
void iBaseView::ShellExecute( const TCHAR *pPath )
{
  HINSTANCE res;
  struct {
    const HINSTANCE res;
    const TCHAR*    pMsg;
  }
  msgList[] = {
    { reinterpret_cast<const HINSTANCE>(ERROR_FILE_NOT_FOUND),   _T( "ファイルが見つかりませんでした" ) },
    { reinterpret_cast<const HINSTANCE>(ERROR_PATH_NOT_FOUND),    _T( "パスが見つかりませんでした" ) },
    { reinterpret_cast<const HINSTANCE>(ERROR_BAD_FORMAT),       _T( "Win32用のアプリケーションではないか、イメージでエラーが発生しました)" ) },
    { reinterpret_cast<const HINSTANCE>(SE_ERR_ACCESSDENIED),    _T( "ＯＳが指定したファイルへのアクセスが拒否されました" ) },
    { reinterpret_cast<const HINSTANCE>(SE_ERR_ASSOCINCOMPLETE), _T( "ファイルに関連づけられた物が不完全かまたは無効である" ) },
    { reinterpret_cast<const HINSTANCE>(SE_ERR_DDEBUSY),         _T( "他のDDEプロセスが通信中です" ) },
    { reinterpret_cast<const HINSTANCE>(SE_ERR_DDEFAIL),         _T( "DDE通信エラー" ) },
    { reinterpret_cast<const HINSTANCE>(SE_ERR_DDETIMEOUT),      _T( "DDE通信タイムアウト" ) },
    { reinterpret_cast<const HINSTANCE>(SE_ERR_DLLNOTFOUND),     _T( "指定したDLLファイルが見つかりませんでした" ) },
    { reinterpret_cast<const HINSTANCE>(SE_ERR_FNF),             _T( "ファイルが見つかりませんでした" ) },
    { reinterpret_cast<const HINSTANCE>(SE_ERR_NOASSOC),         _T( "指定したファイルに関連づけられたアプリケーションが見つかりませんでした" ) },
    { reinterpret_cast<const HINSTANCE>(SE_ERR_OOM),             _T( "処理を完了するのに十分なメモリがありません" ) },
    { reinterpret_cast<const HINSTANCE>(SE_ERR_PNF),             _T( "指定したパスが見つかりませんでした" ) },
    { reinterpret_cast<const HINSTANCE>(SE_ERR_SHARE),           _T( "共有違反が発生しました" ) },
    { 0,                                                         _T( "ＯＳのメモリまたはリソースが足りません" ) },
  },
  *pList = msgList;

  res = ::ShellExecute( NULL, NULL, pPath, NULL, NULL, SW_SHOWNORMAL );

  while ( pList->res )
    {
      if ( pList->res == res )
        {
          ::MessageBox( mhWnd, pList->pMsg, _T( "実行エラー" ), MB_ICONERROR | MB_OK );
          break;
        }
      pList++;
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iBaseView::ExtTextOut2( HDC hDC, int x, int y, const TCHAR *str, int cnt, int *lens, COLORREF *cols, int *points, int str_offset )
{
#ifdef UNICODE
  int           i;
  int           j = 1;
  int           pps[ 2000 ];
  int*          p;
  const TCHAR*  cp;

  for ( cp = str, p = points, i = 0; *cp && i < 2000; cp++, p++, i++ )
    {
      if ( *( str + i ) > 255 )
        {
          pps[ i ]  = *p++; 
          pps[ i ] += *p++; 
        }
      else
        {
          pps[ i ]  = *p++; 
        }
    }

  pps[ i ] = 0;

  p = pps;

  for ( i = 0; i < cnt; i++ )
    {
      if ( lens[ i ] == 0 )
        {
          continue;
        }

      ::SetTextColor( hDC, cols[ i ] );

      ::ExtTextOut( hDC, x, y, 0, NULL, str, str_offset, NULL );

      for ( j = 0; j < lens[ i ]; j++ )
        {
          x += *points++;
        }

      str += str_offset;
      str_offset = ( int )_tcslen( str );
    }
#else
  int   i;
  int   j;

  for ( i = 0; i < cnt; i++ )
    {
      if ( lens[ i ] == 0 )
        {
          continue;
        }

      ::SetTextColor( hDC, cols[ i ] );

      ::ExtTextOut( hDC, x, y, 0, NULL, str, lens[ i ], points );

      for ( j = 0; j < lens[ i ]; j++ )
        {
          x += *points++;
          str++;
        }
    }
#endif
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL iBaseView::CheckRect( int xx, int yy, int cxx, int cyy, int *x, int *y, int *cx, int *cy )
{
  BOOL  res = TRUE;

  if ( *x > xx + cxx || *y > yy + cyy )
    {
      res = FALSE;
    }
  else
    {
      if ( *x < xx )
        {
          *x = xx;
        }

      if ( *y < yy )
        {
          *y = yy;
        }

      if ( *x + *cx > xx + cxx )
        {
          *cx = xx + cxx - *x;
        }

      if ( *y + *cy > yy + cyy )
        {
          *cy = yy + cyy - *y;
        }
    }

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iBaseView::SendID( int id )
{
  ::SendMessage( mhWnd, WM_COMMAND, MAKEWPARAM( ( UINT )( m_baseid ),( UINT )( id ) ), ( LPARAM )( HWND )( mhWnd ) );
}
// --------------------------------------------------------------------------


// ===========================================================================
// --
// ---------------------------------------------------------------------------
BOOL iBaseView::ThePtInRect( int x, int y )
{
  BOOL  res = FALSE;
  POINT pos;
  RECT  rc;

  pos.x     = x;
  pos.y     = y;
  rc.left   = m_x;
  rc.top    = m_y;
  rc.right  = m_x + m_cx;
  rc.bottom = m_y + m_cy;

  if ( PtInRect( &rc, pos ) )
    {
      res = TRUE;
    }

  return res;
}
// ---------------------------------------------------------------------------


// ==========================================================================
// -- ラストエラーの内容表示
// --------------------------------------------------------------------------
TCHAR *iBaseView::LastError( void )
{
  static TCHAR  error_string[ MAX_PATH ];

  FormatMessage( 
    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    GetLastError(),
    MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),    // デフォルト言語
    ( LPTSTR )error_string,
    MAX_PATH,
    NULL 
  );

  return error_string;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
TCHAR *iBaseView::RemoveFileSpec( TCHAR *buf, const TCHAR *name )
{
  _tcscpy_s( buf, MAX_PATH, name );
  PathRemoveFileSpec( buf );

  return PathFindFileName( name );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- ラストエラーの内容表示
// --------------------------------------------------------------------------
TCHAR *iBaseView::GetLastErrString( DWORD err )
{
  static TCHAR  buf[ 128 ];

  FormatMessage( 
    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    err,
    MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),    // デフォルト言語
    ( LPTSTR )buf,
    128,
    NULL 
  );

  return buf;
}
// --------------------------------------------------------------------------
