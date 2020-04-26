/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iSubApp.cpp
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
#include "CopyFuncs.h"
#include "iNamelist.h"
#include "iBaseView.h"
#include "iScrollView.h"
#include "iSeachView.h"
#include "iDirectoryView.h"
#include "iInfoView.h"
#include "savedir.h"
#include "ipt.h"
#include "iTreeView.h"
#include "iStatusView.h"
#include "logf.h"
#include "iResultDlg.h"
#include "timerid.h"
#include "iComButton.h"
#include "iSubApp.h"

/* 定義値 */

#define BUTTON_SIZE             DPIX(18)
#define VIEW_SIZE               160
#define DRAWR( hWnd, hDC, rc )  Draw( hWnd, hDC, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top )
#define ISUBAPP_TIMER           1000
#define ISUBAPP_SSSS_TIMER      1


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iSubApp::iSubApp()
{
  iPt   pt;

  mp_tree_view   = new iTreeView;
  mp_status_view = new iStATUsViEW;
  mp_scroll_view = new iScrollView;
  mp_seach_view1 = new iSeachView;
  mp_seach_view2 = new iSeachView;
  mp_info_view   = new iInfoView;
  mp_dir_view1   = new iDirectoryView;
  mp_dir_view2   = new iDirectoryView;

  if ( pt.page_no == 0 )
    {
      mp_act_seach  = mp_seach_view1;
    }
  else
    {
      mp_act_seach  = mp_seach_view2;
    }

  mp_act_seach1 = mp_seach_view1;
  mp_act_seach2 = mp_seach_view2;
  mp_act_dir    = mp_dir_view1;
  mp_act_dir1   = mp_dir_view1;
  mp_act_dir2   = mp_dir_view2;

  mp_act_dir2->SetTop( FALSE );

  m_lock  = FALSE;
  m_focus = 1;

  m_tree_mode = FALSE;

  m_cursor_wait_icon = LoadCursor( NULL, IDC_SIZEWE );

  m_splt_on = FALSE;

  m_tool_size = pt.tool_size;

  m_cx = 0;

  /* 2015/8/27追加 */

  mpCheckBoxs = NULL;

  mStyle   = WS_VISIBLE | WS_CAPTION | WS_POPUP | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME;

  mTitleBuf[ 0 ] = L'\0';

  mIsERASEBKGND = TRUE;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iSubApp::~iSubApp()
{
  delete mp_tree_view;
  delete mp_status_view;
  delete mp_scroll_view;
  delete mp_seach_view1;
  delete mp_seach_view2;
  delete mp_info_view;
  delete mp_dir_view1;
  delete mp_dir_view2;

  if ( mpCheckBoxs )
    {
      delete mpCheckBoxs;
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void PaintCustomCaption(HWND hWnd, HDC hdc)
{
  /* Paint the title on the custom frame. */

  RECT        rcClient;
  RECT        rcPaint;
  HTHEME      hTheme = OpenThemeData( NULL, L"CompositedWindow::Window" );
  HDC         hdcPaint;
  int         cx;
  int         cy;
  BITMAPINFO  dib = { 0 };
  HBITMAP     hbm;
  HBITMAP     hbmOld;
  LOGFONT     lgFont;
  HFONT       hFontOld = NULL;
  HFONT       hFont;

  GetClientRect( hWnd, &rcClient );

  if ( !hTheme )
    {
      goto _err_exit;
    }

  hdcPaint = CreateCompatibleDC( hdc );

  if ( !hdcPaint )
    {
      goto _err_exit_OpenThemeData;
    }

  cx = rcClient.right  - rcClient.left;
  cy = rcClient.bottom - rcClient.top;

  /* Define the BITMAPINFO structure used to draw text.
   * Note that biHeight is negative. This is done because
   * DrawThemeTextEx() needs the bitmap to be in top-to-bottom
   * order.
   */

  dib.bmiHeader.biSize        = sizeof( BITMAPINFOHEADER );
  dib.bmiHeader.biWidth       = cx;
  dib.bmiHeader.biHeight      = -cy;
  dib.bmiHeader.biPlanes      = 1;
  dib.bmiHeader.biBitCount    = 1;
  dib.bmiHeader.biCompression = BI_RGB;

  hbm = CreateDIBSection( hdc, &dib, DIB_RGB_COLORS, NULL, NULL, 0 );

  if ( !hbm )
    {
      goto _err_exit_CreateCompatibleDC;
    }

  hbmOld = ( HBITMAP )SelectObject( hdcPaint, hbm );

  /* Select a font. */

  if ( SUCCEEDED( GetThemeSysFont( hTheme, /*TMT_CAPTIONFONT*/0, &lgFont ) ) )
    {
      hFont    = CreateFontIndirect( &lgFont );
      hFontOld = ( HFONT )SelectObject( hdcPaint, hFont );
    }

  /* Draw the title. */

  rcPaint        = rcClient;
  rcPaint.top   += 8;
  rcPaint.right -= 125;
  rcPaint.left  += 8;
  rcPaint.bottom = 50;

  DrawThemeText( hTheme, hdcPaint, 0, 0, L"szTitle", -1, DT_LEFT | DT_WORD_ELLIPSIS, 0, &rcPaint );

  /* Blit text to the frame. */

  BitBlt( hdc, 0, 0, cx, cy, hdcPaint, 0, 0, SRCCOPY );

  SelectObject( hdcPaint, hbmOld );

  if ( hFontOld )
    {
      SelectObject( hdcPaint, hFontOld );
    }

  DeleteObject( hbm );

  /* End processing */

_err_exit_CreateCompatibleDC:

  DeleteDC( hdcPaint );

_err_exit_OpenThemeData:

  CloseThemeData( hTheme );

_err_exit:

  return;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSubApp::OnDrawItem( const DRAWITEMSTRUCT * lpDrawItem )
{
  mpCheckBoxs->DrawItem( lpDrawItem );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
LRESULT iSubApp::Message( UINT msg, WPARAM wParam, LPARAM lParam )
{
  int             x;
  int             y;
  LRESULT         res;
  TRACKMOUSEEVENT tme;
  HDC             hDC;
  PAINTSTRUCT     ps;
  BOOL            next  = TRUE;
  static int      first = 1;
  RECT            rc;

  x = ( int )( short )LOWORD( lParam );
  y = ( int )( short )HIWORD( lParam );

  switch( msg )
    {
      case WM_KEYDOWN:
        next = LetsKeyDown( wParam, lParam );
        break;

      case WM_KEYUP:
        mp_info_view->TargetSet( mp_act_seach->GetCurrentEntry() );
        break;

      case WM_PAINT:
        hDC = ::BeginPaint( mhWnd, &ps );

        next = Draw( mhWnd, hDC );

        ::EndPaint( mhWnd, &ps );

        if ( first )
          {
            first = 0;
            ::SetTimer( mhWnd, ISUBAPP_SSSS, ISUBAPP_SSSS_TIMER, NULL );    // タイマー設定
          }

      UpdateScale();
      break;

      case WM_CREATE:
        GetClientRect( &rc );

        mp_scroll_view->Create( mhWnd );
        mp_info_view->Create( mhWnd );
        mp_seach_view1->Create( mhWnd );
        mp_seach_view2->Create( mhWnd );
        mp_dir_view1->Create( mhWnd );
        mp_dir_view2->Create( mhWnd );
        mp_act_seach2->SetPositon( FALSE );
        mp_act_dir1->SetEnable( TRUE );
        mp_act_dir2->SetEnable( FALSE );
        break;

      case WM_TIMER:
        next = LetsTimer( ( UINT )wParam );
        break;

      case WM_LBUTTONUP:
        ::ReleaseCapture();
        mp_scroll_view->OnLButtonUp( x, y );
        mp_act_dir->OnLButtonUp( x, y );
        mp_act_seach->OnLButtonUp( x, y );
        mp_info_view->OnLButtonUp( x, y );
        mp_info_view->TargetSet( mp_act_seach->GetCurrentEntry() );
        SetStatusView();
        m_splt_on = FALSE;
        break;
        
      case WM_MOUSEMOVE:
        if ( m_splt_on )
          {
            GetClientRect( &rc );

            if ( m_tool_size != x && x >= TOOLBER_SIZE && x <= ( rc.right - rc.left ) - TOOLBER_SIZE )
              {
                m_tool_size = x;
                Invalidate();
              }

            break;
          }

        tme.cbSize      = sizeof( tme );
        tme.dwFlags     = TME_LEAVE | TME_HOVER;
        tme.hwndTrack   = mhWnd;
        tme.dwHoverTime = 3000;

        TrackMouseEvent( &tme );

        if ( thePtInRect( x, y ) == IRC_SPLIT_LU )
          {
            SetCursor( m_cursor_wait_icon );
            break;
          }
        else if ( thePtInRect( x, y ) == IRC_SHORTCUT2 )
          {
            mp_tree_view->OnMouseMove( x, y );
          }
        else
          {
            mp_tree_view->OnMouseLeave();
          }

        mp_scroll_view->OnMouseMove( x, y );
        mp_act_seach->OnMouseMove( x, y );
        mp_act_dir->OnMouseMove( x, y );
        mp_info_view->OnMouseMove( x, y );
        break;

      case WM_LBUTTONDOWN:
        //
        ::SetCapture( mhWnd );
        next = LetsLButtonDown( x, y );
        break;


      case WM_RBUTTONDOWN:
        //
        next = LetsRButtonDown( x, y );
        break;
        
      case WM_RBUTTONUP:
        //
        mp_tree_view->OnRButtonUp( x, y );
        mp_act_dir->OnRButtonUp( x, y );
        mp_act_seach->OnRButtonUp( x, y );
        mp_info_view->TargetSet( mp_act_seach->GetCurrentEntry() );
        SetStatusView();
        break;
        
      case WM_LBUTTONDBLCLK:
        //
        if ( m_tree_mode )
          {
            mp_tree_view->OnLButtonDblClk( x, y - m_rcs[ IRC_SHORTCUT2 ].top, 0 );
            SetScrollPos();
            Invalidate();
          }
        else
          {
            mp_act_seach->OnLButtonDblClk( x, y, 0 );
            mp_act_dir1->SetStepBackAndUpFocus( mp_act_seach->m_step_que.top() != NULL,
                                                mp_act_seach->m_back_que.size() > 1,
                                               !mp_act_seach->PathIsRoot() );
            mp_act_dir->SetDirectory1( mp_act_seach->GetCurrentDirectory(), mp_act_seach->GetDirCount() );
            mp_tree_view->SetCurrentPath( mp_act_seach->GetCurrentDirectory() );
            mp_tree_view->SetMarkPath( 0, mp_act_seach1->GetCurrentDirectory() );
            mp_tree_view->SetMarkPath( 1, mp_act_seach2->GetCurrentDirectory() );
            mp_tree_view->Redraw( mhWnd, &m_rcs[ IRC_SHORTCUT2 ] );
            SetTitle();
            SetScrollPos();
            SetStatusView();
          }

        break;

      case WM_MOUSELEAVE:
        mp_tree_view->OnMouseLeave();
        mp_scroll_view->OnMouseLeave();
        mp_act_seach->OnMouseLeave();
        mp_act_dir->OnMouseLeave();
        mp_info_view->OnMouseLeave();
        break;

      case WM_COMMAND:
        next = LetsCommand( LOWORD( wParam ), HIWORD( wParam ), lParam );
        break;
        
      case WM_SETFOCUS:
        mp_scroll_view->SetFocus( TRUE );
        mp_info_view->SetFocus( TRUE );

        if ( m_tree_mode )
          {
            mp_tree_view->SetFocusLV2( TRUE );
            mp_seach_view1->SetFocus( FALSE );
            mp_seach_view2->SetFocus( FALSE );
          }
        else
          {
            mp_tree_view->SetFocusLV2( TRUE );
            mp_tree_view->SetFocus( FALSE );
            mp_seach_view1->SetFocus( TRUE );
            mp_seach_view2->SetFocus( TRUE );
          }

        mp_dir_view1->SetFocus( TRUE );
        mp_dir_view2->SetFocus( TRUE );
        mp_status_view->SetFocus( TRUE );

        Invalidate();
        break;

      case WM_KILLFOCUS:
        mp_scroll_view->SetFocus( FALSE );
        mp_info_view->SetFocus( FALSE );
        mp_tree_view->SetFocusLV2( FALSE );
        mp_seach_view1->SetFocus( FALSE );
        mp_seach_view2->SetFocus( FALSE );
        mp_dir_view1->SetFocus( FALSE );
        mp_dir_view2->SetFocus( FALSE );
        mp_status_view->SetFocus( FALSE );

        Invalidate();
        break;

      case WM_MOUSEHOVER:
        mp_info_view->OnMouseHover();
        mp_dir_view1->OnMouseHover();
        mp_dir_view2->OnMouseHover();
        break;

      case WM_DESTROYCLIPBOARD:
      case WM_DRAWCLIPBOARD:
        mp_info_view->TargetSet( mp_act_seach->GetCurrentEntry() );
        break;

      case WM_PAINTCLIPBOARD:
        break;
        
      case WM_VSCROLLCLIPBOARD:
        break;
        
      case WM_SIZECLIPBOARD:
        break;

      case WM_SHOWWINDOW:
      case WM_SIZE:
        mIsERASEBKGND = TRUE;
        break;

      case WM_ASKCBFORMATNAME:
        break;

      case WM_CHANGECBCHAIN:
        break;

      case WM_HSCROLLCLIPBOARD:
        break;

      default:
        break;
    }

  if ( next )
    {
      res = _boxApp::Message( msg, wParam, lParam );
    }
  else
    {
      res = DefaultMessage( mhWnd, msg, wParam, lParam );
    }
 
  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iSubApp::SetMiRect( HDC hDC, RECT *p_rc, int type )
{
  int   tool_size;
  RECT  rc;
  int   rate;
  int   heighti;
  int   heights;
  int   height_d1;
  int   height_d2;
  int   height_v1;
  int   height_v2;
  int   height_va;

  rc = *p_rc;

  rc.right = p_rc->right - BUTTON_SIZE;

  if ( m_tool_size == 0 )
    {
      m_tool_size = TOOLBER_SIZE;
    }

  tool_size = m_tool_size;

  if ( rc.right - rc.left < tool_size )
    {
      tool_size = rc.right - rc.left;
    }

  rc.top += TITLE_HEIGHT;

  heighti = mp_info_view->GetHeight( hDC );
  heights = mp_status_view->GetHeight( hDC );

  m_rcs[ IRC_SCROLL ].left   = rc.right;
  m_rcs[ IRC_SCROLL ].top    = rc.top;
  m_rcs[ IRC_SCROLL ].right  = p_rc->right;
  m_rcs[ IRC_SCROLL ].bottom = rc.bottom;

  m_rcs[ IRC_SHORTCUT2 ].left   = rc.left;
  m_rcs[ IRC_SHORTCUT2 ].top    = rc.top/* + heighti*/;
  m_rcs[ IRC_SHORTCUT2 ].right  = rc.left + tool_size;
  m_rcs[ IRC_SHORTCUT2 ].bottom = rc.bottom - heights - heighti;

  m_rcs[ IRC_SPLIT_LU ].left   = rc.left + tool_size - 3;
  m_rcs[ IRC_SPLIT_LU ].top    = rc.top/* + heighti*/;
  m_rcs[ IRC_SPLIT_LU ].right  = rc.left + tool_size + 3;
  m_rcs[ IRC_SPLIT_LU ].bottom = rc.bottom - heights - heighti;

  if ( type == 0 )
    {
      mp_act_seach1->SetEnable( TRUE );
      mp_act_dir1->SetEnable( TRUE, FALSE );
      mp_act_seach2->SetEnable( FALSE );
      mp_act_dir2->SetEnable( FALSE, FALSE );
      mp_act_dir = mp_act_dir1;
      rate = 5;
    }
  else
    {
      mp_act_seach1->SetEnable( FALSE );
      mp_act_dir1->SetEnable( FALSE, FALSE );
      mp_act_seach2->SetEnable( TRUE );
      mp_act_dir2->SetEnable( TRUE, FALSE );
      mp_act_dir = mp_act_dir2;
      rate = 1;
    }

  height_d1 = mp_act_dir1->GetHeight( hDC );
  height_d2 = mp_act_dir2->GetHeight( hDC );

  height_va = ( rc.bottom - rc.top ) - ( heighti + heights + height_d1 + height_d2 );
  height_v1 = height_va * rate / ( 6 );
  height_v2 = height_va - height_v1;

  if ( height_v1 < 0 )
    {
      height_v1 = 0;
    }

  if ( height_v2 < 0 )
    {
      height_v2 = 0;
    }

  if ( rc.bottom - rc.top < heighti + heights )
    {
      heighti   = rc.bottom - rc.top;
      height_d1 = 0;
      height_d2 = 0;
    }
  else if ( rc.bottom - rc.top < heighti + heights + height_d1 )
    {
      height_d1 = rc.bottom - rc.top - heighti - heights;
      height_d2 = 0;
    }
  else if ( rc.bottom - rc.top < heighti + heights + height_d1 + height_d2 )
    {
      height_d2 = rc.bottom - rc.top - heighti - heights - height_d1;
    }

  m_rcs[ IRC_DIR1 ].left     = rc.left + tool_size;
  m_rcs[ IRC_DIR1 ].top      = rc.top/* + heighti*/;
  m_rcs[ IRC_DIR1 ].right    = rc.right;
  m_rcs[ IRC_DIR1 ].bottom   = m_rcs[ IRC_DIR1 ].top + height_d1;

  m_rcs[ IRC_SEACH1 ].left   = rc.left + tool_size;
  m_rcs[ IRC_SEACH1 ].top    = m_rcs[ IRC_DIR1 ].bottom;
  m_rcs[ IRC_SEACH1 ].right  = rc.right;
  m_rcs[ IRC_SEACH1 ].bottom = m_rcs[ IRC_SEACH1 ].top + height_v1;

  m_rcs[ IRC_DIR2 ].left     = rc.left + tool_size;
  m_rcs[ IRC_DIR2 ].top      = m_rcs[ IRC_SEACH1 ].bottom;
  m_rcs[ IRC_DIR2 ].right    = rc.right;
  m_rcs[ IRC_DIR2 ].bottom   = m_rcs[ IRC_DIR2 ].top + height_d2;

  m_rcs[ IRC_SEACH2 ].left   = rc.left + tool_size;
  m_rcs[ IRC_SEACH2 ].top    = m_rcs[ IRC_DIR2 ].bottom;
  m_rcs[ IRC_SEACH2 ].right  = rc.right;
  m_rcs[ IRC_SEACH2 ].bottom = m_rcs[ IRC_SEACH2 ].top + height_v2;

  m_rcs[ IRC_INFO ].left   = rc.left;
  m_rcs[ IRC_INFO ].top    = m_rcs[ IRC_SEACH2 ].bottom;
  m_rcs[ IRC_INFO ].right  = rc.right;
  m_rcs[ IRC_INFO ].bottom = m_rcs[ IRC_INFO ].top + heighti;

  m_rcs[ IRC_STATUS ].left   = rc.left;
  m_rcs[ IRC_STATUS ].top    = rc.bottom - heights;
  m_rcs[ IRC_STATUS ].right  = rc.right;
  m_rcs[ IRC_STATUS ].bottom = rc.bottom;

  return rate;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL iSubApp::Draw( HWND hWnd, HDC hDC )
{
  RECT    rc;
  xTmpDC  dc( hDC );
  iPt     pt;

  ::GetClientRect( mhWnd, &rc );

  SetMiRect( hDC, &rc, mp_act_seach == mp_act_seach2 );

  dc.SetSelectPen( 0 );
  dc.SetSelectBrush( pt.COL_BASE_BAR );

  SetStatusView( FALSE );
  mp_status_view->DRAWR( hWnd, hDC, m_rcs[ IRC_STATUS ] );

  if ( !m_tree_mode )
    {
      mp_tree_view->SetCurrentPath( mp_act_seach->GetCurrentDirectory() );
      mp_tree_view->SetFocus( FALSE );
    }
  else
    {
      mp_tree_view->SetFocus( TRUE );
    }

  mp_tree_view->SetMarkPath( 0, mp_act_seach1->GetCurrentDirectory() );
  mp_tree_view->SetMarkPath( 1, mp_act_seach2->GetCurrentDirectory() );
  mp_tree_view->DRAWR( hWnd, hDC, m_rcs[ IRC_SHORTCUT2 ] );

  mp_info_view->TargetSet( mp_act_seach->GetCurrentEntry(), FALSE );
  mp_info_view->DRAWR( hWnd, hDC, m_rcs[ IRC_INFO ] );

  mp_act_dir1->SetStepBackAndUpFocus( mp_act_seach1->m_step_que.top() != NULL,
                                      mp_act_seach1->m_back_que.size() > 1,
                                     !mp_act_seach1->PathIsRoot() );
  mp_act_dir2->SetStepBackAndUpFocus( mp_act_seach2->m_step_que.top() != NULL,
                                      mp_act_seach2->m_back_que.size() > 1,
                                     !mp_act_seach2->PathIsRoot() );
  mp_act_dir1->SetDirectory1( mp_act_seach1->GetCurrentDirectory(), mp_act_seach1->GetDirCount(), FALSE );
  mp_act_dir2->SetDirectory1( mp_act_seach2->GetCurrentDirectory(), mp_act_seach2->GetDirCount(), FALSE );

  if ( m_rcs[ IRC_DIR1 ].right - m_rcs[ IRC_DIR1 ].left > 0 )
    {
      if ( m_rcs[ IRC_DIR1 ].bottom - m_rcs[ IRC_DIR1 ].top > 0 )
        {
          mp_act_dir1->DRAWR( hWnd, hDC, m_rcs[ IRC_DIR1 ] );
        }

      if ( m_rcs[ IRC_SEACH1 ].bottom - m_rcs[ IRC_SEACH1 ].top > 0 )
        {
          mp_act_seach1->DRAWR( hWnd, hDC, m_rcs[ IRC_SEACH1 ] );
        }

      if ( m_rcs[ IRC_DIR2 ].bottom - m_rcs[ IRC_DIR2 ].top > 0 )
        {
          mp_act_dir2->DRAWR( hWnd, hDC, m_rcs[ IRC_DIR2 ] );
        }

      if ( m_rcs[ IRC_SEACH2 ].bottom - m_rcs[ IRC_SEACH2 ].top > 0 )
        {
          mp_act_seach2->DRAWR( hWnd, hDC, m_rcs[ IRC_SEACH2 ] );
        }
    }

  SetScrollPos( FALSE );

  mp_scroll_view->DRAWR( hWnd, hDC, m_rcs[ IRC_SCROLL ] );

  SetTitle();

  return FALSE;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL iSubApp::Draw2( HWND hWnd, HDC hDC )
{
  xTmpDC  dc( hDC );
  RECT    rc;

  GetClientRect( &rc );

  dc.FillRect( &rc, RGB( 248, 248, 248 ) );

  return FALSE;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSubApp::SetScrollPos( BOOL redraw )
{
  SCROLLINFO  si;

  si.nMin = 0;

  if ( m_tree_mode )
    {
      si.nMax = mp_tree_view->GetTotalPage();
      si.nPos = mp_tree_view->GetCurrentPage();
    }
  else
    {
      si.nMax = mp_act_seach->GetTotalPage();
      si.nPos = mp_act_seach->GetCurrentPage();
    }

  mp_scroll_view->SetScrollInfo( &si, redraw );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iSubApp::GetLogicalDrives( void )
{
  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSubApp::Redraw( void )
{
  xDC   dc( mhWnd );

  Draw( mhWnd, dc );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
TCHAR *iSubApp::GetSelected( void )
{
  return mp_bdown_view->GetSelected();
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSubApp::SetFonts( const TCHAR *fname1, int size1,
                        const TCHAR *fname2, int size2,
                        const TCHAR *fname3, int size3,
                        const TCHAR *fname4, int size4 )
{
  mp_info_view->SetFont( fname4, size4 );

  mp_seach_view1->SetFont( fname2, size2 );
  mp_seach_view2->SetFont( fname2, size2 );

  mp_dir_view1->SetFont( fname1, size1 );
  mp_dir_view2->SetFont( fname1, size1 );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSubApp::SetColumn( int column )
{
  mp_act_seach1->SetColumn( column );
  mp_act_seach2->SetColumn( column );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSubApp::Sort( void )
{
  mp_act_seach1->Sort();
  mp_act_seach2->Sort();
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- ウインドウクラスの再設定
// --------------------------------------------------------------------------
void iSubApp::PresetWindowClass( WNDCLASSEX *pWC )
{
  pWC->hbrBackground  = NULL;
  pWC->hIcon          = ::LoadIcon( mhInstance, _T( "IDI_ICON1" ) );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSubApp::SetDirectorys( const TCHAR *dir1, const TCHAR *dir2 )
{
  mp_act_seach1->SetDirectory2( dir1 );
  mp_act_seach2->SetDirectory2( dir2 );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSubApp::SetDirectory( const TCHAR *dir, const TCHAR *prev_dir, int on_msg )
{
  mp_act_seach->SetDirectory2( dir, prev_dir, on_msg );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- マウスホイールの処理
// --------------------------------------------------------------------------
void iSubApp::OnMouseWheel( int x, int y, int zDelta, UINT fwKeys )
{
  if ( zDelta < 0 )
    {
      SendMessage( WM_VSCROLL, SB_LINERIGHT, 0 );
    }
  else
    {
      SendMessage( WM_VSCROLL, SB_LINELEFT, 0 );
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- WM_VSCROLLの処理
// --------------------------------------------------------------------------
void iSubApp::OnVScroll( HWND hwndCtl, UINT code, int pos )
{
  POINT cur_pos;
  int   id;

  GetCursorPos( &cur_pos );

  ScreenToClient( mhWnd, &cur_pos );

  id = thePtInRect( cur_pos.x, cur_pos.y );

  if ( m_tree_mode )
    {
      if ( ( id == IRC_SEACH1 || id == IRC_SEACH2 ) && code != SB_THUMBTRACK )
        {
          mp_seach_view1->SetFocus( TRUE );
          mp_seach_view2->SetFocus( TRUE );
          m_tree_mode = FALSE;
        }
      else
        {
          switch( code )
            {
              case SB_PAGERIGHT:
              case SB_LINERIGHT:
                mp_tree_view->PageDown();
                break;
              case SB_PAGELEFT:
              case SB_LINELEFT:
                mp_tree_view->PageUp();
                break;
              case SB_THUMBTRACK:
                mp_tree_view->PagePos( pos );
                break;
              default:
                break;
            }
        }
    }
  else
    {
      if ( id == IRC_SHORTCUT2 && code != SB_THUMBTRACK )
        {
          mp_seach_view1->SetFocus( FALSE );
          mp_seach_view2->SetFocus( FALSE );
          m_tree_mode = TRUE;
        }
      else
        {
          switch ( code )
            {
              case SB_PAGERIGHT:
              case SB_LINERIGHT:
                //
                mp_act_seach->PageDown();
                break;
                  
              case SB_PAGELEFT:
              case SB_LINELEFT:
                //
                mp_act_seach->PageUp();
                break;
                
              case SB_THUMBTRACK:
                //
                mp_act_seach->PagePos( pos );
                break;
                
              default:
                break;
            }
        }
    }

  Invalidate();
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL iSubApp::LetsCommand( int action, int id, LPARAM lParam )
{
  BOOL    res = FALSE;

  switch ( action )
    {
      case ISEACHVIEW_BASE_ID:
        mp_bdown_view = mp_act_seach;
        res = TRUE;
        break;

      case IDIRECTORY_BASE_ID:
        mp_bdown_view = mp_act_dir;

        if ( id == IDIRECTORYID_L_FOLDER )
          {
            mp_act_seach->SetDirectory2( mp_act_dir->GetSelected(), mp_act_dir->GetSelected2() );
            Invalidate();
          }
        else if ( id == IDIRECTORYID_BACK )
          {
            Back();
          }
        else if ( id == IDIRECTORYID_STEP )
          {
            Step();
          }
        else
          {
            res = TRUE;
          }

        break;

      case INAMELIST_BASE_ID:
        ( ( iNamelist* )lParam )->SeachLock();
        Redraw();
        break;

      case IEASYCOMMON_BASE_ID:
        if ( m_lock )
          {
            break;
          }

        SetSeachView( FALSE );

        if ( id == VK_ESCAPE )
          {
            mp_seach_view1->SetDirectory2();
            mp_seach_view2->SetDirectory2();
          }

        Invalidate();
        break;

      case ISTATUS_BASE_ID:
        mp_seach_view1->SetCopeingFile( ( TCHAR* )lParam );
        mp_seach_view2->SetCopeingFile( ( TCHAR* )lParam );
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
dirent *iSubApp::GetCurrentEntry( void )
{
  return mp_act_seach->GetCurrentEntry();
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL iSubApp::LetsKeyDown( WPARAM wParam, LPARAM lParam )
{
  BOOL  res = FALSE;

  if ( m_tree_mode )
    {
      if( wParam == VK_TAB /*&& HIBYTE( GetKeyState( VK_CONTROL ) )*/ || wParam == VK_BACK )
        {
          m_tree_mode = FALSE;
          mp_act_seach1->SetFocus( TRUE );
          mp_act_seach2->SetFocus( TRUE );
          mp_tree_view->SetFocus( FALSE );
          Invalidate();
        }
      else if ( wParam == VK_RETURN )
        {
          TCHAR buf[ MAX_PATH ];
          mp_tree_view->GetCurrentPath( buf );
          mp_act_seach->SetDirectory2( buf, NULL, 1 );
          SetScrollPos();
          Invalidate();
        }
      else
        {
          res = mp_tree_view->KeyDown( ( UINT )wParam, HIBYTE( GetKeyState( VK_CONTROL ) ), HIBYTE( GetKeyState( VK_SHIFT ) ), 0 );

          SetScrollPos();

          if ( res )
            {
              mp_tree_view->Redraw( mhWnd, &m_rcs[ IRC_SHORTCUT2 ] );
            }
        }
    }
  else
    {
      if ( wParam == VK_TAB )
        {
          if ( HIBYTE( GetKeyState( VK_CONTROL ) ) )
            {
              mp_seach_view1->SetFocus( FALSE );
              mp_seach_view2->SetFocus( FALSE );
              m_tree_mode = TRUE;
            }
          else
            {
              if( mp_act_seach == mp_act_seach1 )
                {
                  mp_act_seach = mp_act_seach2;
                }
              else
                {
                  mp_act_seach = mp_act_seach1;
                }
            }

          Invalidate();
        }
      else
        {
          res = mp_act_seach->KeyDown( ( UINT )wParam, HIBYTE( GetKeyState( VK_CONTROL ) ), HIBYTE( GetKeyState( VK_SHIFT ) ), 0 );

          mp_act_dir->SetStepBackAndUpFocus( mp_act_seach->m_step_que.top() != NULL,
                                             mp_act_seach->m_back_que.size() > 1,
                                            !mp_act_seach->PathIsRoot() );
          mp_act_dir->SetDirectory1( mp_act_seach->GetCurrentDirectory(), mp_act_seach->GetDirCount() );

          if ( wParam == VK_RETURN || wParam == VK_BACK || wParam == 0xDC )
            {
              mp_tree_view->SetCurrentPath( mp_act_seach->GetCurrentDirectory() );
              mp_tree_view->SetMarkPath( 0, mp_act_seach1->GetCurrentDirectory() );
              mp_tree_view->SetMarkPath( 1, mp_act_seach2->GetCurrentDirectory() );
              mp_tree_view->Redraw( mhWnd, &m_rcs[ IRC_SHORTCUT2 ] );
            }

          SetTitle();
          SetScrollPos();
          SetStatusView();
        }
    }

  return TRUE;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSubApp::GetCurrentDirectorys( TCHAR *path1, TCHAR *path2, int max )
{
  _tcscpy_s( path1, max, mp_seach_view1->GetCurrentDirectory() );
  _tcscpy_s( path2, max, mp_seach_view2->GetCurrentDirectory() );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iSubApp::GetCurrentPageNo( void )
{
  if ( mp_act_seach == mp_act_seach1 )
    {
      return 0;
    }

  return 1;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
TCHAR *iSubApp::GetCurrentDirectory( void )
{
  return mp_act_seach->GetCurrentDirectory(); 
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iSubApp::Back( void )
{
  const TCHAR*  back_path;
        TCHAR*  cp = mp_act_seach->Back( &back_path );

  if ( cp )
    {
      mp_act_seach->SetDirectory2( cp, back_path );
      Invalidate();
    }
}
// --------------------------------------------------------------------------
void iSubApp::Step( void )
{
  TCHAR*  cp = mp_act_seach->Step();

  if ( cp )
    {
      mp_act_seach->SetDirectory2( cp );
      Invalidate();
    }
}
// --------------------------------------------------------------------------


// ===========================================================================
// -- プロセスの起動
// ---------------------------------------------------------------------------
BOOL iSubApp::WinExec( TCHAR *pCmdline, const TCHAR *pPath )
{
  STARTUPINFO           startInfo;
  PROCESS_INFORMATION   procInfo;       // 起動するウィンドウの状態 
  BOOL                  is;

  ::GetStartupInfo( &startInfo );       // 現在のプロセスと同じ状態

  is = ::CreateProcess(
      pPath,                            // プロセスのパス（EXEのパス）
      pCmdline,                         // プロセスへのコマンドライン
      NULL,                             // ハンドル継承(プロセスへのセキュリティ)
      NULL,                             // ハンドル継承(スレッドへのセキュリティ)
      FALSE,                            // ハンドル継承を行うか否か
      /*CREATE_NEW_PROCESS_GROUP |*/    // プロセスタイプおよび優先順位
      /*BELOW_NORMAL_PRIORITY_CLASS |*/
      /*NORMAL_PRIORITY_CLASS |*/
      HIGH_PRIORITY_CLASS,
      NULL,                             // 環境ブロックのポインタ
      NULL,                             // プロセスのパス
      &startInfo,                       // 起動状態
      &procInfo );                      // プロセスの情報

  if ( is )
    {
      /* プロセスが起動されるまで待つ */

      ::WaitForInputIdle( procInfo.hProcess, INFINITE );
    }

  return is;
}
// ---------------------------------------------------------------------------


// ===========================================================================
// --
// ---------------------------------------------------------------------------
void iSubApp::GetCurrentPosition( int *x, int *y )
{
  mp_act_seach->GetCurrentPosition( x, y );
}
// ---------------------------------------------------------------------------


// ===========================================================================
// --
// ---------------------------------------------------------------------------
int iSubApp::thePtInRect( int x, int y )
{
  int   i;
  POINT pos;

  pos.x = x;
  pos.y = y;

  for ( i = 0; i < IRC_MAX; i++ )
    {
      if ( PtInRect( &m_rcs[ i ], pos ) )
        {
          break;
        }
    }

  return i;
}
// ---------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL iSubApp::LetsLButtonDown( int x, int y )
{
  BOOL  res = FALSE;

  m_splt_on = FALSE;

  switch( thePtInRect( x, y ) )
    {
      case IRC_SPLIT_LU:
        SetCursor( m_cursor_wait_icon );
        m_splt_on = TRUE;
        break;

      case IRC_SCROLL:
        mp_scroll_view->OnLButtonDown( x, y );
        break;

      case IRC_INFO:
        mp_info_view->OnLButtonDown( x, y );
        break;

      case IRC_SHORTCUT:
        break;

      case IRC_SHORTCUT2:
        if ( !m_tree_mode )
          {
            mp_seach_view1->SetFocus( FALSE );
            mp_seach_view2->SetFocus( FALSE );
            m_tree_mode = TRUE;
          }

        TCHAR buf[ MAX_PATH ];

        mp_tree_view->OnLButtonDown( x, y - m_rcs[ IRC_SHORTCUT2 ].top );
        mp_tree_view->GetCurrentPath( buf );
        mp_act_seach->SetDirectory2( buf );

        Invalidate();

        break;

      case IRC_DIR1:
        if ( mp_act_dir == mp_act_dir1 )
          {
            mp_act_dir->OnLButtonDown( x, y );
          }
        else
          {
            res = TRUE;
          }

        break;

      case IRC_DIR2:
        if ( mp_act_dir == mp_act_dir2 )
          {
            mp_act_dir->OnLButtonDown( x, y );
          }
        else
          {
            res = TRUE;
          }

        break;

      case IRC_SEACH1:
        if ( m_tree_mode )
          {
            mp_seach_view1->SetFocus( TRUE );
            mp_seach_view2->SetFocus( TRUE );
            m_tree_mode = FALSE;
            Invalidate();
          }

        if ( mp_act_seach == mp_act_seach1 )
          {
            mp_act_seach->OnLButtonDown( x, y );
          }
        else
          {
            mp_act_seach = mp_act_seach1;
            Invalidate();
          }

        break;

      case IRC_SEACH2:
        if ( m_tree_mode )
          {
            mp_seach_view1->SetFocus( TRUE );
            mp_seach_view2->SetFocus( TRUE );
            m_tree_mode = FALSE;
            Invalidate();
          }

        if ( mp_act_seach == mp_act_seach2 )
          {
            mp_act_seach->OnLButtonDown( x, y );
          }
        else
          {
            mp_act_seach = mp_act_seach2;
            Invalidate();
          }

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
BOOL iSubApp::LetsRButtonDown( int x, int y )
{
  BOOL  res = FALSE;

  switch ( thePtInRect( x, y ) )
    {
      case IRC_SCROLL:
        break;
      case IRC_SHORTCUT2:
        mp_tree_view->OnRButtonDown( x, y - m_rcs[ IRC_SHORTCUT2 ].top );
        break;
      case IRC_DIR1:
        if ( mp_act_dir == mp_act_dir1 )
          {
            mp_act_dir->OnRButtonDown( x, y );
          }
        else
          {
            res = TRUE;
          }
        break;
      case IRC_DIR2:
        if ( mp_act_dir == mp_act_dir2 )
          {
            mp_act_dir->OnRButtonDown( x, y );
          }
        else
          {
            res = TRUE;
          }
        break;
      case IRC_SEACH1:
        if ( mp_act_seach == mp_act_seach1 )
          {
            mp_act_seach->OnRButtonDown( x, y );
          }
        break;
      case IRC_SEACH2:
        if ( mp_act_seach == mp_act_seach2 )
          {
            mp_act_seach->OnRButtonDown( x, y );
          }
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
int iSubApp::GetMarkedNum( dirent** entrylist )
{
  return mp_act_seach->GetMarkedNum( entrylist );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iSubApp::GetMarkedNumTop( dirent** entrylist )
{
  return mp_act_seach1->GetMarkedNum( entrylist );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iSubApp::GetMarkedNumBottom( dirent** entrylist )
{
  return mp_act_seach2->GetMarkedNum( entrylist );
}
// --------------------------------------------------------------------------



// ==========================================================================
// -- タイトルバーに情報を表示
// --------------------------------------------------------------------------
void iSubApp::SetTitle()
{
  TCHAR   buf[ MAX_PATH + 10 ];
  TCHAR*  cp;
  RECT    rc;

  if ( m_lock != TRUE )
    {
      cp = mp_act_seach->GetCurrentDirectory();

      if ( cp )
        {
          _stprintf_s( buf, MAX_PATH, _T( "%s - [ %s %s ]" ), L"F's", _T( __DATE__ ), _T( __TIME__ ) );
          _tcscpy_s( mTitleBuf, MAX_PATH, cp );
          StrTrimRight( mTitleBuf, L"\\" );

          if ( mTitleBuf[ 1 ] == L':' && mTitleBuf[ 2 ] == L'\0' )
            {
              _stprintf_s( mTitleBuf, L"ルート: '%c'", mTitleBuf[ 0 ] );
            }
        }
      else
        {
          _stprintf_s( buf, MAX_PATH, _T( "\\ - [ %s %s ]" ), _T( __DATE__ ), _T( __TIME__ ) );
          _tcscpy_s( mTitleBuf, MAX_PATH, L"今日の予定は？" );
        }

      GetClientRect( &rc );

      SetWindowText( buf );
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSubApp::StopNotification()
{
  mp_act_seach1->StopNotification();
  mp_act_seach2->StopNotification();
  ::KillTimer( mhWnd, ISUBAPP_TIMEID );
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSubApp::StartNotification()
{
  ::SetTimer( mhWnd, ISUBAPP_TIMEID, ISUBAPP_TIMER, NULL );   // タイマー設定
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iSubApp::PtInActiveSeach( int x, int y )
{
  int   ptin   = thePtInRect( x, y );
  int   target = -1;

  switch( ptin )
    {
      case IRC_SEACH1:
        if ( mp_act_seach1 == mp_act_seach )
          {
            target = ptin;
          }

        break;

      case IRC_SEACH2:
        if ( mp_act_seach2 == mp_act_seach )
          {
            target = ptin;
          }

        break;

      case IRC_DIR1:
        if ( mp_act_dir1 == mp_act_dir )
          {
            target = ptin;
          }

        break;

      case IRC_DIR2:
        if ( mp_act_dir2 == mp_act_dir )
          {
            target = ptin;
          }

        break;

      case IRC_SHORTCUT:
        target = ptin;
        break;

      default:
        break;
    }

  return ptin;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSubApp::Idle()
{
  if (mp_act_seach1->FileSeach() > 0)
    {
      ;
    }
  else if (mp_act_seach2->FileSeach() > 0)
    {
      ;
    }
  else
    {
      _boxApp::Idle();
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
BOOL iSubApp::LetsTimer( UINT id )
{
  BOOL  next   = FALSE;
  int   update = 0;
  iPt   pt;

  switch( id )
    {
      case ISCROLL_TIMERID:
        mp_scroll_view->OnTimer( id );
        break;
        
      case ISUBAPP_TIMEID:
        update |= mp_tree_view->Update();

        if ( mp_act_seach1->IsChangeFolderState()/* && mp_act_seach1->GetMarkedNum() == 0*/ )
          {
            mp_act_seach1->SetDirectory2();
            update |= 1;
          }

        if ( mp_act_seach2->IsChangeFolderState()/* && mp_act_seach2->GetMarkedNum() == 0*/ )
          {
            mp_act_seach2->SetDirectory2();
            update |= 1;
          }

        if( update )
          {
            Invalidate();
          }

        mp_status_view->OnTimer( ISUBAPP_TIMEID );
        mp_tree_view->OnTimer( ISUBAPP_TIMEID );
        break;

      case ISUBAPP_SSSS:
        KillTimer( mhWnd, ISUBAPP_SSSS );
        SetDirectorys( pt.path1, pt.path2 );
        ::SetTimer( mhWnd, ISUBAPP_TIMEID, ISUBAPP_TIMER, NULL );   // タイマー設定
        Invalidate();
        break;

      case IINFO_LEAVE_TIMERID:
        mp_info_view->OnTimer( id );
        KillTimer( mhWnd, IINFO_LEAVE_TIMERID );
        break;

      case IINFO_WAIT_TIMERID:
        mp_info_view->OnTimer( id );
        KillTimer( mhWnd, IINFO_WAIT_TIMERID );
        break;

      case ISEACH_TIMERID:
        mp_act_seach->OnTimer( ISEACH_TIMERID );
        break;

      default:
        next = TRUE;
        break;
    }

  return next;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSubApp::SetCopyInfo( TCOPYACTION *act )
{
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSubApp::CompCopyInfo( TCOPYACTION *act )
{
  SetScrollPos();
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSubApp::EndCopyInfo( void )
{
  Invalidate();
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSubApp::StartCopyInfo( void )
{
  TCHAR       dir[MAX_PATH];
  HANDLE      hHeap;
  dirent*     top;
  iResultDlg  dlg(mhWnd);
  BOOL        isCancel;

  hHeap = ::HeapCreate(HEAP_NO_SERIALIZE, 10000, 0);

  if ( !hHeap )
    {
      return;
    }

  _tcscpy_s(dir, MAX_PATH, GetCurrentDirectory());

  SetCopyView(TRUE);

  top = ReadClips(mhWnd, hHeap, dir);

  isCancel = CheckCopyInfo(mhWnd, top, dir);

  SetSeachView();

  if (isCancel)
    {
      if (!mp_status_view->doCopy(mhWnd, top, dir))
        {
          dlg.SetExtParams(top, dir, 0);
          dlg.Modal();
        }

      Refresh(dir, top);
    }

  ::HeapDestroy(hHeap);
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSubApp::CancelCopyInfo( void )
{
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSubApp::FindDirentMark( const TCHAR *name )
{
  dirent* entry = mp_act_seach->FindDirent( name );

  if ( entry )
    {
      entry->bookMark = 1;
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSubApp::MoveNextMark( int current )
{
  mp_act_seach->MoveNextMark( current );
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSubApp::SetCopyView( BOOL redraw )
{
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSubApp::SetSeachView( BOOL redraw )
{
  if ( mp_act_dir == mp_act_dir1 )
    {
      mp_act_dir1 = mp_dir_view1;
      mp_act_dir2 = mp_dir_view2;
      mp_act_dir  = mp_act_dir1;
    }
  else if ( mp_act_dir == mp_act_dir2 )
    {
      mp_act_dir1 = mp_dir_view1;
      mp_act_dir2 = mp_dir_view2;
      mp_act_dir  = mp_act_dir2;
    }

  if ( mp_act_seach == mp_act_seach1 )
    {
      mp_act_seach1 = mp_seach_view1;
      mp_act_seach2 = mp_seach_view2;
      mp_act_seach  = mp_act_seach1;
    }
  else if ( mp_act_seach == mp_act_seach2 )
    {
      mp_act_seach1 = mp_seach_view1;
      mp_act_seach2 = mp_seach_view2;
      mp_act_seach  = mp_act_seach2;
    }

  if ( redraw )
    {
      Invalidate();
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iSubApp::GetTotalCopyFile( void )
{
  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSubApp::SelectSeachMode( const TCHAR *cardlist )
{
  mp_act_seach->SetWildcard( cardlist );
  mp_act_seach->SelectSeachMode( TRUE );
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSubApp::Refresh( TCHAR *dir, dirent *namelist )
{
  mp_info_view->Refresh();
  mp_tree_view->Refresh();
  mp_status_view->Refresh();

  mp_act_seach->ForceUpadteEnable( TRUE );

  SetDirectorys();

  if ( namelist )
    {
      mp_act_seach1->SetMarkFromNamelist( dir, namelist );
      mp_act_seach2->SetMarkFromNamelist( dir, namelist );
    }

  Invalidate();
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSubApp::SetStatusView( BOOL redraw )
{
  int             d_cnt  = mp_act_seach->GetFileCount();
  int             f_cnt  = mp_act_seach->GetDirCount();
  ULARGE_INTEGER  f_size = mp_act_seach->GetTotalFileSize();
  int             marked_d_cnt;
  int             marked_f_cnt;
  ULARGE_INTEGER  marked_f_size;

  mp_act_seach->GetMarkedInfo( &marked_d_cnt, &marked_f_cnt, &marked_f_size );

  mp_status_view->SetStatus( d_cnt, f_cnt, &f_size, marked_f_cnt, marked_d_cnt, &marked_f_size );

  if ( redraw )
    {
      mp_status_view->Redraw();
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSubApp::OnSize( UINT state, int cx, int cy )
{
  mp_act_seach1->OnSize( state, cx, cy );
  mp_act_seach2->OnSize( state, cx, cy );
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iSubApp::OnSizing( UINT nSide, LPRECT lpRect )
{
  RECT  rc = *lpRect;

  GetClientRect( &rc );

  mp_act_seach1->OnSizing( nSide, &rc );
  mp_act_seach2->OnSizing( nSide, &rc );
}
// --------------------------------------------------------------------------
