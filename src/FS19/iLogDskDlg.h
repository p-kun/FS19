/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iLogDskDlg.h
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
#ifndef _ILOGDSKDLG_H_
#define _ILOGDSKDLG_H_


class iComList;
class iComEdit;
class iFont;

#define MAX_PATH2	(MAX_PATH *2)

// ==========================================================================
// -- iLogDskDlg
// --------------------------------------------------------------------------
class iLogDskDlg : public xDialog
{
public:
	void	SetCurrent( const TCHAR *name );
	TCHAR	logdrv[ MAX_PATH2 ];

public:
	iLogDskDlg( HWND hParent );
	~iLogDskDlg();

private:
	BOOL OnInitDialog( HWND hwndFocus, LPARAM lParam );
	void OnCommand( int id, HWND hwndCtl, UINT codeNotify );
	void OnPaint( void );
	void SetList( void );

private:
	iComList*	mpLogList;
	iComEdit*	mpEdit;
	xFont*		mpFont;

};
/*==========================================================================*/


#endif//_ILOGDSKDLG_H_
