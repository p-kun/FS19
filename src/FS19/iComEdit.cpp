/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iComEdit.cpp
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
#include "iComEdit.h"


// ==========================================================================
// -- 上下キーのみ親にメッセージを送るよ
// --------------------------------------------------------------------------
void iComEdit::OnKeyDown( UINT vk, BOOL fDown, int cRepeat, UINT flags )
{
	switch ( vk )
    {
    	case VK_PRIOR:
    	case VK_NEXT:
    	case VK_UP:
    	case VK_DOWN:
    		GetParent()->SendMessage( WM_COMMAND, GET_WM_COMMAND_MPS( ::GetDlgCtrlID( mhWnd ), mhWnd, ICOMEDIT_KEY | ( vk & 0xFF ) ) );
    		break;
    	case VK_INSERT:
    		if ( HIBYTE( GetKeyState( VK_CONTROL ) ) == 0 && HIBYTE( GetKeyState( VK_SHIFT ) ) == 0 )
      	  {
      			GetParent()->SendMessage( WM_COMMAND, GET_WM_COMMAND_MPS( ::GetDlgCtrlID( mhWnd ), mhWnd, ICOMEDIT_KEY | ( vk & 0xFF ) ) );
      			break;
      		}
    	default:
    		xEdit::OnKeyDown( vk, fDown, cRepeat, flags );
    		break;
  	}
}
// --------------------------------------------------------------------------
