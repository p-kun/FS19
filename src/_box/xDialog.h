/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: xDialog.h
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
#ifndef _XDIALOG_H_
#define _XDIALOG_H_

/*===========================================================================

	-- xDialog クラス --

 ===========================================================================*/
class xDialog : public _boxMsgCore
{
public:
	HWND	Create();
	int		Modal();
	BOOL	EndDialog( int result );
	HWND	GetDlgItem( int iID );
	void	PumpMessage();
	BOOL	CenterWindow();
	int		Attach( _boxCore& obj, int id );
	int		Attach( _boxCore* obj, int id );
	void	ResultDialog( int result ) { mResult = result; }		// メッセージ処理内で使用する

protected:
	virtual LRESULT DefaultMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	virtual int  OnMessage( UINT msg, WPARAM wParam, LPARAM lParam );
	virtual void OnDrawItem( const DRAWITEMSTRUCT *pDIS );
	virtual void OnMeasureItem( MEASUREITEMSTRUCT *pMIS );

public:
	xDialog( const TCHAR *pTemplate, HWND hParent );
	xDialog( const TCHAR *pTemplate, _boxCore *pParent );
	virtual ~xDialog();

protected:
	WNDPROC			mpControlFunc;
	HWND			mhParent;
	const TCHAR*	mpTemplate;
	int				mResult;

protected:
	static INT_PTR CALLBACK DialogInitProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );
	static INT_PTR CALLBACK DialogMainProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );

};
/*==========================================================================*/

#endif	/* _XDIALOG_H_ */
