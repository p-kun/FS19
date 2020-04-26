/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iShortcutView.h
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
#ifndef _ISHORTCUTVIEW_H_
#define _ISHORTCUTVIEW_H_


// SendCommandのIDのベース
enum _ISHORTCUT_E_ {
	ISHORTCUTID_NULL,
	ISHORTCUTID_PAST,
	ISHORTCUTID_COPY,
	ISHORTCUTID_DELETE,
	ISHORTCUTID_RENAME,
	ISHORTCUTID_REFRESH,
	ISHORTCUTID_PROPERTIES,
	ISHORTCUTID_EJECT,
	ISHORTCUTID_SETTING,
	ISHORTCUTID_L_FOLDER,
	ISHORTCUTID_R_FOLDER,
	ISHORTCUTID_BACK,
	ISHORTCUTID_STEP,
	ISHORTCUT_R_MASK,
};


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
class iShortcutView : public iBaseView
{
public:
	iShortcutView( int base = ISHORTCUT_BASE_ID );
	~iShortcutView();
	
	
public:
	void	Draw( HWND hWnd, HDC hDC, int x, int y, int cx, int cy );
	void	OnMouseMove( int x, int y );
	void	OnMouseLeave( void );
	void	OnLButtonDown( int x, int y );
	void	OnLButtonUp( int x, int y );
	void	OnRButtonDown( int x, int y );
	void	OnRButtonUp( int x, int y );
	int		Update( void );
	TCHAR*	GetSelected( void );
	
	
private:
	int		SetMiRect( int width, int cx, int cy );
	void	DrawMain( HDC hDC, int id, int cy, int height );
	int		GetToolActionID( int x, int y );
	int		GetSystemFolderInfo( int id, TCHAR *name, int index );
	int		GetLogicalDrives( int *index );
	void	Redraw( void );
	
	
private:
	int		m_max_len;
	int		m_points[ MAX_PATH ];
	int		m_action;
	int		m_prev_id;
	
};
// --------------------------------------------------------------------------


#endif//_ISHORTCUTVIEW_H_
