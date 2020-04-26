/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: _boxCore.cpp
 *  Created  : 08/06/04(水) 08:28:31
 *
 *  Function
 *    ウインドウズのAPI一覧クラス
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/07/27 10:32:48 $
 *  $Author: poko $
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 */
#include <windows.h>
#include <windowsx.h>
#include <assert.h>
#include <tchar.h>
#include "_boxCore.h"


// -- ローカル定義！
//
#define SET_WINDOW_LONG
#define SC_CLASS_NAME "SHINOSAN2"


// - private parameter(s)
//
HINSTANCE _boxCore::mhInstance        = NULL;   // アプリケーションインスタンス
_boxCore* _boxCore::theTopLevelWindow = NULL;
TCHAR     _boxCore::mErrMsg[ 128 ];
FLOAT     _boxCore::dpiX = 96;                  // 96dpi;
FLOAT     _boxCore::dpiY = 96;                  // 96dpi;


// ==========================================================================
// -- メインウインドウコールバックルーチン
// --------------------------------------------------------------------------
LRESULT CALLBACK _boxCore::MainWindowFunc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
#ifndef SET_WINDOW_LONG
  _boxCore* pBase = ( _boxCore* )GetProp( hWnd, mpClassName );
#else
  _boxCore* pBase = ( _boxCore* )GetWindowLongPtr( hWnd, GWLP_USERDATA );  
#endif

  assert( pBase );

  return pBase->Message( msg, wParam, lParam );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- メインウインドウコールバックルーチン
// --------------------------------------------------------------------------
LRESULT CALLBACK _boxCore::InitWindowFunc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  LRESULT       res;
  CREATESTRUCT* pCrStruct;
  _boxCore*     pBase;

  if ( msg == WM_CREATE )
    {
      pCrStruct   = ( CREATESTRUCT* )lParam;
      pBase     = ( _boxCore* )pCrStruct->lpCreateParams;
      pBase->mhWnd  = hWnd;
      pBase->Attach( hWnd );

      res = pBase->Message( msg, wParam, lParam );
    }
  else
    {
      res = ::DefWindowProc( hWnd, msg, wParam, lParam );
    }
  
  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- _boxCoreクラスの構築 --
// --------------------------------------------------------------------------
_boxCore::_boxCore()
{
  mhWnd         = NULL;
  mStyle        = WS_VISIBLE | WS_CHILD | WS_BORDER;
  mExStyle      = 0;
  mpClassName   = _T( SC_CLASS_NAME );  // クラス名設定
  mpCaptionName = _T( "" );             // ウインドウテキスト設定
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- クラスが破棄されたときの処理
// --------------------------------------------------------------------------
_boxCore::~_boxCore()
{
  if ( IsWindow( mhWnd ) )
    {
      /* ウインドウがまだ存在していたら破棄！ */

#ifndef SET_WINDOW_LONG
      SubclassWindow( mhWnd, InitWindowFunc );
#else
      ::SetWindowLongPtr( mhWnd, GWLP_WNDPROC, ( LONG_PTR )InitWindowFunc );
#endif
      ::DestroyWindow( mhWnd );
    }

#ifndef SET_WINDOW_LONG
  RemoveProp( mhWnd, mpClassName );
#endif
}
// --------------------------------------------------------------------------


// ===========================================================================
// -- 時間のかかるループ中にメッセージの処理をさせるときに呼ぶ
// ---------------------------------------------------------------------------
UINT _boxCore::PumpMessage( void )
{
  MSG   msg;

  while ( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
    {
      if ( msg.message == WM_QUIT )
        {
          break;
        }
      if ( GetMessage( &msg, NULL, 0, 0 ) <= 0 )
        {
          break;          // 終了！
        }
      ::TranslateMessage( &msg );
      ::DispatchMessage ( &msg );
    }

  return msg.message;
}
// ---------------------------------------------------------------------------


// ==========================================================================
// -- ウインドウの作成
// --------------------------------------------------------------------------
HWND _boxCore::Create( _boxCore *pParent, int x, int y, int cx, int cy )
{
  return Create( pParent->mhWnd, x, y, cx, cy );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- ウインドウの作成
//  [I] parent     : 親ウインドウのオブジェクト
//  [R] 0      : 失敗
//    others     : ウインドウハンドル
// --------------------------------------------------------------------------
HWND _boxCore::Create( HWND hWnd, int x, int y, int cx, int cy )
{
  HWND  hMyWnd;

  hMyWnd = ::CreateWindowEx(
              mExStyle,       // 拡張ウインドウスタイル
              mpClassName,    // ウインドウクラス名
              mpCaptionName,  // キャプション名
              mStyle,         // ウインドウスタイル
              x, y,           // ウインドウの位置
              cx, cy,         //         大きさ
              hWnd,           // 親ウインドウのハンドル
              NULL,
              mhInstance,     // アプリケーションインスタンス
              this            // 自分自身のポインタ
  );

  return hMyWnd;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- コントロールのサブクラス化
// --------------------------------------------------------------------------
int _boxCore::Attach( HWND hWnd )
{
  mhWnd = hWnd;

  mpControlFunc = ( WNDPROC )::GetWindowLongPtr( hWnd, GWLP_WNDPROC );

#ifndef SET_WINDOW_LONG
  SetProp( hWnd, mpClassName, pBase );
  SubclassWindow( hWnd, MainWindowFunc );
#else
  ::SetWindowLongPtr( hWnd, GWLP_WNDPROC, ( LPARAM )( WNDPROC )MainWindowFunc );
  ::SetWindowLongPtr( hWnd, GWLP_USERDATA, ( LONG_PTR )this );
#endif

  return 1;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- ラストエラーの内容表示
// --------------------------------------------------------------------------
TCHAR *_boxCore::GetLastErrString( DWORD *pErr )
{
  DWORD err = GetLastError();
  TCHAR buf[ 128 ];

  FormatMessage( 
    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    err,
    MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),    // デフォルト言語
    ( LPTSTR )buf,
    128,
    NULL 
  );

  wsprintf( mErrMsg, _T( "%s(0x%08X)" ), buf, err );

  if( pErr )
    {
      *pErr = err;
    }

  return mErrMsg;
}
// --------------------------------------------------------------------------


// ===========================================================================
// -- APIラップ一覧
// ---------------------------------------------------------------------------
int  _boxCore::GetWindowText( TCHAR* pString, int len ) { return ::GetWindowText( mhWnd, pString, len ); }
int  _boxCore::SetWindowText( const TCHAR* pString ) { return ::SetWindowText( mhWnd, pString ); }
int  _boxCore::ShowWindow( int show ) { return ::ShowWindow( mhWnd, show ); }
int  _boxCore::UpdateWindow() { return ::UpdateWindow( mhWnd ); }
LRESULT _boxCore::SendMessage( UINT msg, WPARAM wParam, LPARAM lParam )
  { return ::SendMessage( mhWnd, msg, wParam, lParam ); }
LRESULT _boxCore::SendMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
  { return ::SendMessage( hWnd, msg, wParam, lParam ); }
LRESULT _boxCore::PostMessage( UINT msg, WPARAM wParam, LPARAM lParam )
  { return ::PostMessage( mhWnd, msg, wParam, lParam ); }
BOOL _boxCore::GetClientRect( RECT *rc ) { return ::GetClientRect( mhWnd, rc ); }
BOOL _boxCore::GetWindowRect( RECT *rc ) { return ::GetWindowRect( mhWnd, rc ); }
BOOL _boxCore::Invalidate( BOOL isErase ) { return ::InvalidateRect( mhWnd, NULL, isErase ); }
BOOL _boxCore::MoveWindow( int x, int y, int w, int h, BOOL isRepaint )
  { return ::MoveWindow( mhWnd, x, y, w, h, isRepaint ); }
BOOL _boxCore::MoveWindow( RECT *rc, BOOL isRepaint )
  { return ::MoveWindow( mhWnd, rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top, isRepaint ); }
_boxCore *_boxCore::GetParent() { return ( _boxCore* )::GetWindowLongPtr( ::GetParent( mhWnd ), GWLP_USERDATA ); }
_boxCore *_boxCore::GetClassPtr() { return ( _boxCore* )::GetWindowLongPtr( mhWnd, GWLP_USERDATA ); }
BOOL _boxCore::GetWindowPlacement( WINDOWPLACEMENT *pPlace ) { return ::GetWindowPlacement( mhWnd, pPlace ); }
HDC  _boxCore::BeginPaint( PAINTSTRUCT *pPaint ) { return ::BeginPaint( mhWnd, pPaint ); }
void _boxCore::EndPaint( PAINTSTRUCT *pPaint ) { ::EndPaint( mhWnd, pPaint ); }
int  _boxCore::MessageBox( const TCHAR *pText, const TCHAR *pCaption, UINT type )
  { return ::MessageBox( mhWnd, pText, pCaption, type ); }
UINT_PTR _boxCore::SetTimer( UINT_PTR iD, UINT timout, TIMERPROC pTimrFunc )
  { return ::SetTimer( mhWnd, iD, timout, pTimrFunc ); }
HWND _boxCore::SetFocus() { return ::SetFocus( mhWnd ); }
HWND _boxCore::SetActiveWindow() { return ::SetActiveWindow( mhWnd ); }
BOOL _boxCore::SetWindowPos( HWND hInsWnd, int x, int y, int cx, int cy, UINT uFlags )
  { return ::SetWindowPos( mhWnd, hInsWnd, x, y, cx, cy, uFlags ); }
void _boxCore::SetWindowLong( int nIndex, LONG dwNewLong ) { ::SetWindowLong( mhWnd, nIndex, dwNewLong ); }
BOOL _boxCore::Destroy() { return ::DestroyWindow( mhWnd ); }
// ---------------------------------------------------------------------------


void _boxCore::UpdateScale( void )
{
  HDC screen = GetDC(0);

  dpiX = static_cast<FLOAT>(GetDeviceCaps(screen, LOGPIXELSX));
  dpiY = static_cast<FLOAT>(GetDeviceCaps(screen, LOGPIXELSY));

  ReleaseDC(0, screen);
}
