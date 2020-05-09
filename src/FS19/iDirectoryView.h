/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iDirectoryView.h
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
#ifndef _IDIRECTORYVIEW_H_
#define _IDIRECTORYVIEW_H_


/* SendCommandのIDのベース */

#define IDIRECTORYID_L_FOLDER 0x00
#define IDIRECTORYID_R_FOLDER (IDIRECTORYID_L_FOLDER | IDIRECTORY_R_MASK)
#define IDIRECTORYID_BACK     0x01
#define IDIRECTORYID_STEP     0x02
#define IDIRECTORYID_DOWN     0x03
#define IDIRECTORYID_NULL     0x0F
#define IDIRECTORY_R_MASK     0x80


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
class iDirectoryView : public iBaseView
{
public:
  iDirectoryView( int base = IDIRECTORY_BASE_ID );
  ~iDirectoryView();

public:
  void SetTop( BOOL isTop );
  void SetStepBackAndUpFocus(BOOL step_enable = TRUE,
                             BOOL back_enable = TRUE,
                             BOOL up_enable   = TRUE);

public:
  virtual void    Draw( HWND hWnd, HDC hDC, int x, int y, int cx, int cy );
  virtual void    Redraw( void );
  virtual void    OnMouseMove( int x, int y );
  virtual void    OnMouseLeave( void );
  virtual void    OnLButtonDown( int x, int y );
  virtual void    OnLButtonUp( int x, int y );
  virtual void    OnRButtonDown( int x, int y );
  virtual void    OnRButtonUp( int x, int y );
  virtual void    OnMouseHover( void );
  virtual int     Update( void );
  virtual TCHAR*  GetSelected( void );
  virtual TCHAR*  GetSelected2( void );
  virtual void    SetDirectory1( const TCHAR *dir, int d_cnt, BOOL redraw = TRUE );
  virtual void    SetEnable( BOOL enable, BOOL redraw = TRUE );
  virtual int     GetHeight( HWND hWnd );
  virtual int     GetHeight( HDC  hDC );

private:
  int   SetMiRect( HDC hDC, int width, int cx, int cy );
  int   SetMiiSubRect(HDC          hDC,
                      int          idx,
                      int          x,
                      int          cy,
                      WORD         id,
                      int          offset = 0,
                      const TCHAR *str    = NULL );
  void  DrawMain( HDC hDC, int id, int width, int height, BOOL active = TRUE );
  void  DrawMain2( HDC hDC, int width );
  int   GetToolActionID( int x, int y );

protected:
  int   m_action;
  TCHAR m_directory[ MAX_PATH ];
  int   m_prev_id;
  int   m_d_cnt;

protected:
  BOOL  m_enable;
  BOOL  mIsTop;
  BOOL  m_step_enable;
  BOOL  m_back_enable;
  BOOL  m_up_enable;

};
// --------------------------------------------------------------------------


#endif//_IDIRECTORYVIEW_H_
