/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iSettingDialog.cpp
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
#ifndef _ISETTINGDIALOG_H_
#define _ISETTINGDIALOG_H_


class xComboBox;
class xEdit;
// ==========================================================================
// -- 
// --------------------------------------------------------------------------
class iSettingDialog : public xDialog
{
public:
	BOOL	SetFontDialog( LOGFONT *font, int fixed = 0 );
	void	SetLogFont( int no, const TCHAR *fnam, int height );
	LOGFONT*
			GetLogFont( int no );
	void	SetParam( int column, int number, int type );
	void	GetParam( int *column, int *number, int *type );

public:
	iSettingDialog( HWND hParent );
	~iSettingDialog();

public:
	BOOL	OnInitDialog( HWND hwndFocus, LPARAM lParam );
	void	OnCommand( int id, HWND hwndCtl, UINT codeNotify );

protected:
	xComboBox*	mpCmbo1;
	xComboBox*	mpCmbo2;
	xComboBox*	mpCmbo3;
	xEdit*		  mpEdit1;
	xEdit*		  mpEdit2;
	xEdit*		  mpEdit3;
	xEdit*		  mpEdit4;

	TCHAR		    mFontName1[ MAX_PATH ];
	TCHAR		    mFontName2[ MAX_PATH ];
	TCHAR		    mFontName3[ MAX_PATH ];
	TCHAR		    mFontName4[ MAX_PATH ];
	int			    mFontSize1;
	int			    mFontSize2;
	int			    mFontSize3;
	int			    mFontSize4;

	LOGFONT		  mLogFont1;
	LOGFONT		  mLogFont2;
	LOGFONT		  mLogFont3;
	LOGFONT		  mLogFont4;

	int			    mColumn;
	int			    mNumber;
	int			    mType;

};
// --------------------------------------------------------------------------


#endif//_ISETTINGDIALOG_H_
