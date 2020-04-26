/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: _boxMsgCore.h
 *  Created  : 08/06/04(水) 08:28:31
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
#ifndef __BOXMSGCORE_H_
#define __BOXMSGCORE_H_

/*===========================================================================

	-- _boxMsgCore クラス --

 ===========================================================================*/
class _boxMsgCore : public _boxCore
{
	// ----------------------------------------------------------------------
	//	-- 公開Windowメッセージ関数一覧
	//     公開メッセージ別SendMessage関数一覧
	// ----------------------------------------------------------------------
public :
	#include "_forward.hpp"
	#include "_handle1.hpp"

public :
	_boxMsgCore*	GetParent() { return ( _boxMsgCore* )_boxCore::GetParent(); }

	// ----------------------------------------------------------------------
	//	-- メッセージ処理用仮想関数
	// ----------------------------------------------------------------------
protected:
	virtual int     OnMessage( UINT msg, WPARAM wParam, LPARAM lParam );
	virtual LRESULT DefaultMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

public:
	virtual LRESULT Message( UINT msg, WPARAM wParam, LPARAM lParam );

#ifdef __DEBUG__
public:
	static const char* const msgStrings[];
#endif
};
/*==========================================================================*/

#endif	/* __BOXMSGCORE_H_ */
