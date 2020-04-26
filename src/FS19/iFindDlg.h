/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iFindDlg.h
 *  Created  : 08/06/04(水) 08:28:31
 *
 *  Function
 *    ファインダイアログ
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/07/27 10:32:49 $
 *  $Author: poko $
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 */
#ifndef _IFINDDLG_H_
#define _IFINDDLG_H_


class xFont;
class xComboBox;
// ===========================================================================
// -- iFindDlgクラス
// ---------------------------------------------------------------------------
class iFindDlg : public xDialog
{
public:
	void  SetFileInfo( dirent* pFD );
	void  SetFileInfo( const TCHAR *name );
	void  SetFont( const TCHAR *name, int size );
	TCHAR rename[ MAX_PATH ];
	void  SetList( void );
	void  SaveList( void );

public:
	TCHAR m_target[ MAX_PATH ];

public:
	iFindDlg( HWND hParent );
	~iFindDlg();

private:
	BOOL OnInitDialog( HWND hwndFocus, LPARAM lParam );
	void OnCommand( int id, HWND hwndCtl, UINT codeNotify );

private:
	xComboBox*	mpCmbo;
	xFont*		  mpFont;

};
// ---------------------------------------------------------------------------


#endif//_IFINDDLG_H_
