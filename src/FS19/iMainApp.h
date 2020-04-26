/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iMainApp.h
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
#ifndef _MAINAPP_H_
#define _MAINAPP_H_


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
class iMainApp : public iSubApp
{
	// --
	typedef int ( iMainApp::*PT_FUNC_TYPE )( TCHAR *cp );
	
protected:
	void  OnCommand( int id, HWND hwndCtl, UINT codeNotify );
	BOOL  OnCreate( LPCREATESTRUCT lpCreateStruct );
	void  OnShowWindow( BOOL fShow, UINT status );
	void  DoContextMenu( int x, int y );
	void  DoContextMenu( int x, int y, const TCHAR *target );
	void  OnInitMenuPopup( HMENU hMenu, UINT item, BOOL fSystemMenu );
	void  OnDrawItem( const DRAWITEMSTRUCT * lpDrawItem );
	void  OnMeasureItem( MEASUREITEMSTRUCT * lpMeasureItem );
	void  OnDestroy();
	void  OnKeyDown( UINT vk, BOOL fDown, int cRepeat, UINT flags );
	void  OnDropFiles( HDROP hDrop );
	int   ReadClip();
	void  LetsCopy( TCOPYACTION *act, int id );

private:
	TCHAR* GetDesktopFolder( void );

public:
	int   Dummy( TCHAR *param = NULL ) { return 0; }
	int   FuncEditor( TCHAR *param = NULL );
	int   FuncLogDsk( TCHAR *param = NULL );
	int   FuncRefresh( TCHAR *param = NULL );
	int   FuncExec( TCHAR *param = NULL );
	int   FuncBack( TCHAR *param = NULL );
	int   FuncStep( TCHAR *param = NULL );
	int   FuncEject( TCHAR *param = NULL );
	int   FuncCopy( TCHAR *param = NULL );
	int   FuncPast( TCHAR *param = NULL );
	int   FuncRename( TCHAR *param = NULL );
	int   FuncOption( TCHAR *param = NULL );
	int   FuncSelExe( TCHAR *param = NULL );
	int   FuncDelete( TCHAR *param = NULL );
	int   FuncInsert( TCHAR *param = NULL );
	int   FuncApps( TCHAR *param = NULL );
	int   FuncProperties( TCHAR *param = NULL );
	int   FuncAttribute( TCHAR *param = NULL );
	int   FuncShortcut1( TCHAR *param = NULL );
	int   FuncShortcut2( TCHAR *param = NULL );
	int   FuncShortcut3( TCHAR *param = NULL );
	int   FuncShortcut4( TCHAR *param = NULL );
	int   FuncSetCopyView( TCHAR *param = NULL );
	int   FuncFind( TCHAR *param = NULL );
  int   FuncSort( TCHAR *param = NULL );

private:
	PT_FUNC_TYPE*	mpFuncs;
	TCHAR			m_desktopfolder[ MAX_PATH ];

public:
	iMainApp();
	~iMainApp();

public:
	UINT	mOsMessageMode;
	HWND	mhNextClipWnd;
};
// --------------------------------------------------------------------------


#endif//_MAINAPP_H_
