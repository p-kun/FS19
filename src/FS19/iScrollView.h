/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iScrollView.h
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
#ifndef _ISCROLLVIEW_H_
#define _ISCROLLVIEW_H_

// SendCommandのIDのベース
//#define ISCROLL_TIMERID 0x1124

// ==========================================================================
// -- 
// --------------------------------------------------------------------------
class iScrollView : public iBaseView
{
public:
  iScrollView( int base = ISCROLL_BASE_ID );
  ~iScrollView();

public:
  void  Draw( HWND hWnd, HDC hDC, int x, int y, int cx, int cy );
  void  Redraw( void );
  void  OnMouseMove( int x, int y );
  void  OnMouseLeave( void );
  void  OnLButtonDown( int x, int y );
  void  OnLButtonUp( int x, int y );
  void  OnTimer( UINT id );
  void  SetScrollInfo( SCROLLINFO *si, BOOL redraw );
  void  GetScrollInfo( SCROLLINFO *si );

private:
  int   SetMiRect( int cx, int cy );
  void  DrawMain( HDC hDC );
  int   GetToolActionID( int x, int y );

private:
  SCROLLINFO  m_si;
  int     m_action;
  int     m_offset;
  int     m_position;
  int     m_prev_id;
  int     m_pos;
};
// --------------------------------------------------------------------------

#endif//_ISCROLLVIEW_H_
