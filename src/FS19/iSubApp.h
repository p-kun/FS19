/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iSubApp.h
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
#ifndef _SUBAPP_H_
#define _SUBAPP_H_

enum {
  IRC_SCROLL   = 0,
  IRC_SPLIT_LU,
  IRC_INFO,
  IRC_SHORTCUT,
  IRC_DIR1,
  IRC_DIR2,
  IRC_SEACH1,
  IRC_SEACH2,
  IRC_SHORTCUT2,
  IRC_STATUS,
  IRC_MAX,
};

#define TITLE_HEIGHT  0

/* このクラスで使用するクラスさんたち... */

class iBaseView;
class iShortcutView;
class iScrollView;
class iSeachView;
class iInfoView;
class iDirectoryView;
class iEasyCommonInfo;
class iTreeView;
class iStATUsViEW;
class iMainView;

// ==========================================================================
// -- 
// --------------------------------------------------------------------------
class iSubApp : public _boxApp
{
protected:
  void    OnMouseWheel( int x, int y, int zDelta, UINT fwKeys );
  void    OnVScroll( HWND hwndCtl, UINT code, int pos );
  void    PresetWindowClass( WNDCLASSEX *pWC );
  void    OnSize( UINT state, int cx, int cy );
  void    OnSizing( UINT nSide, LPRECT lpRect );
  void    OnDrawItem( const DRAWITEMSTRUCT * lpDrawItem );

protected:
  void    SetFonts( const TCHAR *fname1, int size1,
                    const TCHAR *fname2, int size2,
                    const TCHAR *fname3, int size3,
                    const TCHAR *fname4, int size4 );
  int     GetLogicalDrives( void );
  TCHAR*  GetSelected( void );
  void    SetColumn( int column );
  void    SetDirectory( const TCHAR *dir = NULL, const TCHAR *prev_dir = NULL, int on_msg = 0 );
  BOOL    Draw( HWND hWnd, HDC hDC );
  BOOL    Draw2( HWND hWnd, HDC hDC );
  void    SetDirectorys( const TCHAR *dir1 = NULL, const TCHAR *dir2 = NULL );
  BOOL    LetsCommand( int action, int id, LPARAM lParam );
  BOOL    LetsKeyDown( WPARAM wParam, LPARAM lParam );
  BOOL    LetsLButtonDown( int x, int y );
  BOOL    LetsRButtonDown( int x, int y );
  BOOL    LetsTimer( UINT id );
  dirent* GetCurrentEntry( void );
  int     SetMiRect( HDC hDC, RECT *p_rc, int type = 0 );
  void    GetCurrentDirectorys( TCHAR *path1, TCHAR *parh2, int max );
  TCHAR*  GetCurrentDirectory( void );
  void    Back( void );
  void    Step( void );
  BOOL    WinExec( TCHAR *pCmdline, const TCHAR *pPath = NULL );
  void    GetCurrentPosition( int *x, int *y );
  int     GetMarkedNum( dirent** entrylist );
  void    SetTitle();
  void    StopNotification();
  void    StartNotification();
  BOOL    PtInActiveSeach( int x, int y );
  void    SetCopyInfo( TCOPYACTION *act );
  void    EndCopyInfo( void );
  void    StartCopyInfo( void );
  void    FindDirentMark( const TCHAR *name );
  void    MoveNextMark( int current = -1 );
  void    CompCopyInfo( TCOPYACTION *act );
  void    CancelCopyInfo( void );
  void    SetCopyView( BOOL redraw = TRUE );
  void    SetSeachView( BOOL redraw = TRUE );
  int     GetTotalCopyFile( void );
  void    SelectSeachMode( const TCHAR *cardlist );
  int     GetCurrentPageNo( void );
  void    Refresh(  TCHAR *dir = NULL, dirent *namelist = NULL );
  int     GetMarkedNumTop( dirent** entrylist );
  int     GetMarkedNumBottom( dirent** entrylist );
  void    Sort( void );

private:
  virtual LRESULT Message( UINT msg, WPARAM wParam, LPARAM lParam );

private:
  void    SetScrollPos( BOOL redraw = TRUE );
  int     thePtInRect( int x, int y );
  void    Idle();
  void    Redraw( void );
  void    SetStatusView( BOOL redraw = TRUE );

public:
  iSubApp();
  ~iSubApp();

protected:
  iTreeView*        mp_tree_view;
  iStATUsViEW*      mp_status_view;
  iShortcutView*    mp_shortcut_view;
  iDirectoryView*   mp_dir_view1;
  iDirectoryView*   mp_dir_view2;
  iScrollView*      mp_scroll_view;
  iSeachView*       mp_seach_view1;
  iSeachView*       mp_seach_view2;
  iInfoView*        mp_info_view;
  iSeachView*       mp_act_seach;
  iSeachView*       mp_act_seach1;
  iSeachView*       mp_act_seach2;
  iDirectoryView*   mp_act_dir;
  iDirectoryView*   mp_act_dir1;
  iDirectoryView*   mp_act_dir2;
  iEasyCommonInfo*  mp_com_info;
  iBaseView*        mp_bdown_view;
  RECT              m_rcs[ IRC_MAX ];
  BOOL              m_lock;
  int               m_focus;
  BOOL              m_tree_mode;
  HICON             m_cursor_wait_icon;
  BOOL              m_splt_on;
  int               m_tool_size;
  int               m_cx;
  xComboBox*        mpCheckBoxs;
  TCHAR             mTitleBuf[ MAX_PATH ];
  BOOL              mIsERASEBKGND;
};
// --------------------------------------------------------------------------

#endif//_SUBAPP_H_
