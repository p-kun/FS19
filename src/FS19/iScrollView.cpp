/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iScrollView.cpp
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
#include "xDcBase.h"
#include "iBaseView.h"
#include "iicon.h"
#include "timerid.h"
#include "iPt.h"
#include "iScrollView.h"

/* 定義値一覧です */

#define BUTTON_SIZE		    18
#define ICON_SIZE		      16
#define BUTTON_DPI_SIZE		DPIX(BUTTON_SIZE)
#define ICON_DPI_SIZE	    DPIX(ICON_SIZE)
#define TAB_MAX_SIZE	    32


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iScrollView::iScrollView( int base ) : iBaseView( base )
{
	m_prev_id  = -1;
	m_action   = -1;
	m_offset   = 0;
	m_position = 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iScrollView::Draw( HWND hWnd, HDC hDC, int x, int y, int cx, int cy )
{
	xTmpDC	dc( hDC );
	xBmpDC	bmp;
	iPt		  pt;

	bmp.Create( dc, cx, cy );

	SetMiRect( cx, cy );

	bmp.FillRect( 0, 0, cx, cy, pt.COL_BASE_BAR_LOW );

	DrawMain( bmp );

	dc.BitBlt( x, y, cx, cy, bmp, 0, 0, SRCCOPY );

	mhWnd = hWnd;
	mhDC  = bmp;
	m_x   = x;
	m_y   = y;
	m_cx  = cx;
	m_cy  = cy;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iScrollView::Redraw( void )
{
	if ( mhDC )
    {
  		xDC		dc( mhWnd );

      Draw( mhWnd, dc, m_x, m_y, m_cx, m_cy );
  	}
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iScrollView::DrawMain( HDC hDC )
{
	xTmpDC	dc( hDC );
	RECT	  rc;
	RECT	  rc1;
	RECT	  rc2;
	int		  offset = m_position;
	int		  tab;
	iPt		  pt;
  int     x = ( ICON_DPI_SIZE - ICON_SIZE ) / 2;
  int     y = ( BUTTON_DPI_SIZE - BUTTON_SIZE ) / 2;
  iico    ico(hDC, IMG_SCROLLVIEW);

	rc  = mi_lines[ 2 ].rc;
	rc1 = mi_lines[ 3 ].rc;
	rc2 = mi_lines[ 4 ].rc;

	if ( m_action == SB_THUMBPOSITION )
    {
  		if ( mi_lines[ 0 ].rc.bottom > offset )
    	  {
    			offset = mi_lines[ 0 ].rc.bottom;
    		}
  		else if ( mi_lines[ 1 ].rc.top < offset + ( rc.bottom - rc.top ) )
    	  {
    			offset = mi_lines[ 1 ].rc.top - ( rc.bottom - rc.top );
    		}

  		offset = offset - rc.top;

  		rc.top    += offset;
  		rc.bottom += offset;

  		rc1.bottom = rc.top;
  		rc2.top    = rc.bottom;

  		tab  = ( mi_lines[ 1 ].rc.top - mi_lines[ 0 ].rc.bottom ) / ( m_si.nMax - m_si.nMin + 1 );
  		m_pos = ( m_si.nMax - m_si.nMin + 1 ) * ( rc.top - mi_lines[ 0 ].rc.bottom ) / ( mi_lines[ 1 ].rc.top - mi_lines[ 0 ].rc.bottom - ( tab + TAB_MAX_SIZE ) );

  	  if ( m_pos > m_si.nMax - m_si.nMin )
    	  {
    			m_pos = m_si.nMax - m_si.nMin;
    		}
  	}

	rc.left   += 1;
	rc.top    += 1;
	rc.right  -= 2;
	rc.bottom -= 1;

	if ( m_action == SB_THUMBPOSITION )
    {
  		dc.FillRect( &rc, pt.COL_SCROLL_BAR_SEL );
  	}
	else
    {
  		dc.FillRect( &rc, pt.COL_SCROLL_BAR );
  	}

	rc = mi_lines[ 0 ].rc;

	if ( m_action == SB_LINEUP )
    {
  		rc.top    += 2;
  		rc.bottom -= 1;
  		rc.right  -= 2;
  	}
	else
    {
  		rc.top    += 1;
  		rc.bottom -= 2;
  		rc.right  -= 2;
  	}

	ico.draw(x, rc.top + y, 0, m_focus );

	rc = mi_lines[ 1 ].rc;

	if ( m_action == SB_LINEDOWN )
    {
  		rc.top    += 1;
  		rc.bottom -= 2;
  		rc.right  -= 2;
  	}
	else
    {
  		rc.bottom -= 3;
  		rc.right  -= 2;
  	}

  /* やっぱりアクセントはほしいよね */

  ico.draw(x, rc.top + y, 1, m_focus );

}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iScrollView::~iScrollView()
{
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iScrollView::SetMiRect( int cx, int cy )
{
	BOOL	res = TRUE;
	int		top;
	int		bottom;
	int		len1;
	int		len2;

	mi_lines[ 0 ].rc.left   = 0;
	mi_lines[ 0 ].rc.top    = 0;
	mi_lines[ 0 ].rc.right  = cx;
	mi_lines[ 0 ].rc.bottom = BUTTON_DPI_SIZE;

	mi_lines[ 1 ].rc.left   = 0;
	mi_lines[ 1 ].rc.top    = cy - BUTTON_DPI_SIZE;
	mi_lines[ 1 ].rc.right  = cx;
	mi_lines[ 1 ].rc.bottom = cy;

	mi_lines[ 0 ].id = SB_LINEUP;
	mi_lines[ 1 ].id = SB_LINEDOWN;
	mi_lines[ 2 ].id = SB_THUMBPOSITION;
	mi_lines[ 3 ].id = SB_PAGEUP;
	mi_lines[ 4 ].id = SB_PAGEDOWN;

	/* グリッド調整 */

  if ( m_si.nMin < m_si.nMax )
    {
  		len1 = mi_lines[ 1 ].rc.top - mi_lines[ 0 ].rc.bottom - ( TAB_MAX_SIZE );
  		len2 = m_si.nMax - m_si.nMin + 1;

  		top    = mi_lines[ 0 ].rc.bottom + m_si.nPos * ( len1 + 0 ) / len2;
  		bottom = top + len1 / len2 + ( TAB_MAX_SIZE );

  		/* グリッド */

      mi_lines[ 2 ].rc.top    = top;
  		mi_lines[ 2 ].rc.left   = 0;
  		mi_lines[ 2 ].rc.bottom = bottom;
  		mi_lines[ 2 ].rc.right  = cx;

      /* Bar1 */

      mi_lines[ 3 ].rc.left   = 0;
  		mi_lines[ 3 ].rc.top    = mi_lines[ 0 ].rc.bottom;
  		mi_lines[ 3 ].rc.right  = cx;
  		mi_lines[ 3 ].rc.bottom = mi_lines[ 2 ].rc.top;

      /* Bar2 */

      mi_lines[ 4 ].rc.left   = 0;
  		mi_lines[ 4 ].rc.top    = mi_lines[ 2 ].rc.bottom;
  		mi_lines[ 4 ].rc.right  = cx;
  		mi_lines[ 4 ].rc.bottom = mi_lines[ 1 ].rc.top;
  	}
	else
    {
  		/* Bar1 */

      mi_lines[ 3 ].rc.left   = 0;
  		mi_lines[ 3 ].rc.top    = mi_lines[ 0 ].rc.bottom;
  		mi_lines[ 3 ].rc.right  = cx;
  		mi_lines[ 3 ].rc.bottom = mi_lines[ 1 ].rc.top;

      /* Bar2 */

      mi_lines[ 4 ].rc = mi_lines[ 3 ].rc;
  		mi_lines[ 4 ].rc.bottom--;

      /* グリッド */

      mi_lines[ 2 ].rc = mi_lines[ 4 ].rc;

  		res = FALSE;
  	}

	return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iScrollView::GetToolActionID( int x, int y )
{
	int		i;
	int		target = -1;
	POINT	pos;

	pos.x = x - m_x;
	pos.y = y - m_y;

	for ( i = 0; i < TB_MAX_LINE; i++ )
    {
  		if ( PtInRect( &mi_lines[ i ].rc, pos ) )
    	  {
    			target = mi_lines[ i ].id;
    			break;
    		}
  	}

  return target;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iScrollView::OnMouseMove( int x, int y )
{
	if ( m_action == SB_THUMBPOSITION )
    {
  		m_position = y + m_offset;

  		Redraw();

  		::SendMessage( mhWnd, WM_VSCROLL, MAKEWPARAM( ( UINT )( int )( SB_THUMBTRACK ), ( UINT )( int )( m_pos ) ), ( LPARAM )( HWND )mhWnd );
  	}
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iScrollView::OnMouseLeave( void )
{
	m_prev_id = -1;
	Redraw();
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iScrollView::OnTimer( UINT id )
{
	if ( id == ISCROLL_TIMERID )
    {
  		::SetTimer( mhWnd, ISCROLL_TIMERID, 30, NULL );
  		::PostMessage( mhWnd, WM_VSCROLL, MAKEWPARAM( ( UINT )( int )( m_action ), ( UINT )( int )( m_si.nPos ) ), ( LPARAM )( HWND )mhWnd );
  	}
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iScrollView::OnLButtonDown( int x, int y )
{
	m_action = GetToolActionID( x, y );

	switch ( m_action )
    {
    	case SB_LINEDOWN:
    	case SB_PAGEDOWN:
    	case SB_LINEUP:
    	case SB_PAGEUP:
    		::SetTimer( mhWnd, ISCROLL_TIMERID, 300, NULL );
    		m_offset   = 0;
    		m_position = 0;
    		break;

    	case SB_THUMBPOSITION:
    		m_offset   = mi_lines[ 2 ].rc.top - y;
    		m_position = y + m_offset;
    		break;

      default:
        break;
  	}

	::SendMessage( mhWnd, WM_VSCROLL, MAKEWPARAM( ( UINT )( int )( m_action ), ( UINT )( int )( m_si.nPos ) ), ( LPARAM )( HWND )mhWnd );
	Redraw();
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iScrollView::OnLButtonUp( int x, int y )
{
	::KillTimer( mhWnd, ISCROLL_TIMERID );
	m_action = -1;
	Redraw();
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iScrollView::SetScrollInfo( SCROLLINFO *si, BOOL redraw )
{
	if ( m_si.nPos != si->nPos || m_si.nMax != si->nMax )
    {
  		m_si = *si;

  		if ( redraw )
        {
    			Redraw();
    		}
  	}
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iScrollView::GetScrollInfo( SCROLLINFO *si )
{
	*si = m_si;
}
// --------------------------------------------------------------------------
