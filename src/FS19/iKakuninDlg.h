/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iKakuninDlg.h
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
#ifndef _IKAKUNINDLG_H_
#define _IKAKUNINDLG_H_

/* Public type declarations */

enum
{
	IKAKU_NEW,
	IKAKU_RENAME,
	IKAKU_OVERWRITE,
	IKAKU_NOTCOPY,
	IKAKU_MAX,
};

class iComListEx;
class iComButton;
class xFont;

// ==========================================================================
// -- iKakuninDlg
// --------------------------------------------------------------------------
class iKakuninDlg : public xDialog
{
public:
	void	SetSourceFileInfo( const TCHAR *src, FILETIME *time, LARGE_INTEGER *size );
	void	SetTransFileInfo( const TCHAR *trans, FILETIME *time, LARGE_INTEGER *size );
	void	SetSourceFileInfo( const TCHAR *src, FILETIME *time, DWORD high, DWORD low );
	void	SetFiles( const TCHAR *src, const TCHAR *trans );
	int		selected;
	BOOL	thatsall;
	TCHAR	mNewPath[ MAX_PATH ];

public:
	iKakuninDlg( HWND hParent );
	~iKakuninDlg();

private:
	BOOL OnInitDialog( HWND hwndFocus, LPARAM lParam );
	void OnCommand( int id, HWND hwndCtl, UINT codeNotify );
	void OnPaint( void );

private:
	iComListEx*	mpLogList;
	xButton*		mpButton1;
	xButton*		mpButton2;
	xButton*		mpButton3;
	xFont		    mFont;
	int				  mHeight;
  int         mFraem_cy;
};
/*==========================================================================*/

#endif//_IKAKUNINDLG_H_
