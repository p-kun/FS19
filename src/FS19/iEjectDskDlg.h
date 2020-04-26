/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iEjectDskDlg.h
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
#ifndef _IEJECTDSKDLG_H_
#define _IEJECTDSKDLG_H_


class iComList;
// ==========================================================================
// -- iEjectDskDlg
// --------------------------------------------------------------------------
class iEjectDskDlg : public xDialog
{
public:
	BOOL CheckEjectDrive( void );
	
public:
	iEjectDskDlg( HWND hParent );
	~iEjectDskDlg();
	
private:
	BOOL OnInitDialog( HWND hwndFocus, LPARAM lParam );
	void OnCommand( int id, HWND hwndCtl, UINT codeNotify );
	
private:
	BOOL DiscEject( TCHAR *drv );
	BOOL DiscLoad( int c );
	
private:
	iComList*	mpLogList;
	
};
/*==========================================================================*/


#endif//_IEJECTDSKDLG_H_
