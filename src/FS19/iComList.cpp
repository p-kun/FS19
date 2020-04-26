/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iComList.h
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
#include "iPt.h"
#include "iIcon.h"
#include "savedir.h"
#include "iHistory.h"
#include "iComList.h"


// ==========================================================================
// -- iComListクラスの構築 --
// --------------------------------------------------------------------------
iComList::iComList()
{
  iHistory  his;

  date_time = his.GetLocalDateTime();
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- iComListクラスの破棄 --
// --------------------------------------------------------------------------
iComList::~iComList()
{
}
// --------------------------------------------------------------------------


// ===========================================================================
// -- ドライブ情報追加
// ---------------------------------------------------------------------------
void iComList::SetLogData( int index, const TCHAR *pRootPath, int type )
{
  InsertString( index, pRootPath );
  SetItemData( index, type );
}
// --------------------------------------------------------------------------


// ===========================================================================
// -- オーナー描画の処理
// ---------------------------------------------------------------------------
int iComList::DrawItem( const DRAWITEMSTRUCT *pDIS )
{
  xTmpDC      dc( pDIS->hwndItem, pDIS->hDC );
  xBmpDC      bmp;
  TCHAR       buf[ MAX_PATH ];
  TCHAR       tmp[ MAX_PATH ];
  iPt         pt;
  SIZE        size;
  WPARAM      type;
  SHFILEINFO  shfi;
  HIMAGELIST  handle = 0;
  RECT        rc;
  TCHAR       date_str[ 50 ];
  iHistory    his;
  int         xxx = 0;
  int         yy1 = 0;
  int         yy2 = 0;
  int         cx;
  int         cy;
  iico        ico;

  if ( pDIS->itemID == ( UINT )(~0) )
    {
      return 0;
    }

  rc = pDIS->rcItem;

  cx  = rc.right  - rc.left;
  cy  = rc.bottom - rc.top;

  bmp.Create( dc, cx, cy );
  bmp.SetBkMode( TRANSPARENT );
  bmp.SetTextColor( 0 );
  bmp.SetSelectFont( pt.fontname2, pt.fontsize2 );

  ico.attach(bmp, IMG_MARK);

  if ( pDIS->itemState & ODS_SELECTED )
    {
      bmp.Rectangle( 0, 0, cx, cy, pt.COL_SELECTED_WAKU, pt.COL_SELECTED );
    }
  else
    {
      bmp.FillRect( 0, 0, cx, cy, pt.COL_BASE_BAR );
    }

  yy1 = ( cy - bmp.tm.tmHeight * 2 ) / 3;
  yy2 = yy1 + bmp.tm.tmHeight + yy1;

  GetText( pDIS->itemID, buf );

  type = GetItemData( pDIS->itemID );

  if ( type == 1 )
    {
      his.CmpLocalDateTime( buf, date_time, date_str, 50 );

      GetTextExtentPoint32( bmp, date_str, (int)_tcslen( date_str ), &size );

      _tcscpy_s( tmp, MAX_PATH, buf + HISTORY_PATH_OFFSET );

      handle = ( HIMAGELIST )::SHGetFileInfo( tmp,
                                              0,
                                             &shfi,
                                              sizeof( SHFILEINFO ),
                                              SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_DISPLAYNAME);
      xxx += 4;

      if (handle)
        {
          ::ImageList_DrawEx( handle,
                              shfi.iIcon,
                              bmp,
                              xxx,
                              yy1,
                              DPIX(16),
                              DPIY(16),
                              CLR_NONE,
                              CLR_NONE,
                              ILD_NORMAL );

          xxx += DPIX(24);

          bmp.TextOut( xxx, yy1, shfi.szDisplayName );
        }
      else
        {
          ico.draw(xxx, yy1, 0);

          xxx += DPIX(24);

          bmp.TextOut( xxx, yy1, PathFindFileName( tmp ) );
        }

      PathCompactPath( bmp, tmp,  cx - xxx - size.cx );

      bmp.SetTextColor( RGB( 128, 128, 128 ) );
      bmp.TextOut( xxx, yy2, tmp );

      if ( *buf == _T( '*' ) )
        {
          ico.draw(2, 2, 1);
        }

      bmp.TextOut( cx - ( size.cx + DPIX( 4 ) ), yy1, date_str );
    }
  else
    {
      handle = ( HIMAGELIST )::SHGetFileInfo( buf, 0, &shfi, sizeof( SHFILEINFO ), SHGFI_DISPLAYNAME | SHGFI_SYSICONINDEX );
      bmp.TextOut( DPIX(40), ( cy - bmp.tm.tmHeight ) / 2, shfi.szDisplayName );
      ::ImageList_DrawEx( handle, shfi.iIcon, bmp, 2, ( cy - DPIX(32) ) / 2, DPIX(32), DPIY(32), CLR_NONE, CLR_NONE, ILD_NORMAL );
    }

  dc.BitBlt( rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, bmp, 0, 0, SRCCOPY );

  return 1;
}
// ---------------------------------------------------------------------------


// ===========================================================================
// -- オーナー描画コントロールの寸法を設定
// ---------------------------------------------------------------------------
int iComList::MeasureItem( MEASUREITEMSTRUCT *pMIS )
{
  xDC   dc( mhWnd );
  iPt   pt;
  TCHAR path[ MAX_PATH ];

  dc.SetSelectFont( pt.fontname2, pt.fontsize2 );

  GetText( pMIS->itemID, path );

  if ( PathIsRoot( path ) )
    {
      if ( dc.tm.tmHeight < 32 )
        {
          pMIS->itemHeight = DPIY(38);
        }
      else
        {
          pMIS->itemHeight = dc.tm.tmHeight + DPIY(20);
        }
    }
  else
    {
      if ( dc.tm.tmHeight < DPIY(20) )
        {
          pMIS->itemHeight = DPIY(20);
        }
      else
        {
          pMIS->itemHeight = dc.tm.tmHeight + DPIY(4);
        }

      pMIS->itemHeight *= 2;
    }

  return 1;
}
// ---------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iComList::FillRect( HDC hDC, const RECT *pRC, COLORREF fg )
{
  FillRect( hDC, pRC, fg, fg );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iComList::FillRect( HDC hDC, const RECT *pRC, COLORREF pen, COLORREF fg )
{
  xTmpDC  dc( hDC );

  dc.Rectangle( pRC, pen, fg );
}
// --------------------------------------------------------------------------
