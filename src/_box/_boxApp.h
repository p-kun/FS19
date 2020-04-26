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
#ifndef __BOXAPP_H_
#define __BOXAPP_H_

/*===========================================================================

	-- _boxApp クラス --

 ===========================================================================*/
class _boxApp : public _boxMsgCore
{
public:
	int		Initialize( HINSTANCE hInstance = NULL );
	HWND	Create(
				int x  = CW_USEDEFAULT,
				int y  = CW_USEDEFAULT,
				int cx = CW_USEDEFAULT,
				int cy = CW_USEDEFAULT );

public:
	virtual int		Run();

public:
	void	OnDestroy();

protected:
	virtual void	PresetWindowClass( WNDCLASSEX *pWC ) {}
	virtual void	Idle() { ::WaitMessage(); }

public:
	_boxApp();

};
/*==========================================================================*/

#endif	/* __BOXAPP_H_ */
