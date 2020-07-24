/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iSeachView.cpp
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
#include "savedir.h"
#include "iNamelist.h"
#include "FormatInc.h"
#include "xDcBase.h"
#include "SubFuncs.h"
#include "iIcon.h"
#include "iBaseView.h"
#include "iDragDrop.h"
#include "logf.h"
#include "CopyFuncs.h"
#include "timerid.h"
#include "ipt.h"
#include "GdiLoadImage.h"
#include "iSeachView.h"

/* Local definition */

#define ICON_SIZE_ORG 16
#define ICON_SIZE     DPIX(ICON_SIZE_ORG)
#define OFFSET_1      16
#define OFFSET_2      4
#define OFFSET_3      DPIX(2)
#define MIN_HEIGHT    (ICON_SIZE + OFFSET_3 + OFFSET_3)
#define STR_LEFT      (OFFSET_ICON + ICON_SIZE + 4)
#define STR_RIGHT     6
#define OFFSET_ICON   6
#define WIN10_LINES   1

/* Private data */

static TCHAR  _desktop_folder[ MAX_PATH ] = _T( "" );


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iSeachView::iSeachView( int base ) : iBaseView( base )
{
  LPITEMIDLIST  pidlist;

  m_current1    = 0;
  m_current2    = -1;
  m_action      = -1;
  m_active      = TRUE;
  mh_handle     = NULL;
  m_hold        = FALSE;
  m_hold_start  = -1;
  m_hold_end    = -1;
  m_topposition = TRUE;
  m_seach_count = 0;

  SetColumn( 3 );

  _tcscpy_s( mEmptyMessage, MAX_PATH, _T( " *** Loading ... *** " ) );

  ZeroMemory( &m_pay, sizeof( m_pay ) );

  if ( _desktop_folder[ 0 ] == '\0' )
    {
      SHGetSpecialFolderLocation( NULL, CSIDL_DESKTOPDIRECTORY, &pidlist );
      SHGetPathFromIDList( pidlist, _desktop_folder );
      CoTaskMemFree( pidlist );
    }

  mp_bmpinfo = NULL;
  m_seaching = FALSE;

  ZeroMemory( mSeachingFolderName, sizeof( mSeachingFolderName ) );

  mSeachingIconState = 0;

  m_disp_rate = 0;
  m_disp_recalc = FALSE;

  m_disp_hold = FALSE;

  m_disp_hold_x = -1;
  m_disp_hold_y = -1;

  m_disp_offset_x = 0;
  m_disp_offset_y = 0;

  m_disp_x = -1;
  m_disp_y = -1;

  m_disp_min_rate = 0;

  m_disp_cx = 0;
  m_disp_cy = 0;

  mb_find = FALSE;

  mp_lastUpdateFile = NULL;
  mb_forceUpdate    = FALSE;

  /* Get monitor size */

  memset(&m_monitor, 0, sizeof(m_monitor));

  GetDisplayMonitors(&m_monitor);
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::DrawForSeachingString( HDC hDC, int x, int y, int cx, int cy )
{
  xTmpDC        dc( hDC );
  SIZE          size;
  const TCHAR*  str = L"Searching ...";
  const TCHAR*  tar = L"Search target";
        TCHAR*  buf;
  int           x1;
  int           y1;
  iPt           pt;
  iico          ico(dc, IMG_LOADING);
  size_t        len0;
  size_t        len1;

  GetTextExtentPoint32( dc, str, (int)_tcslen( str ), &size );

  size.cx += STR_LEFT;

  x1 = x + ( cx - size.cx ) / 2;
  y1 = y + ( cy - dc.tm.tmHeight * 4 - ICON_SIZE ) / 2;

  if ( m_active && !m_lock && m_focus )
    {
      dc.SetTextColor( RGB( 0, 0, 0 ) );
      ico.draw(x1, y1, mSeachingIconState, TRUE);
    }
  else
    {
      dc.SetTextColor( Brend( RGB( 0, 0, 0 ), 50 ) );
      ico.draw(x1, y1, mSeachingIconState, FALSE);
    }

  y1 = y + ( cy - dc.tm.tmHeight * 4 - dc.tm.tmHeight ) / 2;

  dc.SetBkMode( 0 );
  dc.TextOut( x1 + STR_LEFT, y1, str );

  len0 = _tcslen(tar);
  len1 = (cx - 4) / dc.tm.tmAveCharWidth;

  for (int i = 0; i < m_argc; i++)
    {
      len0 += 3;
      len0 += _tcslen(m_argv[i]);
    }

  if (len0 < len1)
    {
      len0 = len1;
    }

  buf = (TCHAR *)alloca((len0 + 1) * sizeof(TCHAR));

  _tcscpy_s(buf, len0, tar);

  for (int i = 0; i < m_argc; i++)
    {
      StrCat(buf, L" \"");
      StrCat(buf, m_argv[i]);
      StrCat(buf, L"\"");
    }

  GetTextExtentPoint32(dc, buf, (int)_tcslen(buf), &size);

  x1  = x + (cx - size.cx) / 2;

  y1 += (dc.tm.tmHeight + 12);

  dc.TextOut(x1, y1, buf);

  y1 += (dc.tm.tmHeight + 8);

  dc.FillRect( x + 2, y1, cx - 4, dc.tm.tmHeight , pt.COL_BASE_BAR );

  if (len1 > 0)
    {
      PathCompactPathEx(buf, mSeachingFolderName, (UINT)len1, 0);

      GetTextExtentPoint32( dc, buf, (int)_tcslen( buf ), &size );

      x1 = x + ( cx - size.cx ) / 2;

      dc.TextOut( x1, y1, buf );
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::DrawForSeaching( HDC hDC, int x, int y, int cx, int cy )
{
  xTmpDC  dc( hDC );
  iPt     pt;

  dc.FillRect( x, y, cx, cy, pt.COL_BASE_BAR );
  dc.SetStretchBltMode( HALFTONE );
  dc.Line( x + cx - 1, y, x + cx - 1, y + cy );
  dc.Line( x + 3, y + cy - 4, x + cx - 3, y + cy - 4 );

  if ( m_topposition )
    {
      dc.SetSelectPen( pt.COL_SPLIT_LINE );
      dc.Line( x, y + cy - 1, x + cx - 1, y + cy - 1 );
    }
  else
    {
      dc.Line( x, y + cy - 1, x + cx - 1, y + cy - 1 );
      dc.SetSelectPen( pt.COL_SPLIT_LINE );
    }

  dc.Line( x, y, x + cx - 1, y );
  dc.Line( x, y, x, y + cy - 1 );
  dc.Line( x + cx - 2, y, x + cx - 2, y + cy - 1 );

  DrawForSeachingString( dc, x, y, cx, cy );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::StretchDIBits( HDC hDC, int x, int y, int dispCx, int dispCy, BITMAPINFO *pBitmapInfo, void *pData )
{
  xTmpDC      dc( hDC );
  int         cx;
  int         cy;
  int         w = pBitmapInfo->bmiHeader.biWidth;
  int         h = pBitmapInfo->bmiHeader.biHeight;
  int         rate_x;
  int         rate_y;
  static int  rate = 0;

  if ( m_disp_recalc == 0 )
    {
      m_disp_recalc = 1;

      if ( w < dispCx && h < dispCy )
        {
          cx = w;
          cy = h;
        }
      else
        {
          cx = dispCx;
          cy = h * cx / w;

          if ( cy > dispCy )
            {
              cy = dispCy;
              cx = w * cy / h;
            }
        }

      rate_x = cx * 100 / w;
      rate_y = cy * 100 / h;

      if ( rate_x > rate_y )
        {
          m_disp_rate = rate_y;
        }
      else
        {
          m_disp_rate = rate_x;
        }

      cx = m_disp_rate * w / 100;
      cy = m_disp_rate * h / 100;

      x += ( dispCx - cx ) / 2;
      y += ( dispCy - cy ) / 2;

      m_disp_x = x;
      m_disp_y = y;

      rate = m_disp_rate;

      m_disp_min_rate = m_disp_rate;

      m_disp_cx = dispCx;
      m_disp_cy = dispCy;
    }
  else
    {
      cx = m_disp_rate * w / 100;
      cy = m_disp_rate * h / 100;

      if ( rate != m_disp_rate )
        {
          x += ( dispCx - cx ) / 2;
          y += ( dispCy - cy ) / 2;

          rate = m_disp_rate;
        }
    }

  if ( m_disp_rate < 100 )
    {
      dc.SetStretchBltMode( HALFTONE );
    }
  else
    {
      dc.SetStretchBltMode( COLORONCOLOR );
    }

  m_disp_cx       = dispCx;
  m_disp_cy       = dispCy;
  m_disp_offset_x = dispCx - cx;
  m_disp_offset_y = dispCy - cy;

  dc.StretchDIBits( m_disp_x, m_disp_y, cx, cy, pBitmapInfo, pData );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::Draw( HWND hWnd, HDC hDC, int x, int y, int cx, int cy )
{
  xTmpDC  dc( hDC );
  xBmpDC  bmp;
  int     cx_d,
          cy_d;
  iPt     pt;

  mhWnd = hWnd;
  mhDC  = dc;
  m_x   = x;
  m_y   = y;
  m_cx  = cx;
  m_cy  = cy;

  dc.SetSelectFont( m_fontname, m_fontsize );
  dc.SelectObject( GetStockObject( NULL_BRUSH ) );
  dc.SetSelectPen( pt.COL_BASE_BAR );

  SetMiRect( x + 3, y + 3, cx - 7, cy - 7, dc.tm.tmAveCharWidth, dc.tm.tmHeight );

  if ( m_total > 0 && m_current1 >= m_total )
    {
      m_current1 = m_total - 1;
    }

  if ( m_seaching )
    {
      DrawForSeaching( dc, x, y, cx, cy );
    }
  else if ( mp_bmpinfo )
    {
      cx_d = cx - 5;
      cy_d = cy - 4;
      bmp.Create( dc, cx_d, cy_d );
      bmp.SetSelectPen( pt.COL_BASE_BAR );
      bmp.FillRect( 0, 0, cx_d, cy_d, pt.COL_BASE_BAR );

      dc.SetSelectPen( pt.COL_BASE_BAR );

      if ( m_topposition )
        {
          dc.Line( x, y + cy - 2, x + cx - 1, y + cy - 2 );
        }

      dc.Line( x, y + 1, x + cx - 1, y + 1 );
      dc.Line( x + 1, y, x + 1, y + cy - 1 );
      dc.Line( x + cx - 3, y, x + cx - 3, y + cy - 1 );
      dc.Line( x + cx - 1, y, x + cx - 1, y + cy );

      dc.SetSelectPen( pt.COL_SPLIT_LINE );

      if ( m_topposition )
        {
          dc.Line( x, y + cy - 1, x + cx - 1, y + cy - 1 );
        }

      dc.Line( x, y, x + cx - 1, y );
      dc.Line( x, y, x, y + cy - 1 );
      dc.Line( x + cx - 2, y, x + cx - 2, y + cy - 1 );

      StretchDIBits( bmp, 0, 0, cx_d, cy_d, mp_bmpinfo );

      dc.BitBlt( x + 2, y + 2, cx_d, cy_d, bmp, 0, 0 );
    }
  else
    {
      dc.Rectangle( x + 1, y + 1, cx - 3, cy - 2 );
      dc.Rectangle( x + 2, y + 2, cx - 5, cy - 4 );
      dc.Line( x + cx - 1, y, x + cx - 1, y + cy );
      dc.Line( x + 3, y + cy - 4, x + cx - 3, y + cy - 4 );

      if ( m_topposition )
        {
#if not WIN10_LINES
          dc.SetSelectPen( pt.COL_SPLIT_LINE );
#endif
          dc.Line( x, y + cy - 1, x + cx - 1, y + cy - 1 );
          dc.Line( x, y, x, y + cy - 1 );
        }
      else
        {
          dc.Line( x, y + cy - 1, x + cx - 1, y + cy - 1 );

#if not WIN10_LINES
          dc.SetSelectPen( pt.COL_SPLIT_LINE );
#endif
          dc.Line( x, y, x, y + cy );
        }

      dc.Line( x, y, x + cx - 1, y );
      dc.SetSelectPen( pt.COL_SPLIT_LINE );
      dc.Line( x + cx - 2, y, x + cx - 2, y + cy - 1 );

      dc.FillRect( x + 2, m_pay.y[ m_pay.lines - 1 ] + m_pay.h , cx - 5, cy + y - ( m_pay.y[ m_pay.lines - 1 ] + m_pay.h ) - 1, pt.COL_BASE_BAR );

      DrawMain( dc );
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::Redraw( void )
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
void iSeachView::DrawMain( HDC hDC )
{
  int     i;
  int     j;
  int     line_no;
  RECT    rc;
  xTmpDC  dc( hDC );
  xBmpDC  bmp;
  int     lines = m_pay.h + 10;
  iPt     pt;

  bmp.Create( dc, m_pay.w, lines );
  bmp.SetSelectFont( m_fontname, m_fontsize );
  bmp.SetBkMode( TRANSPARENT );

  for ( i = 1; i < m_pay.column; i++ )
    {
      rc.left   = m_pay.x[ i - 1 ] + m_pay.w;
      rc.top    = m_pay.y[ 0 ];
      rc.right  = m_pay.x[ i ];
      rc.bottom = m_pay.y[ m_pay.lines - 1 ] + m_pay.h;
      FillRect3( dc, &rc, pt.COL_BASE_BAR );
    }

  line_no = m_current1 / m_pay.line_per_page * m_pay.line_per_page;

  for ( i = 0; i < m_pay.column; i++ )
    {
      for ( j = 0; j < m_pay.lines; j++ )
        {
          if ( m_total > line_no )
            {
              DrawLine( bmp, line_no );
              line_no++;
              BitBlt3( dc, bmp, m_pay.x[ i ], m_pay.y[ j ], m_pay.w, m_pay.heights[ j ] );
            }
          else
            {
              FillRect3( dc, m_pay.x[ i ], m_pay.y[ j ], m_pay.w, m_pay.heights[ j ], pt.COL_BASE_BAR );
            }
        }
    }

  if ( m_total > 0 )
    {
      return;
    }

  rc.left   = m_x;
  rc.top    = m_y;
  rc.right  = m_x + m_cx;
  rc.bottom = m_y + m_cy;

  dc.SetBkMode( TRANSPARENT );

  if ( mEmptyMessage[ 0 ] == ' ' )
    {
      dc.SetTextColor( RGB( 64, 64, 64 ) );
    }
  else
    {
      if ( m_active && !m_lock && m_focus )
        {
          dc.SetTextColor( RGB( 255, 0, 0 ) );
        }
      else
        {
          dc.SetTextColor( Brend( RGB( 255, 0, 0 ), 50 ) );
        }
    }

  dc.Printf( &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE, mEmptyMessage );

}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iSeachView::~iSeachView()
{
  if ( mp_bmpinfo )
    {
      GlobalFree( mp_bmpinfo );
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iSeachView::SetMiRect( int x, int y, int cx, int cy, int w, int h )
{
  int   lines;
  int   width;
  int   height;
  int   i;
  int   len;
  int   pos;
  int   last_pos = 0;
  int   column   = m_pay.column;
  int   lim;
  iPt   pt;

  width  = cx - OFFSET_1 * ( column - 1 );

  lim = OFFSET_1 * ( column - 1 ) + w * 10 * column;

  if ( width < lim )
    {
      width = lim;
    }

  for ( i = 0; i < column; i++ )
    {
      m_pay.x[ i ] = i * ( width + 1 ) / column + OFFSET_1 * i + x;
    }

  if ( m_seach_mode == INAM_MODE_NORMAL )
    {
      height = h * 1 + /*OFFSET_2 + OFFSET_2*/pt.line_pitch;
    }
  else
    {
      height = h * 3 + /*OFFSET_2 + OFFSET_2*/pt.line_pitch;
    }

  if ( height < MIN_HEIGHT )
    {
      height = MIN_HEIGHT;
    }

  if ( cy < height )
    {
      cy = height;
    }

  lines = cy / height;

  if ( lines > LINE_MAX )
    {
      lines = LINE_MAX;
    }

  for ( i = 0; i < lines; i++ )
    {
      m_pay.y[ i ] = height * i + y;
    }

  for ( i = 0; i < lines - 1; i++ )
    {
      m_pay.heights[ i ] = m_pay.y[ i + 1 ] - m_pay.y[ i ];
    }

  m_pay.heights[ i ] = ( y + cy ) - m_pay.y[ i ];

  m_pay.w     = width / column;
  m_pay.h     = height;
  m_pay.lines = lines;

  if ( m_seach_mode == INAM_MODE_NORMAL )
    {
      m_pay.str_top = ( ( m_pay.h + 1 ) - h ) / 2;
    }
  else
    {
      m_pay.str_top = (m_pay.h / 2 - h) / 2;
    }

  width = m_pay.w - ( STR_LEFT + STR_RIGHT );
  len   = width / w;

  if ( len > LEN_MAX )
    {
      len = LEN_MAX;
    }

  for ( i = 1; i <= len; i++ )
    {
      pos = i * ( width + 1 ) / len;

      m_pay.points[ i - 1 ] = pos - last_pos;
      last_pos = pos;
    }

  m_pay.len = len;
  m_pay.line_per_page = m_pay.lines * m_pay.column;

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iSeachView::GetToolActionID( int x, int y )
{
  int   target = -1;
  int   i;
  RECT  rc;
  POINT pos;

  pos.x = x;
  pos.y = y;

  for ( i = 0; i < m_pay.column * m_pay.lines; i++ )
    {
      rc.left   = m_pay.x[ i / m_pay.lines ];
      rc.top    = m_pay.y[ i % m_pay.lines ];
      rc.right  = rc.left + m_pay.w;
      rc.bottom = rc.top + m_pay.h;

      if ( PtInRect( &rc, pos ) )
        {
          target = i + m_current1 / m_pay.line_per_page * m_pay.line_per_page;
          break;
        }
    }

  if ( target >= m_total )
    {
      target = -1;
    }

  return target;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::OnMouseMove( int x, int y )
{
  int         target = GetToolActionID( x, y );
  int         i;
  static int  last_target = -1;
  static int  _x = -1;
  static int  _y = -1;

  if( GetEnableAction() )
    {
      if ( mp_bmpinfo && m_disp_hold )
        {
          if( m_disp_offset_x < 0 )
            {
              m_disp_x += ( x - m_disp_hold_x );

              if ( m_disp_x >= 0 )
                {
                  m_disp_x = 0;
                }
              else if ( m_disp_x < m_disp_offset_x )
                {
                  m_disp_x = m_disp_offset_x;
                }
            }

          if ( m_disp_offset_y < 0 )
            {
              m_disp_y += ( y - m_disp_hold_y );

              if ( m_disp_y >= 0 )
                {
                  m_disp_y = 0;
                }
              else if ( m_disp_y < m_disp_offset_y )
                {
                  m_disp_y = m_disp_offset_y;
                }
            }

          m_disp_hold_x = x;
          m_disp_hold_y = y;
          Redraw();
          return;
        }
      else
        {
          return;
        }
    }

  if ( target < 0 )
    {
      SetCurrentLine2( -1 );
    }
  else
    {
      if ( _x != x || _y != y )
        {
          _x = x;
          _y = y;

          SetCurrentLine2( target );
        }
    }

  if ( m_hold && m_hold_start == -1 && m_current1 != m_action )
    {
      m_hold_start = m_action;
    }

  if ( m_action == m_current1 && m_hold )
    {
      DoDrag();
      m_hold = FALSE;
    }
  else if( m_hold_start >= 0 && target >= 0 )
    {
      if ( last_target != target )
        {
          last_target = target;

          if ( m_hold_start > target )
            {
              for( i = 0; i < m_total; i++ )
                {
                  if ( target <= i && i <= m_hold_start )
                    {
                      if ( _tcscmp( PathFindFileName( m_namelist[ i ]->d_name ), _T( ".." ) ) != 0 )
                        {
                          m_namelist[ i ]->bookMark = 1;
                        }
                      else
                        {
                          m_namelist[ i ]->bookMark = 0;
                        }
                    }
                  else
                    {
                      m_namelist[ i ]->bookMark = 0;
                    }
                }
              Redraw();
            }
          else
            {
              for( i = 0; i < m_total; i++ )
                {
                  if ( m_hold_start <= i && i <= target )
                    {
                      if ( _tcscmp( PathFindFileName( m_namelist[ i ]->d_name ), _T( ".." ) ) != 0 )
                        {
                          m_namelist[ i ]->bookMark = 1;
                        }
                      else
                        {
                          m_namelist[ i ]->bookMark = 0;
                        }
                    }
                  else
                    {
                      m_namelist[ i ]->bookMark = 0;
                    }
                }

              Redraw();
            }

          if ( target )
            {
              m_hold_end = target;
            }
        }
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::OnMouseLeave( void )
{
  if ( GetEnableAction() )
    {
      return;
    }

  SetCurrentLine2( -1 );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::OnLButtonDown( int x, int y )
{
  if ( GetEnableAction() )
    {
      if( mp_bmpinfo ) {
        m_disp_hold = TRUE;
        m_disp_hold_x = x;
        m_disp_hold_y = y;
      }
      return;
    }

  m_action = GetToolActionID( x, y );
  m_hold = TRUE;
  m_hold_start = -1;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::DoDrag( void )
{
  iDragDrop*  drop;
  TCHAR*      cp;
  size_t      size;
  HDROP       hDrop;
  dirent*     entry;

  drop = new iDragDrop;

  if ( !drop )
    {
      return;
    }

  if ( GetMarkedNum( &entry ) > 0 )
    {
      hDrop = ( HDROP )MakeHDropFiles( entry );

      if ( hDrop )
        {
          ::ReleaseCapture();

          cp = ( TCHAR* )::GlobalLock( hDrop );
          size = ::GlobalSize( hDrop );

          ::DragAcceptFiles( mhWnd, FALSE );    // ドロップファイルを拒否！

          drop->DoDragDrop( cp, size );     // ドラッグ開始

          ::DragAcceptFiles( mhWnd, TRUE );   // ドロップファイルを受け入れ

          GlobalFree( hDrop );
        }
    }

  delete drop;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::OnRButtonDown( int x, int y )
{
  if ( GetEnableAction() )
    {
      return;
    }

  m_action = GetToolActionID( x, y );
  m_hold = TRUE;
  m_hold_start = -1;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::OnRButtonUp( int x, int y )
{
  int   i;
  int   res = 0;
  
  if ( GetEnableAction() )
    {
      SendID( m_current1 );
      return;
    }

  m_hold = FALSE;

  if ( m_action == GetToolActionID( x, y ) && m_action >= 0 )
    {
      SetCurrentLine1( m_action );
      m_action = -1;

      if ( m_total > 0 && m_current1 < m_total && 0 <= m_current1 )
        {
          _tcscpy_s( m_selected, MAX_PATH, m_namelist[ m_current1 ]->d_name );

          for ( i = 0; i < m_total; i++ )
            {
              res += m_namelist[ i ]->bookMark;
            }

          if ( res )
            {
              Redraw();
            }
        }
      SendID( m_current1 );
    }
  else
    {
      if ( ThePtInRect( x, y ) )
        {
          _tcscpy_s( m_selected, MAX_PATH, m_dir );
          SendID( -1 );
        }
    }

  if ( m_hold_start >= 0 )
    {
      SetCurrentLine1( m_hold_end );
      m_hold_start = -1;
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::OnLButtonUp( int x, int y )
{
  int   i;
  int   res = 0;

  m_disp_hold   = FALSE;
  m_disp_hold_x = -1;
  m_disp_hold_y = -1;

  if ( GetEnableAction() )
    {
      return;
    }

  m_hold = FALSE;

  if ( m_action == GetToolActionID( x, y ) && m_action >= 0 )
    {
      SetCurrentLine1( m_action );
      m_action = -1;

      if ( m_total > 0 && m_current1 < m_total && 0 <= m_current1 )
        {
          _tcscpy_s( m_selected, MAX_PATH, m_namelist[ m_current1 ]->d_name );

          for ( i = 0; i < m_total; i++ )
            {
              res += m_namelist[ i ]->bookMark;
              m_namelist[ i ]->bookMark = 0;
            }

          if ( res )
            {
              Redraw();
            }
        }
    }

  if ( m_hold_start >= 0 )
    {
      SetCurrentLine1( m_hold_end );
      m_hold_start = -1;
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::OnLButtonDblClk( int x, int y, UINT flags )
{
  if ( GetEnableAction() )
    {
      return;
    }

  if ( GetToolActionID( x, y ) == m_current1 )
    {
      Enter( TRUE );
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::OnKeyDown( UINT vk, BOOL fDown, int cRepeat, UINT flags )
{
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::SetColumn( int column )
{
  if ( column < 1 )
    {
      column = 1;
    }
  else if ( column > COLUMN_MAX )
    {
      column = COLUMN_MAX;
    }

  m_pay.column = column;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::DrawLine( HDC hDC, int no )
{
  xTmpDC    dc( hDC );
  TCHAR*    buf;
  TCHAR     tmp[ MAX_PATH ];
  TCHAR*    cp;
  dirent*   entry;
  COLORREF  cols[ 5 ];
  int       lens[ 5 ];
  BOOL      enable = ( m_active && !m_lock );
  int       offset;
  int       lines = m_pay.h + 10;
  int       yyy1 = 0;
  int       yyy2 = 0;
  iPt       pt;
  iico      ico(dc, IMG_COPYING);
  size_t    len;

  assert(dc.tm.tmMaxCharWidth);

  entry = m_namelist[ no ];

  if ( no == mCopeingFileIndex )
    {
      cols[ 0 ] = Brend( FormatColor( entry ), 60 );
      cols[ 1 ] = cols[ 0 ];
      cols[ 2 ] = cols[ 0 ];
      cols[ 3 ] = cols[ 0 ];
    }
  else
    {
      if ( enable && m_focus )
        {
          cols[ 0 ] = FormatColor( entry );
          cols[ 1 ] = Brend( cols[ 0 ], 60 );
          cols[ 2 ] = FormatColor( entry );
          cols[ 3 ] = Brend( cols[ 2 ], 60 );
        }
      else
        {
          cols[ 0 ] = Brend( FormatColor( entry ), 60 );
          cols[ 1 ] = cols[ 0 ];
          cols[ 2 ] = cols[ 0 ];
          cols[ 3 ] = cols[ 0 ];
        }
    }

  dc.FillRect( 0, 0, m_pay.w, lines, pt.COL_BASE_BAR );

  if ( m_current1 == no )
    {
      if ( enable && m_focus )
        {
          FillRect( dc, 0, 0, m_pay.w, m_pay.h, pt.COL_SELECTED, pt.COL_SELECTED_WAKU );
        }
      else
        {
          FillRect( dc, 0, 0, m_pay.w, m_pay.h, pt.COL_SELECTED_CUR );
        }
    }
  else if ( m_current2 == no )
    {
      if ( m_current1 == no + 1 )
        {
          yyy1 =  1;
        }
      else if( m_current1 == no - 1 )
        {
          yyy1 =  1;
          yyy2 = -1;
        }

      if ( entry->bookMark )
        {
          FillRect( dc, 0, yyy2, m_pay.w, m_pay.h + yyy1, Brend( pt.COL_SELECTED_CUR, 60 ) );
        }
      else
        {
          FillRect( dc, 0, yyy2, m_pay.w, m_pay.h + yyy1, pt.COL_SELECTED_CUR );
        }
    }
  else
    {
      if ( entry->/*dwReserved0*/state & 1 )
        {
          dc.FillRect(0, 0, m_pay.w, lines, pt.COL_BASE_BAR_LOW);
        }
      else
        {
          dc.FillRect( 0, 0, m_pay.w, lines, pt.COL_BASE_BAR );
        }

      if( entry->bookMark )
        {
          if ( no + 1 < m_total )
            {
              if ( m_namelist[ no + 1 ]->bookMark || m_current1 == no + 1 || m_current2 == no + 1 )
                {
                  yyy1 = 1;
                }
            }

          if ( no - 1 >= 0 && ( m_current1 == no - 1 || m_current2 == no - 1 ) )
            {
              yyy2 = -1;
              yyy1++;
            }

          FillRect( dc, 0, yyy2, m_pay.w, m_pay.h + yyy1, pt.COL_SELECTED_CUR );
        }
    }

  len = m_monitor.right / dc.tm.tmAveCharWidth + 1;

  buf = (TCHAR *)alloca(sizeof(TCHAR) * len);

  if ( m_seach_mode == INAM_MODE_NORMAL )
    {
      if ( no == mCopeingFileIndex )
        {
          ico.draw(OFFSET_ICON, ( m_pay.h - ICON_SIZE ) / 2, entry, FALSE, m_isRemote );

          ico.draw(OFFSET_ICON, ( m_pay.h - ICON_SIZE ), mCopeingCounter);

          _stprintf_s( buf, len, L"%s  Copying ... ", PathFindFileName( entry->d_name ) );
          dc.SetTextColor( cols[ 0 ] );
          dc.ExtTextOut( STR_LEFT, m_pay.str_top, buf, dc.tm.tmAveCharWidth );
        }
      else
        {
          ico.draw(OFFSET_ICON, ( m_pay.h - ICON_SIZE ) / 2, entry, ( enable && m_focus ), m_isRemote);

          lens[ 0 ] = FormatInc( buf, entry, m_pay.len, 50, &offset );
          lens[ 1 ] = m_pay.len - lens[ 0 ];

          ExtTextOut2( dc, STR_LEFT, m_pay.str_top, buf, 2, lens, cols, m_pay.points, offset );
        }
    }
  else
    {
      ico.draw(OFFSET_ICON, ( m_pay.h / 2 - ICON_SIZE ) / 2, entry, ( enable && m_focus ), m_isRemote);

      lens[ 0 ] = FormatInc( buf, entry, m_pay.len, 50, &offset );
      lens[ 1 ] = m_pay.len - lens[ 0 ];

      ExtTextOut2( dc, STR_LEFT, m_pay.str_top, buf, 2, lens, cols, m_pay.points, offset );

      _tcscpy_s( tmp, MAX_PATH, entry->d_name + _tcslen( m_dir ) );
      cp = PathFindFileName( tmp );
      *cp = '\0';
      StrTrim( tmp, _T( "\\" ) );
      PathCompactPathEx( buf, tmp, m_pay.len, 0 );

      if ( buf[ 0 ] == _T( '\0' ) )
        {
          _tcscpy_s(buf, len, _T("This folder"));
        }

      dc.SetTextColor( Brend( FormatColor( entry ), 60 ) );
      dc.TextOut( STR_LEFT + 2, m_pay.str_top + m_pay.h / 2, buf );
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL iSeachView::Scan( const TCHAR *dir, const TCHAR *prev_dir, int seach_mode, int on_msg )
{
  int     current;
  size_t  len;
  int     i;
  int     state;
  TCHAR  *path;
  TCHAR  *buf;
  BOOL    res  = FALSE;
  size_t  size = 0;

  if ( mp_bmpinfo )
    {
      GlobalFree( mp_bmpinfo );
      mp_bmpinfo = NULL;
    }

  StopNotification();

  len = _tcslen(dir) + 1/* Backslash */ + 1/* Null */;

  path = (TCHAR *)alloca(len * sizeof(TCHAR));

  _tcscpy_s(path, len, dir);

  PathAddBackslash( path );

  switch ( CheckPath( path, 5, mhWnd, on_msg ) )
    {
      case 0:
      case 1:
        if ( RootCmp( path ) || m_dir[ 0 ] == _T( '\0' ) )
          {
            _tcscpy_s( path, len, _desktop_folder );
          }
        else
          {
            goto _EXIT;
          }
        break;
      case 2:
        if ( RootCmp( path ) )
          {
            PathStripToRoot( path );
          }
        else
          {
            goto _EXIT;
          }
        break;
      case 3:
      case 4:
        goto _EXIT;
      default:
        res = TRUE;
        break;
    }

  if (m_lock)
    {
      goto _EXIT;
    }

  current = Scandir( path, prev_dir, mhWnd, seach_mode );

  if ( current < 0 )
    {
      const TCHAR *msg = L"\"%s\"\r\n\r\nFolder search has been canceled.\r\n%s";
      const TCHAR *err = L"";

      len = _tcslen(msg) + _tcslen(err) + _tcslen(path) + 1/* Null */;

      buf = (TCHAR *)alloca(len * sizeof(TCHAR));

      wsprintf(buf, msg, path, err);

      MessageBox( mhWnd, buf, _T( "Interruption!" ), MB_ICONINFORMATION );

      goto _EXIT;
    }

    m_current1 = current;
    m_current2 = -1;

    if ( m_total == 0 )
      {
        if ( ::PathIsRoot( path ) )
          {
            _tcscpy_s( mEmptyMessage, MAX_PATH, _T( "*** This media is empty ***" ) );
          }
        else
          {
            _tcscpy_s( mEmptyMessage, MAX_PATH, _T( "*** Can not access ***" ) );
          }

        goto _EXIT;
      }

  /* Calculate the size of the buffer to reserve */

  for ( i = 0; i < m_total; i++ )
    {
      len = PathFindFileName( m_namelist[ i ]->d_name ) - m_namelist[ i ]->d_name;

      if (len > size)
        {
          size = len;
        }
    }

  size++;

  buf = (TCHAR *)alloca(size * sizeof(TCHAR));

  state = 0;
  m_namelist[ 0 ]->state = state;

  len = PathFindFileName( m_namelist[ 0 ]->d_name ) - m_namelist[ 0 ]->d_name;

  _tcsncpy_s( buf, size, m_namelist[ 0 ]->d_name, len );

  for ( i = 1; i < m_total; i++ )
    {
      len = PathFindFileName( m_namelist[ i ]->d_name ) - m_namelist[ i ]->d_name;

      if ( _tcsnicmp( buf, m_namelist[ i ]->d_name, len - 1 ) != 0 )
        {
          _tcsncpy_s( buf, size, m_namelist[ i ]->d_name, len );
          state++;
        }

      m_namelist[ i ]->state = state;
    }

_EXIT:

  /* フォルダーの監視を開始 */

  StartNotification();

  StartFileSeach();

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::SetCurrentLine1( int no )
{
  if ( m_total > 0 )
    {
      if ( 0 > no )
        {
          no = 0;
        }
      else if ( no >= m_total )
        {
          no = m_total - 1;
        }

      if ( m_current1 / m_pay.line_per_page != no / m_pay.line_per_page )
        {
          m_current1 = no;
          m_current2 = -1;
          Redraw();
        }
      else
        {
          SetCurrentLine( &m_current1, no );
        }
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::SetCurrentLine2( int no )
{
  SetCurrentLine( &m_current2, no );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::SetCurrentLine( int *target, int no )
{
  int     line_no;
  int     i;
  xDC     dc( mhWnd );
  xBmpDC  bmp;
  struct {
    int   no;
    DWORD bm;   /* 0 to browse bookmarks */
  }
  lines[ 6 ];

  if ( m_pay.line_per_page && m_pay.lines && m_pay.column )
    {
      line_no = m_current1 / m_pay.line_per_page * m_pay.line_per_page;
    }
  else
    {
      return;
    }

  bmp.Create( dc, m_pay.w, m_pay.h );
  bmp.SetSelectFont( m_fontname, m_fontsize );
  bmp.SetBkMode( TRANSPARENT );

  lines[ 0 ].no = *target;
  lines[ 1 ].no = *target + 1;
  lines[ 2 ].no = *target - 1;
  lines[ 3 ].no = no;
  lines[ 4 ].no = no - 1;
  lines[ 5 ].no = no + 1;
  lines[ 0 ].bm = 1;
  lines[ 1 ].bm = 0;
  lines[ 2 ].bm = 0;
  lines[ 3 ].bm = 1;
  lines[ 4 ].bm = 0;
  lines[ 5 ].bm = 0;

  *target  = no;

  for ( i = 0; i < 6; i++ )
    {
      if ( 0 > lines[ i ].no || lines[ i ].no >= m_total )
        {
          continue;
        }

      if ( lines[ i ].bm == 0 )
        {
          lines[ i ].bm = m_namelist[ lines[ i ].no ]->bookMark;
        }

      if ( lines[ i ].bm == 0 )
        {
          continue;
        }

      if ( lines[ i ].no < line_no )
        {
          continue;
        }

      DrawLine( bmp, lines[ i ].no );
      BitBlt3( dc, bmp, m_pay.x[ ( lines[ i ].no / m_pay.lines ) % m_pay.column ], m_pay.y[ lines[ i ].no % m_pay.lines ], m_pay.w, m_pay.h );
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::SetCurrentLine( int no )
{
  xDC     dc( mhWnd );
  xBmpDC  bmp;
  int     line_start;
  int     line_end;

  bmp.Create( dc, m_pay.w, m_pay.h );
  bmp.SetSelectFont( m_fontname, m_fontsize );
  bmp.SetBkMode( TRANSPARENT );

  line_start = m_current1 / m_pay.line_per_page * m_pay.line_per_page;
  line_end   = line_start + m_pay.line_per_page;

  if ( line_end > m_total )
    {
      line_end = m_total;
    }

  if ( no != -1 && line_start <= no && no < line_end )
    {
      DrawLine( bmp, no );
      BitBlt3( dc, bmp, m_pay.x[ ( no / m_pay.lines ) % m_pay.column ], m_pay.y[ no % m_pay.lines ], m_pay.w, m_pay.h );
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::Enter( BOOL dblclk )
{
  dirent* entry;
  TCHAR   buf[ MAX_PATH ];
  TCHAR   back_path[ MAX_PATH ];
  TCHAR   cp[MAX_PATH];
  DWORD   attr;
  int     change = 0;

  if ( m_total == 0 )
    {
      return;
    }

  entry = m_namelist[ m_current1 ];

  if ( entry->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
    {
      if ( _tcscmp( PathFindFileName( entry->d_name ), _T( ".." ) ) == 0 )
        {
          BackSpase();
        }
      else
        {
          _tcscpy_s( cp, MAX_PATH, entry->d_name );

          if ( Scan( cp, NULL, -1, 1 ) )
            {
              Push( cp );
            }
          Redraw();
        }
    }
  else
    {
      if ( mp_bmpinfo )
        {
          GlobalFree( mp_bmpinfo );
          mp_bmpinfo = NULL;
          Redraw();
          change = 1;
        }
      else
        {
          if ( ( mp_bmpinfo = LoadImage( entry ) ) != NULL )
            {
              m_disp_recalc = 0;
              Redraw();
              change = 1;
            }
          else
            {
              do
                {
                  if ( !GetShortcutInfo( entry->d_name, buf, &attr ) )
                    {
                      break;
                    }

                  if ( !( attr & FILE_ATTRIBUTE_DIRECTORY ) )
                    {
                      break;
                    }

                  change = 1;

                  if ( CheckPath( buf, 2, mhWnd, 1 ) < 2 )
                    {
                      break;
                    }

                  if ( !PathFileExists( buf ) )
                    {
                      if ( ::MessageBox( mhWnd, _T( "There is no folder. Do you make it?" ), _T( "What are you doing?" ), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 ) == IDNO )
                        {
                          break;
                        }
                      PathAddBackslash( buf );
                      SHCreateDirectoryEx( mhWnd, buf, 0 );
                    }

                  _tcscpy_s( back_path, MAX_PATH, entry->d_name );

                  if ( Scan( buf ) )
                    {
                      Push( buf, back_path );
                    }

                  Redraw();
                }
              while( 0 );
            }
        }

      if ( change == 0 && dblclk )
        {
          ShellExecute( entry->d_name );
        }
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL iSeachView::SetDirectory2( const TCHAR *d_name, const TCHAR *prev_name, int on_msg )
{
  TCHAR   cp[ MAX_PATH ];
  BOOL    res = FALSE;
  int     current;

  if ( d_name == NULL )
    {
      if ( m_total > 0 )
        {
          if ( m_namelist )
            {
              /* Refresh */

              current = m_current1;

              res = Scan( m_dir, m_namelist[ m_current1 ]->d_name );

              if ( m_current1 == 0 && current != 0 )
                {
                  if ( m_total > current )
                    {
                      m_current1 = current;
                    }
                  else
                    {
                      m_current1 = m_total - 1;
                    }
                }
            }
        }
      else
        {
          res = Scan( m_dir );
        }
    }
  else
    {
      if( _tcscmp( PathFindFileName( d_name ), _T( ".." ) ) == 0 )
        {
          res = BackSpase();
        }
      else
        {
          _tcscpy_s( cp, MAX_PATH, d_name );

          res = Scan( cp, prev_name, -1, on_msg );

          if ( res )
            {
              Push( cp );
            }
        }
    }

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- バックスペイス
// --------------------------------------------------------------------------
BOOL iSeachView::BackSpase( void )
{
  TCHAR   dir_cp[MAX_PATH];
  TCHAR   name[ MAX_PATH ];
  BOOL    res = FALSE;

  if ( m_seach_mode == INAM_MODE_SEACH )
    {
      SelectSeachMode();
    }
  else
    {
      if ( !::PathIsRoot( m_dir ) )
        {
          /* Do nothing if it is the root directory */

          _tcscpy_s( name, MAX_PATH, m_dir );
          PathRemoveBackslash( name );

          PathCombine( dir_cp, m_dir, _T( ".." ) );

          res = Scan( dir_cp, name, INAM_MODE_NORMAL );
          Redraw();
        }
    }

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::GotoRoot( void )
{
  TCHAR*  cp;
  TCHAR   buf[MAX_PATH];
  int     i;
  int     depth;
  TCHAR   path[ MAX_PATH ];

  if ( ::PathIsRoot( m_dir ) )
    {
      /* Do nothing if root directory */

      return;
    }

  for ( depth = 0, cp = m_dir; cp; depth++, cp = PathFindNextComponent( cp ) )
    {
      ;
    }

  _tcscpy_s(path, MAX_PATH, m_dir);

  for ( i = 0; i < depth; i++ )
    {
      PathCombine( buf, path, _T( ".." ) );

      if ( ::PathIsRoot( buf ) )
        {
          Scan( buf, path );
          Redraw();
          break;
        }

      _tcscpy_s(path, MAX_PATH, buf);
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iSeachView::GetTotalPage( void )
{
  if ( m_pay.line_per_page == 0 )
    {
      return 0;
    }

  return ( m_total - 1 ) / m_pay.line_per_page;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iSeachView::GetCurrentPage( void )
{
  if ( m_pay.line_per_page == 0 )
    {
      return 0;
    }

  return m_current1 / m_pay.line_per_page;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::SelectSeachMode( BOOL force )
{
  TCHAR  *cp;
  TCHAR  *name;
  size_t  len;

  if (!(m_total > 0))
    {
      return;
    }

  name = m_namelist[ m_current1 ]->d_name;

  len = _tcslen(name) + 1;

  cp = (TCHAR *)alloca(len * sizeof(TCHAR));

  _tcscpy_s( cp, len, name );

  PathRemoveFileSpec( cp );

  m_seaching = TRUE;
  Redraw();

  if ( !force )
    {
      if ( m_seach_mode == INAM_MODE_SEACH )
        {
          Scan( cp, name, INAM_MODE_NORMAL );
        }
      else if ( m_seach_mode == INAM_MODE_NORMAL )
        {
          Scan( m_dir, name, INAM_MODE_SEACH );
        }
    }
  else
    {
      Scan( m_dir, name, INAM_MODE_SEACH );
    }

  m_seaching = FALSE;
  ZeroMemory( mSeachingFolderName, sizeof( mSeachingFolderName ) );

  Redraw();
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::BitBlt3( HDC hDes, HDC hSrc, int x, int y, int cx, int cy )
{
  xTmpDC  dc( hDes );

  if ( CheckRect( m_x + 3, m_y + 3, m_cx - 6, m_cy - 7, &x, &y, &cx, &cy ) )
    {
      dc.BitBlt( x, y, cx, cy, hSrc, 0, 0, SRCCOPY );
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::FillRect3( HDC hDC, int x, int y, int cx, int cy, COLORREF col )
{
  xTmpDC  dc( hDC );

  if ( CheckRect( m_x + 3, m_y + 3, m_cx - 6, m_cy - 7, &x, &y, &cx, &cy ) )
    {
      dc.FillRect( x, y, cx, cy, col );
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::FillRect3( HDC hDC, RECT *rc, COLORREF col )
{
  FillRect3( hDC, rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top, col );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::SetMark( int inc )
{
  dirent* entry;

  if( m_total > 0 )
    {
      entry = m_namelist[ m_current1 ];

      if ( _tcscmp( PathFindFileName( entry->d_name ), _T( ".." ) ) != 0 )
        {
          if ( entry->bookMark )
            {
              entry->bookMark = 0;
            }
          else
            {
              entry->bookMark = 1;
            }
        }

      SetCurrentLine1( m_current1 + inc );
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iSeachView::GetMarkedNum( dirent** entrylist )
{
  dirent* entry;
  dirent* top_entry = NULL;
  int     i;
  int     num = 0;

  for ( i = 0; i < m_total; i++ )
    {
      entry = m_namelist[ i ];

      if( !entry->bookMark )
        {
          continue;
        }

      entry->next = top_entry;
      top_entry = entry;
      num++;
    }

  if ( num == 0 && m_total > 0 && entrylist )
    {
      entry = m_namelist[ m_current1 ];

      if ( _tcscmp( PathFindFileName( entry->d_name ), _T( ".." ) ) != 0 )
        {
          top_entry = entry;
          top_entry->next = NULL;
          num = 1;
        }
    }

  if ( entrylist )
    {
      *entrylist = top_entry;
    }

  return num;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iSeachView::GetMarkedInfo( int *d_cnt, int *f_cnt, ULARGE_INTEGER *f_size )
{
  int             i;
  int             num = 0;
  ULARGE_INTEGER  size;

  *f_cnt           = 0;
  *d_cnt           = 0;
  f_size->QuadPart = 0;

  for ( i = 0; i < m_total; i++ )
    {
      if( !m_namelist[ i ]->bookMark )
        {
          continue;
        }

      if ( m_namelist[ i ]->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
          ( *d_cnt )++;
        }
      else
        {
          ( *f_cnt )++;
        }

      size.LowPart  = m_namelist[ i ]->nFileSizeLow;
      size.HighPart = m_namelist[ i ]->nFileSizeHigh;
      f_size->QuadPart += size.QuadPart;
      num++;
    }

  return num;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iSeachView::AllMark( int mark_mode, BOOL control )
{
  int     num = 0;
  int     i;
  dirent* entry;

  for( i = 0; i < m_total; i++ )
    {
      if ( m_namelist[ i ]->bookMark )
        {
          m_namelist[ i ]->bookMark = 0;
          num++;
        }
    }

  if ( num == 0 )
    {
      for ( i = 0; i < m_total; i++ )
        {
          entry = m_namelist[ i ];

          if ( _tcscmp( PathFindFileName( entry->d_name ), _T( ".." ) ) == 0 )
            {
              continue;
            }

          if ( !control && mark_mode == VK_HOME && ( entry->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
            {
              continue;
            }

          if ( control && mark_mode == VK_END && (entry->userData & 0xFF000000) )
            {
              continue;
            }

          if ( control && mark_mode == VK_HOME && !(entry->userData & 0xFF000000) )
            {
              continue;
            }

          entry->bookMark = 1;
        }
    }

  Redraw();

  return num;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
dirent *iSeachView::GetCurrentEntry( void )
{
  dirent* entry;

  if ( m_total > 0 )
    {
      entry = m_namelist[ m_current1 ];
    }
  else
    {
      ZeroMemory( &m_tmp, sizeof( m_tmp ) );
      StrCpyN( m_tmp.d_name, m_dir, MAX_PATH );
      PathStripToRoot( m_tmp.d_name );
      PathAppend( m_tmp.d_name, _T( "\\" ) );
      m_tmp.cAlternateFileName[ 0 ] = ( TCHAR )-1;
      entry = &m_tmp;
    }

  return entry;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::SetEnable( BOOL enable )
{
  m_active = enable;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::PageUp( void )
{
  if ( GetEnableAction() )
    {
      if ( mp_bmpinfo )
        {
          if ( m_disp_hold || HIBYTE( GetKeyState( VK_CONTROL ) ) )
            {
              m_disp_rate += 20;

              if ( m_disp_rate > 800 )
                {
                  m_disp_rate = 800;
                }

              Redraw();
            }
          else
            {
              KeyDown( VK_UP, 0, 0, 0 );
            }
        }
    }
  else
    {
      if ( m_current1 - m_pay.line_per_page >= 0 )
        {
          m_current1 = m_current1 - m_pay.line_per_page;
        }
      else
        {
          m_current1 = 0;
        }

      m_current2 = -1;
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::PageDown( void )
{
  if ( GetEnableAction() )
    {
      if ( mp_bmpinfo )
        {
          if ( m_disp_hold || HIBYTE( GetKeyState( VK_CONTROL ) ) )
            {
              m_disp_rate -= 20;

              if ( m_disp_rate <= m_disp_min_rate )
                {
                  m_disp_rate = m_disp_min_rate;
                }

              Redraw();
            }
          else
            {
              KeyDown( VK_DOWN, 0, 0, 0 );
            }
        }
    }
  else
    {
      if ( m_current1 + m_pay.line_per_page < m_total )
        {
          m_current1 = m_current1 + m_pay.line_per_page;
        }
      else
        {
          m_current1 = m_total - 1;
        }

      m_current2 = -1;
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::PagePos( int pos )
{
  int   current;

  if ( m_pay.line_per_page )
    {
      current = m_pay.line_per_page * pos + m_current1 % m_pay.line_per_page;

      if ( current < m_total )
        {
          m_current1 = current;
        }
    }

  m_current2 = -1;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL iSeachView::KeyDown( int vk )
{
  switch( vk )
    {
      case VK_BACK:
      case VK_ESCAPE:
      case VK_RETURN:
        GlobalFree( mp_bmpinfo );
        mp_bmpinfo = NULL;
        Redraw();
        break;

      case VK_SPACE:
        if ( m_namelist[ m_current1 ]->bookMark )
          {
            m_namelist[ m_current1 ]->bookMark = 0;
          }
        else
          {
            m_namelist[ m_current1 ]->bookMark = 1;
          }
        break;

      case VK_UP:
        GlobalFree( mp_bmpinfo );
        mp_bmpinfo = NULL;
        m_disp_recalc = 0;

        do
          {
            m_current1--;

            if ( m_current1 < 0 )
              {
                m_current1 = m_total - 1;
              }

            mp_bmpinfo = LoadImage( m_namelist[ m_current1 ] );
          }
        while( mp_bmpinfo == NULL );

        Redraw();
        break;

      case VK_DOWN:
        GlobalFree( mp_bmpinfo );
        mp_bmpinfo = NULL;
        m_disp_recalc = 0;

        do
          {
            m_current1++;

            if ( m_current1 >= m_total )
              {
                m_current1 = 0;
              }

            mp_bmpinfo = LoadImage( m_namelist[ m_current1 ] );
          }
        while( mp_bmpinfo == NULL );

        Redraw();
        break;

      default:
        break;
    }

  return TRUE;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL iSeachView::KeyDownSub( int vk, int ctrl, int shift, int alt )
{
  BOOL  res = FALSE;

  switch ( vk )
    {
      case VK_UP:
        if ( shift > 0 )
          {
            SetMark( - 1 );
          }
        else
          {
            SetCurrentLine1( m_current1 - 1 );
          }
        break;
      case VK_DOWN:
        if ( shift > 0 )
          {
            SetMark( 1 );
          }
        else
          {
            SetCurrentLine1( m_current1 + 1 );
          }
        break;
      case VK_LEFT:
        if ( shift > 0 )
          {
            SetMark( -m_pay.lines );
          }
        else
          {
            SetCurrentLine1( m_current1 - m_pay.lines );
          }
        break;
      case VK_RIGHT:
        if ( shift > 0 )
          {
            SetMark( m_pay.lines );
          }
        else
          {
            SetCurrentLine1( m_current1 + m_pay.lines );
          }
        break;
      case VK_RETURN:
        if ( shift == 0 && ctrl == 0 && alt == 0 )
          {
            Enter();
          }
        break;
      case VK_PRIOR:
        if ( shift == 0 && ctrl == 0 && alt == 0 )
          {
            SetCurrentLine1( m_current1 - m_pay.line_per_page );
          }
        break;
      case VK_NEXT:
        if ( shift == 0 && ctrl == 0 && alt == 0 )
          {
            SetCurrentLine1( m_current1 + m_pay.line_per_page );
          }
        break;
      case VK_BACK:
        BackSpase();
        break;
      case 0xDC:
        GotoRoot();
        break;
      case VK_SPACE:
        SetMark();
        break;
      case VK_ESCAPE:
        if ( m_seach_mode == INAM_MODE_SEACH )
          {
            Scan( m_dir, NULL, INAM_MODE_NORMAL );
            Redraw();
          }
        break;
      case VK_HOME:
      case VK_END:
        AllMark( vk, ctrl > 0 );
        break;

      case 0xBF:
        ::KillTimer( mhWnd, ISEACH_TIMERID );
        ::SetTimer( mhWnd, ISEACH_TIMERID, 15000, NULL );
        mb_find = TRUE;
        break;

      default:
        res = TRUE;
        break;
    }

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL iSeachView::KeyDown( int vk, int ctrl, int shift, int alt )
{
  BOOL  res = FALSE;

  if ( m_seaching )
    {
      res = TRUE;
    }
  else if ( mp_bmpinfo )
    {
      res = KeyDown( vk );
    }
  else if ( mb_find )
    {
      ::KillTimer( mhWnd, ISEACH_TIMERID );

      if ( ( vk >= 'A' && vk <= 'Z' ) || vk == '.' || vk == '[' || vk == ']' )
        {
          ::SetTimer( mhWnd, ISEACH_TIMERID, 15000, NULL );
        }
      else
        {
          mb_find = FALSE;
        }

      res = TRUE;
    }
  else
    {
      res = KeyDownSub(vk, ctrl, shift, alt);
    }

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::GetCurrentPosition( int *x, int *y )
{
  *x = m_pay.x[ ( m_current1 / m_pay.lines ) % m_pay.column ] + STR_LEFT;
  *y = m_pay.y[ m_current1 % m_pay.lines ] + ICON_SIZE + OFFSET_2;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- フォルダー内の変化検出
// --------------------------------------------------------------------------
int iSeachView::IsChangeFolderState( void )
{
  int   res = 0;

  switch ( WaitForSingleObject( mh_handle, 0 ) )
    {
      case WAIT_OBJECT_0:
        res = 1;
        break;
      case WAIT_ABANDONED:
        break;
      case WAIT_TIMEOUT:
        break;
      default:
        break;
    }

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- Start folder monitoring (Close and reset if already set)
// --------------------------------------------------------------------------
void iSeachView::StartNotification( void )
{
  StopNotification();

  mh_handle = FindFirstChangeNotification(
    m_dir,
    FALSE,
      FILE_NOTIFY_CHANGE_FILE_NAME    // Add / delete files, change file names
    | FILE_NOTIFY_CHANGE_DIR_NAME     // Add / remove directories, change directory names
    | FILE_NOTIFY_CHANGE_ATTRIBUTES   // Attribute changes
    | FILE_NOTIFY_CHANGE_SIZE         // Change size
    | FILE_NOTIFY_CHANGE_LAST_WRITE   // Change last write time
//  | FILE_NOTIFY_CHANGE_LAST_ACCESS
    | FILE_NOTIFY_CHANGE_CREATION
    | FILE_NOTIFY_CHANGE_SECURITY     // Changing security attributes
  );
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSeachView::StopNotification( void )
{
  if ( mh_handle )
    {
      FindCloseChangeNotification( mh_handle );
      mh_handle = NULL;
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
TCHAR *iSeachView::GetSelected( void )
{
  return m_selected;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSeachView::StartFileSeach( void )
{
  mp_lastUpdateFile = NULL;
  m_seach_count     = 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSeachView::ClearDateTime( TCHAR *d_name )
{
  TCHAR  *buf;
  D_NODE *d_node;
  size_t  len;

  len = _tcslen(d_name) + 1;

  buf = (TCHAR *)alloca(len * sizeof(TCHAR));

  _tcscpy_s(buf, len, d_name);

  do
    {
      PathRemoveFileSpec(buf);

      if (::PathIsRoot(buf))
        {
          break;
        }

      d_node = savedir(buf);

      d_node->htime = 0;
      d_node->ltime = 0;
    }
  while(1);
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iSeachView::FileSeach( void )
{
  dirent*   entry;
  D_NODE*   d_node;
  FILETIME  temp;
  int       res = 0;
  int       i   = 0;
  iico      ico;

  while( i < 1 )
    {
      if ( m_seach_count < 0 || m_total == 0 )
        {
          break;
        }

      if ( m_seach_count >= m_total )
        {
          if (mp_lastUpdateFile)
            {
              ClearDateTime(mp_lastUpdateFile);
            }

          Sort();
          Redraw();

          /* Clear parameter*/

          mp_lastUpdateFile = NULL;
          mb_forceUpdate    = FALSE;
          m_seach_count     = -1;
          break;
        }

      entry = m_namelist[ m_seach_count++ ];

      if ( _tcscmp( _T( ".." ), PathFindFileName( entry->d_name ) ) == 0 )
        {
          continue;
        }

      d_node = savedir( entry->d_name );

      if ( d_node == NULL )
        {
          break;
        }

      temp.dwHighDateTime = d_node->htime;
      temp.dwLowDateTime  = d_node->ltime;

      if ( mb_forceUpdate || CompareFileTime( &entry->ftLastWriteTime, &temp ) > 0 )
        {
          d_node->hHandle = (HIMAGELIST)ico.get_instance(entry, &d_node->d_no, m_isRemote);

          d_node->htime = entry->ftLastWriteTime.dwHighDateTime;
          d_node->ltime = entry->ftLastWriteTime.dwLowDateTime;

          mp_lastUpdateFile = entry->d_name;
        }

      entry->hHandle  = d_node->hHandle;
      entry->userData = d_node->d_no;

      i++;
      res = 1;

      if ( m_pay.line_per_page && ( m_seach_count % m_pay.line_per_page ) == 0 )
        {
          Redraw();
        }
    }

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
dirent *iSeachView::FindDirent( const TCHAR *name )
{
  dirent*   entry = NULL;
  int       i;
  size_t    len = 0;

  for ( i = 0; i < m_total; i++ )
    {
      if ( i == 0 )
        {
          len = PathFindFileName( m_namelist[ i ]->d_name ) - m_namelist[ i ]->d_name;
        }

      if ( _tcsicmp( m_namelist[ i ]->d_name + len, name ) == 0 )
        {
          entry = m_namelist[ i ];
          break;
        }
    }

  return entry;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSeachView::MoveNextMark( int current )
{
  int   i;

  if ( current < 0 )
    {
      current = m_current1;
    }

  for ( i = current + 1; i < m_total; i++ )
    {
      if ( m_namelist[ i ]->bookMark )
        {
          m_current1 = i;
          break;
        }
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSeachView::SetCopeingFile( TCHAR *path )
{
  int   i;

  if ( path == NULL )
    {
      mCopeingFileName[ 0 ] = '\0';
      mCopeingFileIndex     = -1;
    }
  else if ( mCopeingFileIndex < 0 )
    {
      for ( i = 0; i < m_total; i++ )
        {
          if ( _tcsstr( path, m_namelist[ i ]->d_name ) )
            {
              _tcscpy_s(mCopeingFileName, MAX_PATH, path);
              mCopeingFileIndex = i;
              SetCurrentLine( mCopeingFileIndex );
              break;
            }
        }
    }
  else if ( mCopeingFileIndex >= 0 )
    {
      mCopeingCounter++;
      mCopeingCounter %= 16;

      if ( _tcsicmp( mCopeingFileName, path ) != 0 )
        {
          for ( mCopeingFileIndex = -1, i = 0; i < m_total; i++ )
            {
              if ( _tcsstr( path, m_namelist[ i ]->d_name ) )
                {
                  _tcscpy_s( mCopeingFileName, MAX_PATH, path );
                  mCopeingFileIndex = i;
                  break;
                }
            }
          Redraw();
        }
      else
        {
          SetCurrentLine( mCopeingFileIndex );
        }
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iSeachView::GetEnableAction( void )
{
  if ( mp_bmpinfo || m_seaching )
    {
      return TRUE;
    }

  return FALSE;
}
// --------------------------------------------------------------------------

// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSeachView::NamelistAction( void *param )
{
  TACTION*      action = ( TACTION* )param;
  xDC*          p_dc;
  static size_t len = 0;

  if( action->s_mode == INAM_MODE_SEACH )
    {
      if ( action->time == 0 )
        {
          action->time = ::GetTickCount();
          len = _tcslen( m_dir );
          ZeroMemory( mSeachingFolderName, sizeof( mSeachingFolderName ) );
        }
      else if ( ::GetTickCount() - action->time > 100 )
        {
          if( action->entry )
            {
              if ( *( action->entry->d_name + len ) == '\\' )
                {
                  len++;
                }

              _tcscpy_s( mSeachingFolderName, MAX_PATH, action->entry->d_name + len );
            }

          mSeachingIconState++;
          mSeachingIconState %= 16;
          action->time += 100;

          if ( mhDC )
            {
              p_dc = new xDC( mhWnd );
              p_dc->SetSelectFont( m_fontname, m_fontsize );
              DrawForSeachingString( *p_dc, m_x, m_y, m_cx, m_cy );
              delete p_dc;
            }
        }
    }
  else
    {
      if ( action->time == 0 )
        {
          action->time = ::GetTickCount();
        }
      else
        {
          if ( ::GetTickCount() - action->time > 400 )
            {
              SeachLock();
              Redraw();
              action->time += 10000;
            }
        }
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSeachView::SetMarkFromNamelist( TCHAR *dir, dirent *namelist )
{
  dirent*   entry;
  dirent**  target;
  int       current = -1;

  for ( entry = namelist; entry; entry = entry->next )
    {
      if ( entry->bookMark != TCOPYBMARK_FAIL_IF_EXISTS
        && entry->bookMark != TCOPYBMARK_OVERWRITE
        && entry->bookMark != TCOPYBMARK_RENAME )
        {
          continue;
        }

      if ( ( target = BinSeach( dir, entry ) ) == NULL )
        {
          continue;
        }

      current = (int)( target - m_namelist );
      ( *target )->bookMark = 1;
    }

  if( current != -1 )
    {
      m_current1 = current;
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSeachView::OnSize( UINT state, int cx, int cy )
{
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSeachView::OnSizing( UINT nSide, LPRECT lpRect )
{
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSeachView::OnTimer( UINT id )
{
  mb_find = FALSE;
  ::KillTimer( mhWnd, ISEACH_TIMERID );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL iSeachView::PathIsRoot(void)
{
  return ::PathIsRoot( m_dir );
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSeachView::Sort(void)
{
  m_current1 = iNamelist::Sort(m_current1);
}
// --------------------------------------------------------------------------
