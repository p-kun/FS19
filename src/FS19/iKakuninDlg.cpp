/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iKakuninDlg.h
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
#include "resource.h"
#include "iIcon.h"
#include "iComList.h"
#include "iComEdit.h"
#include "SubFuncs.h"
#include "iPt.h"
#include "iComButton.h"
#include "logf.h"
#include "iKakuninDlg.h"

/* Pre-processor definitions */

#define COL_TITLE       0x00993300
#define OFFSET_2        2
#define OFFSET_4        4
#define OFFSET_8        8
#define OFFSET_12       12
#define OFFSET_16       16
#define ICON_SIZE       16

/* Private types and data */

static struct ITEM_ELEMENT
{
  TCHAR     name[ MAX_PATH ];
  TCHAR     path[ MAX_PATH ];
  FILETIME    time;
  LARGE_INTEGER size;
  HIMAGELIST    hImage;
  SHFILEINFO    shfi;
}
theSrc, theDes;

static struct ITEM_NAMES
{
  const TCHAR*  title;
  const TCHAR*  common;
  int           iicon;
  int           type;
}
theNames_jp[ IKAKU_MAX ] = {
  { _T( "新しい日付をコピー" ), _T( "更新日時の新しいファイルに置き換えます" ),           0, IKAKU_NEW },
  { _T( "名前を変えてコピー" ), _T( "ファイル名を変更してください" ),                     1, IKAKU_RENAME },
  { _T( "上書きコピー" ),       _T( "以下のファイルに置き換えます" ),                     2, IKAKU_OVERWRITE },
  { _T( "コピーしない" ),       _T( "同名のファイルはコピーしません" ),                   3, IKAKU_NOTCOPY },
},
theNames_en[ IKAKU_MAX ] = {
  { _T( "Copy new date" ),      _T( "Replace with new file with updated date and time" ), 0, IKAKU_NEW },
  { _T( "Rename" ),             _T( "Change the file name" ),                             1, IKAKU_RENAME },
  { _T( "Overwrite" ),          _T( "Replace with the following file" ),                  2, IKAKU_OVERWRITE },
  { _T( "Do not copy" ),        _T( "Do not copy files with the same name" ),             3, IKAKU_NOTCOPY },
},
*theNames = theNames_en;

TCHAR   title01_jp[] = _T("この場所には同じ名前のファイルが既にあります。");
TCHAR   title02_jp[] = _T("コピー方法を以下から選んでください。");
TCHAR   title01_en[] = _T("There is a file with the same name in this location.");
TCHAR   title02_en[] = _T("Select a copy method from the following.");
TCHAR*  title01 = title01_en;
TCHAR*  title02 = title02_en;

TCHAR   Explanation_jp[] = _T("サイズ: %,U byte  更新日時: %02d-%02d-%02d %2d:%02d:%02d");
TCHAR   Explanation_en[] = _T("Size: %,U byte  Update: %02d-%02d-%02d %2d:%02d:%02d");
TCHAR*  Explanation = Explanation_en;

TCHAR   title_jp[] = _T("ファイルのコピー");
TCHAR   title_en[] = _T("Copy file");
TCHAR*  title = title_en;

TCHAR   all_after_that_jp[] = _T("以降全てOK(&A)");
TCHAR   all_after_that_en[] = _T("All after that OK(&A)");
TCHAR*  all_after_that = all_after_that_en;

TCHAR   cancel_str_jp[] = _T("ｷｬﾝｾﾙ");
TCHAR   cancel_str_en[] = _T("Cancel");
TCHAR*  cancel_str = cancel_str_en;

// ==========================================================================
// -- 
// --------------------------------------------------------------------------
class iComEditEx : public xEdit
{
public:
  iComEditEx()
  {
    iPt   pt;

    isCreate = FALSE;
    font.Create( pt.fontname1, pt.fontsize1 );

    mStyle = ES_AUTOHSCROLL | WS_CHILD;
    mExStyle = WS_EX_STATICEDGE;
  }

  void OnKeyDown( UINT vk, BOOL fDown, int cRepeat, UINT flags )
  {
    switch( vk )
      {
        case VK_PRIOR:
        case VK_NEXT:
        case VK_UP:
        case VK_DOWN:
        case VK_TAB:
          GetParent()->SendMessage( WM_COMMAND, GET_WM_COMMAND_MPS( ::GetDlgCtrlID( mhWnd ), mhWnd, vk ) );
        break;
        
        default:
          xEdit::OnKeyDown( vk, fDown, cRepeat, flags );
        break;
      }
  }

  HWND Create( HWND hWnd, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT )
  {
    if ( isCreate )
      {
        return mhWnd;
      }

    isCreate = TRUE;

    xDC   dc( hWnd );
    iPt   pt;

    dc.SetSelectFont( pt.fontname1, pt.fontsize1 );

    hWnd = xEdit::Create( hWnd, x, y, cx, dc.tm.tmHeight + 2 );
    SetFont( font );
    return hWnd;
  }

  BOOL Destroy()
  {
    if ( isCreate )
      {
        isCreate = FALSE;
        return xEdit::Destroy();
      }

    return FALSE;
  }

  void OnKillFocus( HWND hwndNewFocus )
  {
    Destroy();
  }

  BOOL IsCreate()
  {
    return isCreate;
  }

private:
  BOOL  isCreate;
  xFont font;
};
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
class iComListEx : public iComList
{
public:
  iComListEx();
  ~iComListEx();

public:
  int   GetItemHeight1( HDC hDC );
  int   GetItemHeight2( HDC hDC );

private:
  int   DrawItem( const DRAWITEMSTRUCT *lpDrawItem );
  int   MeasureItem( MEASUREITEMSTRUCT *lpMeasureItem );
  void  DrawItem( HDC hDC, RECT *rc, ITEM_ELEMENT *item, int index, int mode = 0 );
  void  OnCommand( int id, HWND hwndCtl, UINT codeNotify );

private:
  iComEditEx* mp_rename;
  int         mEdit_x;
  int         mEdit_y;

public: /* 特別 */
  TCHAR   m_newpath[ MAX_PATH ];
  
};
// --------------------------------------------------------------------------


// ===========================================================================
// --
// ---------------------------------------------------------------------------
iComListEx::iComListEx()
{
  mEdit_x = 0;
  mEdit_y = 0;
  mp_rename = new iComEditEx;
}
// --------------------------------------------------------------------------
iComListEx::~iComListEx()
{
  delete mp_rename;
}
// --------------------------------------------------------------------------


// ===========================================================================
// -- オーナー描画の処理
// ---------------------------------------------------------------------------
int iComListEx::DrawItem( const DRAWITEMSTRUCT *pDIS )
{
  xTmpDC  dc( pDIS->hwndItem, pDIS->hDC );
  xBmpDC  bmp;
  TCHAR   buf[ MAX_PATH ];
  iPt     pt;
  RECT    rc;
  int     index;
  UINT    itemState;

  pDIS->rcItem;

  rc.left   = 0;
  rc.top    = 0;
  rc.right  = pDIS->rcItem.right - pDIS->rcItem.left;
  rc.bottom = pDIS->rcItem.bottom - pDIS->rcItem.top;

  bmp.Create( dc, rc.right, rc.bottom );
  bmp.SetBkMode( TRANSPARENT );
  bmp.SetTextColor( 0 );
  bmp.SetSelectFont( pt.fontname1, pt.fontsize1 );

  if ( pDIS->itemID == ( UINT )(~0) )
    {
      return 1;
    }

  bmp.FillRect( &rc, pt.COL_BASE_BAR );

  GetText( pDIS->itemID, buf );

  index = ( int )GetItemData( pDIS->itemID );

  if ( pDIS->itemState & ODS_SELECTED )
    {
      FillRect( bmp, &rc, pt.COL_SELECTED_WAKU, pt.COL_SELECTED );
    }

  switch ( index )
    {
      case IKAKU_NEW:
        if ( CompareFileTime( &theSrc.time, &theDes.time ) > 0 )
          {
            DrawItem( bmp, &rc, &theSrc, index );
          }
        else
          {
            DrawItem( bmp, &rc, &theDes, index );
          }
        break;

      case IKAKU_NOTCOPY:
        DrawItem( bmp, &rc, NULL, index );
        break;

      case IKAKU_RENAME:
        itemState = pDIS->itemState & ( ODS_SELECTED | ODS_FOCUS );
        DrawItem( bmp, &rc, &theDes, index, ( itemState == ( ODS_SELECTED | ODS_FOCUS ) ) );

        if ( itemState == ( ODS_SELECTED | ODS_FOCUS ) )
          {
            mp_rename->Create( pDIS->hwndItem, pDIS->rcItem.left + mEdit_x, pDIS->rcItem.top +mEdit_y, 
              rc.right - ( pDIS->rcItem.left + mEdit_x + ICON_SIZE/* + OFFSET_4 * 2*/) );
            mp_rename->ShowWindow();
            mp_rename->SetFocus();
            mp_rename->SetWindowText( theDes.name );
            mp_rename->SetSel( 0, ( int )( PathFindExtension( theDes.name ) - PathFindFileName( theDes.name ) ) );
          }
        else
          {
            if ( itemState != ODS_SELECTED )
              {
                mp_rename->Destroy();
              }
          }
        break;

      case IKAKU_OVERWRITE:
        DrawItem( bmp, &rc, &theSrc, index );
        break;

      default:
        DrawItem( bmp, &rc, NULL, index );
        break;
    }

  dc.BitBlt( pDIS->rcItem.left, pDIS->rcItem.top, rc.right, rc.bottom, bmp, 0, 0, SRCCOPY );

  return 1;
}
// ---------------------------------------------------------------------------


// ===========================================================================
// --
// ---------------------------------------------------------------------------
int iComListEx::GetItemHeight1( HDC hDC )
{
  xTmpDC  dc( hDC );
  int     height = dc.tm.tmHeight + OFFSET_2;

  return OFFSET_4 + height + height + OFFSET_8 + height + height + height + OFFSET_16;
}
// ---------------------------------------------------------------------------


// ===========================================================================
// --
// ---------------------------------------------------------------------------
int iComListEx::GetItemHeight2( HDC hDC )
{
  xTmpDC  dc( hDC );
  int     height = dc.tm.tmHeight + OFFSET_2;

  return OFFSET_4 + height + height + OFFSET_16;
}
// ---------------------------------------------------------------------------


// ===========================================================================
// -- オーナー描画コントロールの寸法を設定
// ---------------------------------------------------------------------------
int iComListEx::MeasureItem( MEASUREITEMSTRUCT *pMIS )
{
  xDC   dc( mhWnd );
  iPt   pt;
  TCHAR buf[ MAX_PATH ];

  dc.SetSelectFont( pt.fontname1, pt.fontsize1 );

  GetText( pMIS->itemID, buf );

  switch ( buf[ 0 ] )
    {
      case '0':
      case '1':
      case '2':
        pMIS->itemHeight = GetItemHeight1( dc );
        break;
        
      case '3':
        pMIS->itemHeight = GetItemHeight2( dc );
        break;
        
      default:
        break;
    }

  return 1;
}
// ---------------------------------------------------------------------------


// ===========================================================================
// -- コマンドの処理
// ---------------------------------------------------------------------------
void iComListEx::OnCommand( int id, HWND hwndCtl, UINT codeNotify )
{
  int   index = GetCurSel();

  if ( codeNotify == VK_DOWN )
    {
      SetCurSel( index + 1 );
    }
  else if ( codeNotify == VK_UP )
    {
      if ( index > 0 )
        {
          SetCurSel( index - 1 );
        }
    }
  else if ( codeNotify == /*EN_CHANGE*/EN_UPDATE )
    {
      mp_rename->GetWindowText( m_newpath, MAX_PATH );
    }
}
// ---------------------------------------------------------------------------


// ==========================================================================
// -- iKakuninDlgクラスの構築 --
// --------------------------------------------------------------------------
iKakuninDlg::iKakuninDlg( HWND hParent ) : xDialog( _T( "IDD_KAKUNINN" ), hParent )
{
  mpLogList = new iComListEx;
  mpButton1 = new xButton;
  mpButton2 = new xButton;
  mpButton3 = new xButton;
  selected  = 0;
  thatsall  = FALSE;
  mFraem_cy = GetSystemMetrics( SM_CYSIZEFRAME ) +
              GetSystemMetrics( SM_CYCAPTION )   +
              GetSystemMetrics( SM_CYSIZEFRAME );

}
// --------------------------------------------------------------------------


// ==========================================================================
// -- iKakuninDlgクラスの破棄 --
// --------------------------------------------------------------------------
iKakuninDlg::~iKakuninDlg()
{
  delete mpLogList;
  delete mpButton1;
  delete mpButton2;
  delete mpButton3;
}
// --------------------------------------------------------------------------


// ===========================================================================
// -- コマンドの処理
// ---------------------------------------------------------------------------
void iKakuninDlg::OnCommand( int id, HWND hwndCtl, UINT codeNotify )
{
  TCHAR   cp[ MAX_PATH ];

  switch ( id )
    {
      case IDC_LIST1:
        if ( codeNotify != LBN_DBLCLK )
          {
            break;
          }

      case IDC_BUTTON1:
        thatsall = TRUE;
      case IDOK:
        selected = ( int )mpLogList->GetItemData( mpLogList->GetCurSel() );

        if ( selected == IKAKU_RENAME )
          {
            _tcscpy_s( cp, MAX_PATH, theDes.path );

            PathAppend( cp, mpLogList->m_newpath );

            if ( PathFileExists( cp ) )
              {
                break;
              }
            _tcscpy_s( mNewPath, MAX_PATH, mpLogList->m_newpath );
          }

        EndDialog( IDOK );
        break;

      case IDCANCEL:
        EndDialog( IDCANCEL );
        break;

      default:
        break;
    }
}
// ---------------------------------------------------------------------------


// ===========================================================================
// -- ダイアログの初期化
// ---------------------------------------------------------------------------
BOOL iKakuninDlg::OnInitDialog( HWND hwndFocus, LPARAM lParam )
{
  RECT  rc;
  xDC   dc( mhWnd );
  iPt   pt;
  int   index = 0;
  int   height;
  SIZE  size;
  RECT  ctrl;
  int   btnw;
  int   btnh;
  int   titl;

  dc.SetSelectFont( pt.fontname1, pt.fontsize1 );

  dc.GetTextExtentPoint32( cancel_str, &size );

  size.cx *= 2;

  btnw = OFFSET_8 + size.cx + OFFSET_8;
  btnh = OFFSET_8 + size.cy + OFFSET_8;

  titl = OFFSET_8 + ( size.cy + OFFSET_2 ) * 2  + OFFSET_16;

  Attach( *mpLogList, IDC_LIST1 );
  Attach( *mpButton1, IDOK );
  Attach( *mpButton2, IDCANCEL );
  Attach( *mpButton3, IDC_BUTTON1 );

  thatsall = FALSE;

  if ( _tcsicmp( theSrc.path, theDes.path ) == 0 )
    {
      mpLogList->SetLogData( 0, _T( "1" ), IKAKU_RENAME );
      mpLogList->SetLogData( 1, _T( "3" ), IKAKU_NOTCOPY );

      if ( selected == IKAKU_RENAME )
        {
          index = 0;
        }
      else if ( selected == IKAKU_NOTCOPY )
        {
          index = 1;
        }
      height = mpLogList->GetItemHeight1( dc ) + mpLogList->GetItemHeight2( dc );
    }
  else
    {
      mpLogList->SetLogData( 0, _T( "0" ), IKAKU_NEW );
      mpLogList->SetLogData( 1, _T( "1" ), IKAKU_RENAME );
      mpLogList->SetLogData( 2, _T( "2" ), IKAKU_OVERWRITE );
      mpLogList->SetLogData( 3, _T( "3" ), IKAKU_NOTCOPY );
      index = selected;
      height = mpLogList->GetItemHeight1( dc ) * 3 + mpLogList->GetItemHeight2( dc );
    }

  mpLogList->SetCurSel( index );
  mpLogList->SetFocus();

  mHeight = titl + height;

  MoveWindow( 0, 0, 20 * dc.tm.tmMaxCharWidth, mFraem_cy + mHeight + OFFSET_16 + btnh + OFFSET_16 );
  GetClientRect( &rc );

  mpLogList->MoveWindow( OFFSET_8, titl, rc.right - OFFSET_8 - OFFSET_8, height );

  SetWindowText(title);

  mFont.Create( pt.fontname1, pt.fontsize1 );

  /* Set font at button controls */

  mpButton1->SetFont( mFont );
  mpButton2->SetFont( mFont );
  mpButton3->SetFont( mFont );

  ctrl.left   = rc.right - OFFSET_8 - btnw - OFFSET_8 - btnw;
  ctrl.top    = mHeight + OFFSET_12;
  ctrl.right  = ctrl.left + btnw;
  ctrl.bottom = ctrl.top  + btnh;

  mpButton1->MoveWindow( &ctrl );

  ctrl.left   = rc.right - OFFSET_8 - btnw;
  ctrl.top    = mHeight + OFFSET_12;
  ctrl.right  = ctrl.left + btnw;
  ctrl.bottom = ctrl.top  + btnh;

  mpButton2->MoveWindow( &ctrl );

  mpButton2->SetWindowText(cancel_str);

  dc.GetTextExtentPoint32(all_after_that, &size);

  btnw = OFFSET_8 + size.cx + OFFSET_8;

  ctrl.left   = OFFSET_8;
  ctrl.top    = mHeight + OFFSET_12;
  ctrl.right  = ctrl.left + btnw;
  ctrl.bottom = ctrl.top  + btnh;

  mpButton3->MoveWindow( &ctrl );

  mpButton3->SetWindowText(all_after_that);

  mpLogList->GetClientRect( &rc );

  /* 真ん中に移動 */

  CenterWindow();

  return 0;
}
// ---------------------------------------------------------------------------


// ===========================================================================
// -- WM_PAINT
// ---------------------------------------------------------------------------
void iKakuninDlg::OnPaint( void )
{
  xPaintDC  dc( mhWnd );
  RECT      rc;
  int       height;
  iPt       pt;
  int       x;
  int       y;

  GetClientRect( &rc );

  dc.SetSelectFont( pt.fontname1, pt.fontsize1 );
  dc.SetBkMode( TRANSPARENT );
  rc.bottom = mHeight;
  dc.FillRect( &rc, pt.COL_BASE_BAR );

  x = rc.left + 8;
  y = rc.top  + 8;

  height = dc.tm.tmHeight + OFFSET_2;

  dc.SetTextColor( COL_TITLE );
  dc.TextOut( x, y, title01 );
  dc.SetTextColor( 0 );
  y += height;
  dc.TextOut( x, y, title02 );
}
// ---------------------------------------------------------------------------


// ===========================================================================
// -- 
// ---------------------------------------------------------------------------
void iKakuninDlg::SetSourceFileInfo( const TCHAR *src, FILETIME *time, DWORD high, DWORD low )
{
  LARGE_INTEGER size;

  size.HighPart = high;
  size.LowPart  = low;

  SetSourceFileInfo( src, time, &size );
}
// ---------------------------------------------------------------------------


// ===========================================================================
// -- 
// ---------------------------------------------------------------------------
void iKakuninDlg::SetSourceFileInfo( const TCHAR *src, FILETIME *time, LARGE_INTEGER *size )
{
  _tcscpy_s( theSrc.name, MAX_PATH, PathFindFileName( src ) );
  _tcscpy_s( theSrc.path, MAX_PATH, src );

  PathRemoveFileSpec( theSrc.path );

  theSrc.time = *time;
  theSrc.size = *size;

  theSrc.hImage = ( HIMAGELIST )::SHGetFileInfo( src,
                                                 FILE_ATTRIBUTE_ARCHIVE,
                                                &theSrc.shfi,
                                                 sizeof( SHFILEINFO ),
                                                 SHGFI_TYPENAME | SHGFI_SYSICONINDEX );
}
// ---------------------------------------------------------------------------


// ===========================================================================
// -- 
// ---------------------------------------------------------------------------
void iKakuninDlg::SetTransFileInfo( const TCHAR *trans, FILETIME *time, LARGE_INTEGER *size )
{
  _tcscpy_s( theDes.name, MAX_PATH, PathFindFileName( trans ) );
  _tcscpy_s( theDes.path, MAX_PATH, trans );

  PathRemoveFileSpec( theDes.path );

  theDes.time = *time;
  theDes.size = *size;

  theDes.hImage = ( HIMAGELIST )::SHGetFileInfo( trans,
                                                 FILE_ATTRIBUTE_ARCHIVE,
                                                &theDes.shfi,
                                                 sizeof( SHFILEINFO ),
                                                 SHGFI_TYPENAME | SHGFI_SYSICONINDEX );
}
// ---------------------------------------------------------------------------


// ===========================================================================
// -- 
// ---------------------------------------------------------------------------
void iKakuninDlg::SetFiles( const TCHAR *src, const TCHAR *trans )
{
  _tcscpy_s( theSrc.path, MAX_PATH, src );
  _tcscpy_s( theDes.path, MAX_PATH, trans );
}
// ---------------------------------------------------------------------------


// ===========================================================================
// -- 
// ---------------------------------------------------------------------------
void iComListEx::DrawItem( HDC hDC, RECT *rc, ITEM_ELEMENT *i, int index, int mode )
{
  xTmpDC      dc( hDC );
  int         x;
  int         y;
  int         height;
  TCHAR*      cp;
  TCHAR       top[ MAX_PATH ];
  SYSTEMTIME  st;
  FILETIME    local;
  iPt         pt;
  int         len;
  iico        ico(hDC, IMG_COPYDLG);

  x = rc->left + OFFSET_4;
  y = rc->top + OFFSET_4;

  height = dc.tm.tmHeight + OFFSET_2;

  if ( dc.tm.tmHeight > ICON_SIZE )
    {
      ico.draw(x, y + ( dc.tm.tmHeight - ICON_SIZE ) / 2, theNames[ index ].iicon);
      dc.TextOut( x + ICON_SIZE + OFFSET_4, y, theNames[ index ].title );
    }
  else
    {
      ico.draw(x, y, theNames[ index ].iicon);
      dc.TextOut( x + ICON_SIZE + OFFSET_4, y + ( ICON_SIZE - dc.tm.tmHeight ) / 2, theNames[ index ].title );
    }

  y += height;
  dc.SetTextColor( COL_TITLE );
  dc.TextOut( x + ICON_SIZE + OFFSET_2, y, theNames[ index ].common );
  y += ( height + OFFSET_8 );

  if ( !i )
    {
      return;
    }

  FileTimeToLocalFileTime( &i->time, &local );
  FileTimeToSystemTime( &local, &st );
  st.wYear -= 2000;

  cp = top;

  x += OFFSET_16;

  ::ImageList_DrawEx( ( HIMAGELIST )i->hImage, i->shfi.iIcon, hDC, x, y, DPIX(32), DPIY(32), CLR_NONE, CLR_NONE, ILD_NORMAL );

  x += DPIX(40);

  mEdit_x = x;
  mEdit_y = y;

  if ( mode == 0 )
    {
      dc.SetSelectFont( pt.fontname1, pt.fontsize1, FW_DEMIBOLD );
      dc.SetTextColor( RGB( 0, 0, 0 ) );

      if ( dc.tm.tmAveCharWidth )
        {
          len = ( rc->right - ( mEdit_x + ICON_SIZE + OFFSET_4 * 2 ) ) / dc.tm.tmAveCharWidth;
        }
      else
        {
          len = 14;
        }
      strncpy_sc( cp, MAX_PATH, i->name, len, 1 );
      dc.TextOut( x, y, cp );
    }
  y += dc.tm.tmHeight + OFFSET_4;

  dc.SetSelectFont( pt.fontname1, pt.fontsize1 );

  if ( dc.tm.tmAveCharWidth )
    {
      len = ( rc->right - ( mEdit_x + ICON_SIZE + OFFSET_4 * 2 ) ) / dc.tm.tmAveCharWidth;
    }
  else
    {
      len = 16;
    }

  *cp++ = '(';
  PathCompactPathEx( cp, i->path, len, _T( '\\' ) );
  while( *cp ) cp++;
  *cp++ = ')';
  *cp++ = '\0';
  cp = top;

  dc.SetTextColor( COL_TITLE );
  dc.TextOut( x, y, cp );
  y += dc.tm.tmHeight + OFFSET_4;

  ssnprintf( cp,
             MAX_PATH,
             Explanation,
             i->size.QuadPart,
             st.wYear,
             st.wMonth,
             st.wDay,
             st.wHour,
             st.wMinute,
             st.wSecond );

  dc.SetTextColor( 0 );
  dc.TextOut( x, y, cp );
}
// ---------------------------------------------------------------------------
