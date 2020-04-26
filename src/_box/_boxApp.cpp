/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: _boxApp.cpp
 *  Created  : 08/06/04(水) 08:28:31
 *
 *  Function
 *    ウインドウアプリケーションのメインクラス
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/07/27 10:32:48 $
 *  $Author: poko $
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 */
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>
#include <crtdbg.h>
#include "_boxCore.h"
#include "_boxMsgCore.h"
#include "_boxApp.h"


// ==========================================================================
//  -- _boxAppクラスの構築 --
// --------------------------------------------------------------------------
_boxApp::_boxApp()
{
  mStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

  if ( theTopLevelWindow == NULL )
    {
      theTopLevelWindow = this;
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
//  -- ウインドウクラスの初期化!!!
// --------------------------------------------------------------------------
int _boxApp::Initialize( HINSTANCE hInstance )
{
  WNDCLASSEX  wc;

  if ( hInstance == NULL )
    {
      hInstance = ::GetModuleHandle( NULL );
    }

  /* インスタンスを保持 */

  mhInstance      = hInstance;

  wc.cbSize        = sizeof( WNDCLASSEX );
  wc.hInstance     = hInstance;                               // このインスタンスのハンドル
  wc.lpszClassName = mpClassName;                             // ウィンドウクラスの名前
  wc.lpfnWndProc   = InitWindowFunc;                          // ウィンドウ関数
  wc.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;    // デフォルトのスタイル
  wc.hIcon         = LoadIcon( NULL, IDI_APPLICATION );       // 標準のアイコン
  wc.hIconSm       = NULL;                                    // 小さいアイコン
  wc.hCursor       = LoadCursor( NULL, IDC_ARROW );           // カーソルのスタイル
  wc.lpszMenuName  = NULL;                                    // メニューを使用しない
  wc.cbClsExtra    = 32;                                      // その他の情報は必要ない
  wc.cbWndExtra    = 32;                                      // その他の情報は必要ない
  wc.hbrBackground = ( HBRUSH )GetStockBrush( WHITE_BRUSH );  // ウィンドウの背景色

  /* ウインドウクラスの再設定 */

  PresetWindowClass( &wc );

  return RegisterClassEx( &wc );
}
// --------------------------------------------------------------------------


// ==========================================================================
//  -- メッセージループ!!
// --------------------------------------------------------------------------
int _boxApp::Run()
{
  MSG   msg;

  /* メッセージループを作成する */

  while ( 1 )
    {
      if ( !PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
        {
          Idle();         // メッセージのない時はメインループ
        }
      else if ( GetMessage( &msg, NULL, 0, 0 ) <= 0 )
        {
          break;          // 終了！
        }
      else
        {
          TranslateMessage( &msg );
          DispatchMessage( &msg );
        }
    }

  return ( int )msg.wParam;
}
// --------------------------------------------------------------------------


// ==========================================================================
//  -- ウインドウの作成
// --------------------------------------------------------------------------
HWND _boxApp::Create( int x, int y, int cx, int cy )
{
  InitCommonControls();

  return _boxCore::Create( ( HWND )NULL, x, y, cx, cy );
}
// --------------------------------------------------------------------------


// ==========================================================================
//  -- アプリの終了
// --------------------------------------------------------------------------
void _boxApp::OnDestroy()
{
  ::PostQuitMessage( 0 );
}
// --------------------------------------------------------------------------
