/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iComButton.cpp
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
#include "iPt.h"
#include "xDcBase.h"
#include "iComButton.h"


// ==========================================================================
// -- 上下キーのみ親にメッセージを送るよ
// --------------------------------------------------------------------------
void iComButton::OnKeyDown( UINT vk, BOOL fDown, int cRepeat, UINT flags )
{
	switch ( vk )
    {
    	case VK_PRIOR:
    	case VK_NEXT:
    	case VK_UP:
    	case VK_DOWN:
    		GetParent()->SendMessage( WM_COMMAND, GET_WM_COMMAND_MPS( ::GetDlgCtrlID( mhWnd ), mhWnd, iComButton_KEY | ( vk & 0xFF ) ) );
    	  break;

    	default:
    		GetParent()->SendMessage( WM_COMMAND, GET_WM_COMMAND_MPS( ::GetDlgCtrlID( mhWnd ), mhWnd, iComButton_KEY | ( vk & 0xFF ) ) );
      	break;
  	}
}
// --------------------------------------------------------------------------


// ===========================================================================
// -- オーナー描画の処理
// ---------------------------------------------------------------------------
int iComButton::DrawItem( const DRAWITEMSTRUCT *pDIS )
{
	xTmpDC		dc( pDIS->hwndItem, pDIS->hDC );
	TCHAR		  buf[ MAX_PATH ];
	iPt			  pt;
	RECT		  rc;
	int			  height;
	int			  y;

	dc.SetBkMode( TRANSPARENT );
	dc.SetTextColor( 0xFF000000 );
	dc.SetSelectFont( pt.fontname1, pt.fontsize1 );

	if ( pDIS->itemID == ( UINT )(~0) )
    {
      return 0;
    }

  rc = pDIS->rcItem;

	if ( pDIS->itemState & ODS_SELECTED )
	  {
			FillRect( dc, &rc, RGB( 0, 0, 255 ) );
			y = 1;
		}

  if ( pDIS->itemState & ODS_FOCUS )
	  {
			FillRect( dc, &rc, RGB( 0, 0, 192 ) );
			y = 1;
		}
	else
	  {
			FillRect( dc, &rc, RGB( 128, 128, 128 ) );
			y = 0;
		}

	GetText( buf, MAX_PATH );

	height = rc.bottom - rc.top;

	rc.top += y;
	rc.bottom += y;

	dc.DrawText( buf, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

  return 1;
}
// ---------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static COLORREF Brend( COLORREF col, int alph )
{
	int		r = GetRValue( col );
	int		g = GetGValue( col );
	int		b = GetBValue( col );

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
void iComButton::FillRect( HDC hDC, const RECT *pRC, COLORREF fg )
{
	int		  i;
	int		  h;
	int		  alph;
	xTmpDC	dc( hDC );

	h = pRC->bottom - pRC->top;

	dc.RoundRect( pRC, 4, 4, fg, fg );

	for ( i = 0; i < h - 2; i++ )
    {

  		alph = i * 16 / ( h - 1 ) + 9;

  		dc.Line( pRC->left + 1, i + pRC->top + 1, pRC->right - 1, i + pRC->top + 1, Brend( fg, alph ) );
  	}
}
// --------------------------------------------------------------------------
