/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: _boxMsgCore.cpp
 *  Created  : 08/06/04(水) 09:00:33
 *
 *  Function
 *    ウインドウズのメッセージ処理クラス
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/07/27 10:32:48 $
 *  $Author: poko $
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 */
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <tchar.h>
#include "_boxCore.h"
#include "_boxMsgCore.h"
#include "_forward.cpp"
#include "_handle1.cpp"
#include "_handle2.cpp"


// ==========================================================================
// -- メッセージ毎の処理
// --------------------------------------------------------------------------
int _boxMsgCore::OnMessage( UINT msg, WPARAM wParam, LPARAM lParam )
{
  return (int)::DefWindowProc( mhWnd, msg, wParam, lParam );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- メッセージ毎の処理
// --------------------------------------------------------------------------
LRESULT _boxMsgCore::Message( UINT msg, WPARAM wParam, LPARAM lParam )
{
  if ( msg < WM_USER && msgFuncs[ msg ] )
    {
      return ( this->*msgFuncs[ msg ] )( wParam, lParam );
    }

  return OnMessage( msg, wParam, lParam );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- デフォルトメッセージ処理
// --------------------------------------------------------------------------
LRESULT _boxMsgCore::DefaultMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  return ::DefWindowProc( hWnd, msg, wParam, lParam );
}
// --------------------------------------------------------------------------
