/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iSeachView.h
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
#ifndef _ISEACHVIEW_H_
#define _ISEACHVIEW_H_


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
class iSeachView : public iBaseView, public iNamelist
{
public:
  iSeachView( int base = ISEACHVIEW_BASE_ID );
  virtual ~iSeachView();

public:
  virtual void    Draw( HWND hWnd, HDC hDC, int x, int y, int cx, int cy );
  virtual void    Redraw( void );
  virtual void    SetColumn( int column );
  virtual void    OnMouseMove( int x, int y );
  virtual void    OnMouseLeave( void );
  virtual void    OnLButtonDown( int x, int y );
  virtual void    OnRButtonDown( int x, int y );
  virtual void    OnLButtonUp( int x, int y );
  virtual void    OnRButtonUp( int x, int y );
  virtual void    OnSize( UINT state, int cx, int cy );
  virtual void    OnSizing( UINT nSide, LPRECT lpRect );
  virtual BOOL    SetDirectory2( const TCHAR *dir = NULL, const TCHAR *d_name = NULL, int on_msg = 0 );
  virtual void    OnKeyDown( UINT vk, BOOL fDown, int cRepeat, UINT flags );
  virtual void    OnLButtonDblClk( int x, int y, UINT flags );
  virtual void    SetCurrentLine1( int no );
  virtual void    SetCurrentLine2( int no );
  virtual int     GetTotalPage( void );
  virtual int     GetCurrentPage( void );
  virtual dirent* GetCurrentEntry( void );
  virtual int     GetMarkedNum( dirent** entrylist = NULL );
  virtual int     GetMarkedInfo( int *d_cnt, int *f_cnt, ULARGE_INTEGER *f_size );
  virtual void    SetEnable( BOOL enable );
  virtual void    PageUp( void );
  virtual void    PageDown( void );
  virtual void    PagePos( int pos );
  virtual void    SetPositon( BOOL top ) { m_topposition = top; }
  virtual BOOL    KeyDown( int vk, int ctrl, int shilt, int alt );
  virtual void    GetCurrentPosition( int *x, int *y );
  virtual int     IsChangeFolderState( void );
  virtual void    StartNotification( void );
  virtual void    StopNotification( void );
  virtual TCHAR*  GetSelected( void );
  virtual void    StartFileSeach( void );
  virtual int     FileSeach( void );
  virtual dirent* FindDirent( const TCHAR *name );
  virtual void    MoveNextMark( int current = -1 );
  virtual void    DrawLine( HDC hDC, int no );

public:
  void            SetCopeingFile( TCHAR *path = NULL );
  virtual void    NamelistAction( void* );
  void            SetMarkFromNamelist( TCHAR *dir, dirent *namelist );
  void            OnTimer( UINT id );
  BOOL            PathIsRoot( void );
  void            Sort(void);
  void            ClearDateTime( TCHAR *d_name );
  void            ForceUpadteEnable( BOOL bUpdate = FALSE ) { mb_forceUpdate = bUpdate; }
  BOOL            KeyDown( int vk );
  BOOL            KeyDownSub( int vk, int ctrl, int shift, int alt );

public:
  void            SelectSeachMode(  BOOL force = FALSE );

protected:
  enum {
    COLUMN_MAX = 10,
    LINE_MAX   = 100,
    LEN_MAX    = 2024,
  };

  typedef struct _PAY_T_ {
    //
    int   x[ COLUMN_MAX + 1 ];
    int   y[ LINE_MAX + 1 ];
    int   w;
    int   h;
    int   len;
    int   lines;
    int   column;
    int   str_top;
    int   line_per_page;
    int   points[ LEN_MAX + 1 ];
    int   heights[ LINE_MAX + 1 ];
  }
  PAY;

protected:
  int   SetMiRect( int x, int y, int cx, int cy, int w, int h );
  void  DrawMain( HDC hDC );
  void  DrawForSeaching( HDC hDC, int x, int y, int cx, int cy );
  void  DrawForSeachingString( HDC hDC, int x, int y, int cx, int cy );
  int   GetToolActionID( int x, int y );
  void  SetCurrentLine( int *current, int no );
  void  SetCurrentLine( int no );
  void  Enter( BOOL dblclk = FALSE );
  BOOL  BackSpase( void );
  void  GotoRoot( void );
  void  BitBlt3( HDC hDes, HDC hSrc, int x, int y, int cx, int cy );
  void  FillRect3( HDC hDes, int x, int y, int cx, int cy, COLORREF col );
  void  FillRect3( HDC hDes, RECT *rc, COLORREF col );
  void  SetMark( int inc = 1 );
  int   AllMark( int mark_mode = 0, BOOL control = FALSE );
  BOOL  Scan( const TCHAR *dir, const TCHAR *prev_dir = NULL, int seach_mode = INAM_MODE_NORMAL, int on_msg = 0 );
  void  DoDrag( void );
  int   GetEnableAction( void );
  void  StretchDIBits( HDC hDC, int x, int y, int dispCx, int dispCy, BITMAPINFO *pBitmapInfo, void *pData = NULL );

protected:
  PAY   m_pay;
  //
  int   m_action;
  int   m_current1;
  int   m_current2;

 protected:
  BOOL   m_active;
  HANDLE mh_handle;

  struct dirent_ext : public dirent
  {
    TCHAR tmp[ MAX_PATH ];
  }
  m_tmp;

  BOOL   m_topposition;
  TCHAR  m_selected[ MAX_PATH ];
  int    m_seach_count;
  TCHAR mEmptyMessage[ MAX_PATH ];
  TCHAR  mSeachingFolderName[ MAX_PATH ];
  int    mSeachingIconState;

  BOOL   m_hold;
  int    m_hold_start;
  int    m_hold_end;
  BITMAPINFO*
         mp_bmpinfo;
  BOOL   m_seaching;
  BOOL   m_disp_recalc;
  int    m_disp_rate;
  int    m_disp_min_rate;
  BOOL   m_disp_hold;
  int    m_disp_hold_x;
  int    m_disp_hold_y;
  int    m_disp_offset_x;
  int    m_disp_offset_y;
  int    m_disp_x;
  int    m_disp_y;
  int    m_disp_cx;
  int    m_disp_cy;
  BOOL   mb_find;
  TCHAR* mp_lastUpdateFile;
  BOOL   mb_forceUpdate;
  RECT   m_monitor;
};
// --------------------------------------------------------------------------

#endif//_ISEACHVIEW_H_
