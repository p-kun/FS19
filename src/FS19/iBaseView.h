/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iBaseView.h
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
#ifndef _IBASEVIEW_H_
#define _IBASEVIEW_H_

// --
enum COMMAND_BASE_ID_T {
	//
	IDIRECTORY_BASE_ID	= 0x1780,
	ISCROLL_BASE_ID,
	ISHORTCUT_BASE_ID,
	ISEACHVIEW_BASE_ID,
	INAMELIST_BASE_ID,
	IEASYCOMMON_BASE_ID,
	ISTATUS_BASE_ID,
	IMAINVIEW_BASE_ID,
};


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
class iBaseView
{
public:
	iBaseView( int base = 0 );
	virtual ~iBaseView();

protected:
	typedef struct _ICONICON2_T_ {
		//
		void*   handle;
		int		  index;
		TCHAR	  folder[ MAX_PATH ];
		TCHAR	  name[ MAX_PATH ];
		WORD	  id;
		RECT	  rc;
	}
	ICONICON2;

	enum {
		TB_MAX_LINE = 50,
	};

public:
	void	    Create( HWND hWnd );
	void	    SetFont( const TCHAR *fname, int size );
	void	    DrawIcon( HDC hDC, ICONICON2 *info, int x = 0, int y = 0 );
	void	    DrawIcon( HDC hDC, HANDLE handle, int idx, int x, int y, int size = 16 );
	void	    ShellExecute( const TCHAR *pPath );
	BOOL	    ThePtInRect( int x, int y );
	TCHAR* 	  LastError( void );
	void	    SetFocus( BOOL enable = TRUE ) { m_focus = enable; }

public:
	virtual   void   OnMouseMove( int x, int y );
	virtual   void   OnMouseLeave( void );
	virtual   void   OnLButtonDown( int x, int y );
	virtual   void   OnLButtonUp( int x, int y );
	virtual   void   OnKeyDown( UINT vk, BOOL fDown, int cRepeat, UINT flags );
	virtual   TCHAR* GetSelected( void ) { return NULL; }
	virtual   TCHAR* GetSelected2( void ) { return NULL; }
	virtual   TCHAR* GetSelected3( void ) { return NULL; }
	virtual   void Draw( HWND hWnd, HDC hDC, int x, int y, int cx, int cy ) {}

protected:
	void	    FillRect( HDC hDC, const RECT *pRC, COLORREF fg );
	void	    FillRect( HDC hDC, int x, int y, int cx, int cy, COLORREF fg );
	void	    FillRect( HDC hDC, const RECT *pRC, COLORREF fg, COLORREF waku );
	void	    FillRect( HDC hDC, int x, int y, int cx, int cy, COLORREF fg, COLORREF waku );
	COLORREF  Brend( COLORREF col, int alph );
	void	    ExtTextOut2( HDC hDC, int x, int y, const TCHAR *str, int cnt, int *lens, COLORREF *cols, int *points, int str_offset = 0 );
	BOOL	    CheckRect( int xx, int yy, int cxx, int cyy, int *x, int *y, int *cx, int *cy );
	void	    SendID( int id );
	TCHAR*    RemoveFileSpec( TCHAR *buf, const TCHAR *name );
	TCHAR*    GetLastErrString( DWORD err );

protected:
	ICONICON2	mi_lines[ TB_MAX_LINE + 1 ];
	int			  m_baseid;
	TCHAR		  m_fontname[ MAX_PATH ];
	int			  m_fontsize;
	//
	HWND		  mhWnd;
	HDC			  mhDC;
	//
	int			  m_x;
	int			  m_y;
	int			  m_cx;
	int			  m_cy;
	BOOL		  m_focus;

};
// --------------------------------------------------------------------------


#endif//_IBASEVIEW_H_
