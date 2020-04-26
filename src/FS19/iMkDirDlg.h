/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iMkDirDlg.h
 *  Created  : 08/06/04(水) 08:28:31
 *
 *  Function
 *    ディレクトリの作成
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/07/27 10:32:49 $
 *  $Author: poko $
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 */
#ifndef _IMKDIRDLG_H_
#define _IMKDIRDLG_H_


class xFont;
class iComEdit;

/*===========================================================================

	-- iMkDirDlg クラス --

 ===========================================================================*/
class iMkDirDlg : public xDialog
{
public:
	void	  SetFont( const TCHAR *name, int size );
	void	  SetDirectory( const TCHAR *name );
	TCHAR*	GetSel( void );
	
public:
	iMkDirDlg( HWND hParent );
	~iMkDirDlg();

private:
	BOOL OnInitDialog( HWND hwndFocus, LPARAM lParam );
	void OnCommand( int id, HWND hwndCtl, UINT codeNotify );

private:
	iComEdit*	mpEdit;
	xFont*		mpFont;
	xStatic		mStatic;
	int			  m_mode;
	TCHAR		  m_dir[ MAX_PATH ];
	TCHAR		  m_buf[ MAX_PATH * 2 ];

};
/*==========================================================================*/


#endif//_IMKDIRDLG_H_
