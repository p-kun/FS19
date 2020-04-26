/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: xDialog.cpp
 *  Created  : 08/06/04(水) 08:28:31
 *
 *  Function
 *    コントロールの基底クラス
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/07/27 10:32:49 $
 *  $Author: poko $
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 */
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdio.h>
#include <tchar.h>
#include "_boxCore.h"
#include "_boxMsgCore.h"
#include "xControls.h"
#include "xDialog.h"


// ==========================================================================
// -- ダイアログボックスのメインのコールバック
// --------------------------------------------------------------------------
INT_PTR CALLBACK xDialog::DialogMainProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
  INT_PTR   res;

  xDialog*  pDlg = ( xDialog* )::GetWindowLongPtr( hDlg, GWLP_USERDATA );

  pDlg->ResultDialog( -1 );

  res = pDlg->Message( msg, wParam, lParam );

  if ( pDlg->mResult >= 0 )
    {
      res = pDlg->mResult;
    }

  return res;
}
// --------------------------------------------------------------------------



// ==========================================================================
// -- ダイアログボックスの初期コールバック
// --------------------------------------------------------------------------
INT_PTR CALLBACK xDialog::DialogInitProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
  INT_PTR   res = FALSE;
  xDialog*  pDlg;

  if ( msg == WM_INITDIALOG )
    {
      pDlg = ( xDialog* )lParam;

      pDlg->mhWnd = hDlg;
      ::SetWindowLongPtr( hDlg, DWLP_DLGPROC, ( LPARAM )DialogMainProc );
      ::SetWindowLongPtr( hDlg, GWLP_USERDATA, ( LONG_PTR )pDlg );

      res = pDlg->Message( msg, wParam, lParam );
    }

  return res;
}
// --------------------------------------------------------------------------



// ==========================================================================
// -- クラスの構築
// --------------------------------------------------------------------------
xDialog::xDialog( const TCHAR *pTemplate, HWND hParent )
{
  mpTemplate  = pTemplate;
  mhParent  = hParent;
}
// --------------------------------------------------------------------------



// ==========================================================================
// -- クラスの構築
// --------------------------------------------------------------------------
xDialog::xDialog( const TCHAR *pTemplate, _boxCore *pParent )
{
  mpTemplate = pTemplate;
  mhParent   = pParent->mhWnd;
}
// --------------------------------------------------------------------------



// ==========================================================================
// -- クラスが破棄されたときの処理
// --------------------------------------------------------------------------
xDialog::~xDialog()
{
}
// --------------------------------------------------------------------------



// ==========================================================================
// -- モードレスダイアログの作成
// --------------------------------------------------------------------------
HWND xDialog::Create()
{
  return ::CreateDialogParam( ::GetModuleHandle( NULL ), mpTemplate, mhParent, DialogInitProc, ( LPARAM )this );
}
// --------------------------------------------------------------------------



// ==========================================================================
// -- デフォルトメッセージ処理
// --------------------------------------------------------------------------
LRESULT xDialog::DefaultMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  return FALSE;
}
// --------------------------------------------------------------------------



// ==========================================================================
// -- その他のメッセージ毎の処理
// --------------------------------------------------------------------------
int xDialog::OnMessage( UINT msg, WPARAM wParam, LPARAM lParam )
{
  return FALSE;
}
// --------------------------------------------------------------------------



// ===========================================================================
// -- 時間のかかるループ中にメッセージの処理をさせるときに呼ぶ
// ---------------------------------------------------------------------------
void xDialog::PumpMessage()
{
  MSG   msg;

  while ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
    {
      if ( !::IsDialogMessage( mhWnd, &msg ) )
        {
          if ( msg.message == WM_PAINT || msg.message == WM_SYSCOMMAND )
            {
              ::TranslateMessage( &msg );
              ::DispatchMessage ( &msg );
            }
        }
    }
}
// ---------------------------------------------------------------------------



// ===========================================================================
// -- モーダルダイアログの表示
// ---------------------------------------------------------------------------
int xDialog::Modal()
{
  return ( int )::DialogBoxParam( ::GetModuleHandle( NULL ), mpTemplate, mhParent, DialogInitProc, ( LPARAM )this );
}
// ---------------------------------------------------------------------------



// ===========================================================================
// -- モーダルダイアログボックスを破棄し、処理を終了させます。
// ---------------------------------------------------------------------------
BOOL xDialog::EndDialog( int result )
{
  return ::EndDialog( mhWnd, result );
}
// ---------------------------------------------------------------------------



// ===========================================================================
// -- 指定された ID を持つコントロールのウィンドウハンドルを取得
// ---------------------------------------------------------------------------
HWND xDialog::GetDlgItem( int iID )
{
  return ::GetDlgItem( mhWnd, iID );
}
// ---------------------------------------------------------------------------



// ===========================================================================
// -- ウインドウを中央に移動
// ---------------------------------------------------------------------------
BOOL xDialog::CenterWindow()
{
  RECT  rcThis;
  RECT  rcOwner;
  int   width;
  int   height;

  ::GetWindowRect( mhParent, &rcOwner );
  GetWindowRect( &rcThis );

  width  = rcOwner.right  - rcOwner.left;
  height = rcOwner.bottom - rcOwner.top;

  width  -= ( rcThis.right - rcThis.left );
  height -= ( rcThis.bottom - rcThis.top );

  width  /= 2;
  height /= 2;

  return MoveWindow( rcOwner.left  + width,
                     rcOwner.top   + height,
                     rcThis.right  - rcThis.left,
                     rcThis.bottom - rcThis.top );
} 
// ---------------------------------------------------------------------------



// --------------------------------------------------------------------------
int xDialog::Attach( _boxCore& obj, int id )
{
  return obj.Attach( GetDlgItem( id ) );
}
// --------------------------------------------------------------------------



// --------------------------------------------------------------------------
int xDialog::Attach( _boxCore* obj, int id )
{
  return obj->Attach( GetDlgItem( id ) );
}
// --------------------------------------------------------------------------



// ==========================================================================
// -- コントロールにクラスを割り付け
// --------------------------------------------------------------------------
void xDialog::OnDrawItem( const DRAWITEMSTRUCT *pDIS )
{
  xControls*  pCtl = ( xControls* )::GetWindowLongPtr( ( HWND )pDIS->hwndItem, GWLP_USERDATA );

  if ( pCtl )
    {
      mResult = pCtl->DrawItem( pDIS );
    }
}
// --------------------------------------------------------------------------



// ===========================================================================
// -- オーナー描画コントロールの寸法を設定
// ---------------------------------------------------------------------------
void xDialog::OnMeasureItem( MEASUREITEMSTRUCT *pMIS )
{
  xControls*  pCtl = ( xControls* )::GetWindowLongPtr( ( HWND )GetDlgItem( pMIS->CtlID ), GWLP_USERDATA );

  if ( pCtl )
    {
      mResult = pCtl->MeasureItem( pMIS );
    }
}
// ---------------------------------------------------------------------------
