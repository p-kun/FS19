/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iInfoView.h
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
#ifndef _IINFOVIEW_H_
#define _IINFOVIEW_H_


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
class iInfoView : public iBaseView
{
public:
	iInfoView( int base = ISHORTCUT_BASE_ID );
	~iInfoView();

public:
	void	Draw( HWND hWnd, HDC hDC, int x, int y, int cx, int cy );
	void	Redraw( void );
	void	TargetSet( dirent *entry, BOOL redraw = TRUE );
	int		GetHeight( HWND hWnd );
	int		GetHeight( HDC hDC );
	void	EndCopyInfo( void );
	void	Refresh( void );
	void	OnMouseMove( int x, int y );
	void	OnMouseLeave( void );
	void	OnLButtonDown( int x, int y );
	void	OnLButtonUp( int x, int y );
	void	OnTimer( UINT id );
	void	OnMouseHover( void );

private:
	void	  SetMiRect( HDC hDC, int width, int cx, int cy );
	int		  SetMiiSubRect( HDC hDC, ICONICON2 *lines, int x, int cy, WORD id, int offset = 0, const TCHAR *str = NULL );
	void	  DrawMainA( HDC hDC, int cx, int cy, int w, int h );
	void	  DrawMainB( HDC hDC, int cx, int cy, int w, int h );
	void	  MakeFileInfo( dirent *entry );
	void	  ChangeDrive( TCHAR *drv );
	TCHAR*	SizeToStr( ULONGLONG size, TCHAR *pBuf );
	TCHAR*	SizeToStr( ULONGLONG capa, ULONGLONG used, TCHAR *pBuf );
  HANDLE  GetFileIconAndType( const TCHAR *target, TCHAR *type_name );
	void	  FillRect( HDC hDC, const RECT *pRC, COLORREF fg, int rate );
	void	  DrawCapaInfo( HDC hDC, int x, int h, int cx, int cy );
	void	  FillRect( HDC hDC, int x, int y, int cx, int cy, COLORREF fg, int rate );
	int		  GetToolActionID( int x, int y );
	int		  SetElement( HDC hDC, int no, int x, int height, WORD id, const TCHAR *str = NULL );

private:
	enum {
		MAX_LEN  = 100,
		MAX_LEN2 = 50,
	};

private:
	dirent	m_entry;
	int		  m_mode;
	TCHAR	  m_target[ MAX_PATH ];
	TCHAR	  m_info[ MAX_LEN ];
	TCHAR	  m_capa[ MAX_LEN2 ];
	TCHAR	  m_used[ MAX_LEN2 ];
	TCHAR	  m_free[ MAX_LEN2 ];
	TCHAR	  m_usedcapa[ MAX_LEN2 ];

	HANDLE	m_drive_handle;
	int		  m_drive_index;
	TCHAR	  m_drive_name[ 80 ];
	int		  m_drive_rate;
	int		  m_action;
	int		  m_prev_id;
	int		  m_timer_on;
	BOOL	  mCopyEnable;
	BOOL	  mPastEnable;
	
};
// --------------------------------------------------------------------------


#endif//_IINFOVIEW_H_
