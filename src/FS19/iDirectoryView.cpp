/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iDirectoryView.cpp
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
#include "_box.h"
#include "dirent.h"
#include "iIcon.h"
#include "xDcBase.h"
#include "iBaseView.h"
#include "iPt.h"
#include "iDirectoryView.h"

/* 定義値一覧です */

#define ICON_SIZE       16
#define HENABLE         ((void*)-3)
#define OFFSET_TOP      4
#define OFFSET_BTM      4
#define OFFSET_OFFSET   3
#define OFFSET_INTERVAL 6

/* enum */

enum {
  BACK_ARROW,
  BREAK_ARROW,
  BREAK_LINE,
  UP_FOLDER,
  POP_FOLDER,
  PUSH_FOLDER,
  NONE_LINE,
};

// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iDirectoryView::iDirectoryView( int base ) : iBaseView( base )
{
  Update();

  ::GetCurrentDirectory( MAX_PATH, m_directory );

  m_prev_id     = -1;
  m_action      = -1;
  m_enable      = FALSE;
  m_d_cnt       = 0;
  mIsTop        = TRUE;
  m_step_enable = TRUE;
  m_back_enable = TRUE;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iDirectoryView::Draw( HWND hWnd, HDC hDC, int x, int y, int cx, int cy )
{
  xTmpDC  dc( hDC );
  xBmpDC  bmp;
  iPt     pt;
  RECT   *rc;

  bmp.Create( dc, cx, cy );
  bmp.SetSelectFont( m_fontname, m_fontsize );

  SetMiRect( bmp, bmp.tm.tmAveCharWidth, cx, cy );

  bmp.FillRect( 0, 0, cx, cy, pt.COL_BASE_BAR );

  bmp.Line( cx - 2, 0, cx - 2, cy, pt.COL_SPLIT_LINE );

  bmp.SetBkMode( TRANSPARENT );

  if ( m_enable )
    {
      if ( m_focus )
        {
          bmp.SetTextColor( 0 );
        }
      else
        {
          bmp.SetTextColor( Brend( 0, 50 ) );
        }

      rc = &mi_lines[6].rc;

      bmp.Rectangle(rc->left,
                    0,
                    cx - rc->left - 8 - 72,
                    cy,
                    pt.COL_SPLIT_LINE,
                    pt.COL_BASE_BAR);

      DrawMain( bmp, m_prev_id, cx, cy, m_focus );
    }
  else
    {
      DrawMain2( bmp, cy );
    }

  dc.BitBlt( x, y, cx, cy, bmp, 0, 0, SRCCOPY );

  mhWnd = hWnd;
  mhDC  = bmp;
  m_x   = x;
  m_y   = y;
  m_cx  = cx;
  m_cy  = cy;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iDirectoryView::Redraw( void )
{
  if ( mhDC )
    {
      xDC   dc( mhWnd );

      Draw( mhWnd, dc, m_x, m_y, m_cx, m_cy );
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iDirectoryView::DrawMain( HDC hDC, int id, int width, int height, BOOL active )
{
  int     i;
  int     y;
  int     offset;
  xTmpDC  dc( hDC );
  iPt     pt;
  BOOL    enable = active;
  iico    ico(dc, IMG_DIRVIEW);

  for ( i = 0; i < TB_MAX_LINE && mi_lines[ i ].handle; i++ )
    {
      y = 1;

      if ( mi_lines[ i ].id == IDIRECTORYID_NULL )
        {
          offset = 0;
        }
      else
        {
          if ( mi_lines[ i ].id == IDIRECTORYID_BACK && !m_back_enable )
            {
            }
          else if ( ( mi_lines[ i ].id == IDIRECTORYID_STEP || mi_lines[ i ].id == IDIRECTORYID_DOWN ) && !m_step_enable )
            {
            }
          else if ( mi_lines[ i ].id == IDIRECTORYID_L_FOLDER && !m_up_enable )
            {
            }
          else if ( ( m_action == -1 && id == i ) || m_action == i )
            {
              FillRect( dc, &mi_lines[ i ].rc, pt.COL_SELECTED, pt.COL_SELECTED_WAKU );

              if ( m_action == i )
                {
                  y = 3;
                }
            }

          offset = OFFSET_OFFSET;
        }

      if ( mi_lines[ i ].handle != HENABLE )
        {
          if ( mi_lines[ i ].id == IDIRECTORYID_BACK )
            {
              enable = m_back_enable;
            }
          else if ( mi_lines[ i ].id == IDIRECTORYID_STEP || mi_lines[ i ].id == IDIRECTORYID_DOWN )
            {
              enable = m_step_enable;
            }
          else if ( mi_lines[ i ].id == IDIRECTORYID_L_FOLDER )
            {
              enable = m_up_enable;
            }
          else
            {
              enable = active;
            }
          ico.draw(mi_lines[ i ].rc.left + offset, ( height - ICON_SIZE ) / 2 + y, mi_lines[ i ].index, enable);
          offset += ( ICON_SIZE + OFFSET_INTERVAL );
        }

      if ( mi_lines[ i ].name[ 0 ] )
        {
          dc.TextOut( mi_lines[ i ].rc.left + offset, ( height - dc.tm.tmHeight ) / 2 + y, mi_lines[ i ].name );
        }
    }

  ico.draw(width - 24, (height - ICON_SIZE) / 2 + y, 7, active);
  ico.draw(width - 48, (height - ICON_SIZE) / 2 + y, 8, active);
  ico.draw(width - 72, (height - ICON_SIZE) / 2 + y, 9, active);
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iDirectoryView::DrawMain2( HDC hDC, int height )
{
  xTmpDC  dc( hDC );
  int     i;
  int     x;
  int     y  = ( height - dc.tm.tmHeight ) / 2;
  int     yd = ( height - ICON_SIZE) / 2;
  iico    ico(hDC, IMG_DIRVIEW);

  dc.SetTextColor( Brend( 0, 50 ) );

  for ( i = 6; i < TB_MAX_LINE && mi_lines[ i ].handle; i++ )
    {
      x = mi_lines[ i ].rc.left - mi_lines[ 6 ].rc.left + OFFSET_INTERVAL;

      if ( mi_lines[ i ].handle == HENABLE )
        {
          dc.TextOut( x, y, mi_lines[ i ].name );
        }
      else
        {
          if ( mi_lines[ i ].index == BREAK_LINE )
            {
              ico.draw(x, yd, BREAK_LINE, ILD_BLEND50);
            }
          else
            {
              ico.draw(x, yd, BREAK_ARROW, ILD_BLEND50);
            }
        }
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iDirectoryView::~iDirectoryView()
{
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iDirectoryView::SetMiiSubRect(HDC          hDC,
                                  int          idx,
                                  int          x,
                                  int          cy,
                                  WORD         id,
                                  int          offset,
                                  const TCHAR *str )
{
  SIZE        size      = {0, 0};
  int         icon_size = 0;
  ICONICON2  *lines;

  lines = &mi_lines[idx];

  if ( str )
    {
      _tcscpy_s( lines->name, MAX_PATH, str );
      ::GetTextExtentPoint32( hDC, str, ( int )_tcslen( str ), &size );
    }

  if ( lines->handle != HENABLE )
    {
      icon_size = ICON_SIZE;

      if ( size.cx )
        {
          icon_size += OFFSET_INTERVAL;
        }
    }

  lines->rc.left   = x;
  lines->rc.top    = OFFSET_TOP;
  lines->rc.right  = x + offset + icon_size + size.cx + offset;
  lines->rc.bottom = cy - OFFSET_BTM;
  lines->id        = id;

  return lines->rc.right;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iDirectoryView::SetMiRect( HDC hDC, int width, int cx, int cy )
{
  SHFILEINFO  shfi;
  TCHAR      *tmp;
  TCHAR*      cp;
  int         x = 8;
  int         i;
  int         j;
  int         len;
  int         top;
  TCHAR*      context;
  iico        ico(hDC, IMG_DIRVIEW);

  cx -= 72;

  len = (int)_tcslen(m_directory) + 1;

  tmp = (TCHAR *)malloc(len * sizeof(TCHAR));

  _tcscpy_s( tmp, len, m_directory );

  for (i = 0; i < TB_MAX_LINE; i++)
    {
      mi_lines[ i ].handle = ico;
    }

  mi_lines[ 0 ].index = PUSH_FOLDER;
  mi_lines[ 1 ].index = NONE_LINE;
  mi_lines[ 2 ].index = POP_FOLDER;
  mi_lines[ 3 ].index = BREAK_LINE;
  mi_lines[ 4 ].index = UP_FOLDER;
  mi_lines[ 5 ].index = NONE_LINE;

  x = SetMiiSubRect( hDC, 0, x, cy, IDIRECTORYID_BACK,     OFFSET_OFFSET );
  x = SetMiiSubRect( hDC, 1, x, cy, IDIRECTORYID_NULL );
  x = SetMiiSubRect( hDC, 2, x, cy, IDIRECTORYID_STEP,     OFFSET_OFFSET );
  x = SetMiiSubRect( hDC, 3, x, cy, IDIRECTORYID_DOWN );
  x = SetMiiSubRect( hDC, 4, x, cy, IDIRECTORYID_L_FOLDER, OFFSET_OFFSET );
  x = SetMiiSubRect( hDC, 5, x, cy, IDIRECTORYID_NULL );

  _tcscpy_s( mi_lines[ 4 ].folder, MAX_PATH, _T( ".." ) );

  top = 6;

  for ( i = top, cp = _tcstok_s( tmp, L"\\", &context ); cp; cp = _tcstok_s( NULL, L"\\", &context ) )
    {
      if ( i == top )
        {
          mi_lines[ i ].index  = NONE_LINE;
        }
      else
        {
          mi_lines[ i ].index  = BREAK_ARROW;
        }

      x = SetMiiSubRect( hDC, i, x, cy, IDIRECTORYID_NULL );
      i++;

      mi_lines[ i ].handle = HENABLE;

      if ( i == top + 1 )
        {
          if ( PathIsUNC( m_directory ) )
            {
              _stprintf_s( mi_lines[ i ].folder, MAX_PATH, _T( "\\\\%s" ), cp );
              cp = mi_lines[ i ].folder;
            }
          else
            {
              _tcscpy_s( mi_lines[ i ].folder, MAX_PATH, cp );

              PathAddBackslash( mi_lines[ i ].folder );
              ::SHGetFileInfo( mi_lines[ i ].folder, FILE_ATTRIBUTE_ARCHIVE, &shfi, sizeof( SHFILEINFO ), SHGFI_DISPLAYNAME );
              cp = shfi.szDisplayName;
            }
        }
      else
        {
          _tcscpy_s( mi_lines[ i ].folder, MAX_PATH, mi_lines[ i - 2 ].folder );
          ::PathAppend( mi_lines[ i ].folder, cp );
        }

      x = SetMiiSubRect( hDC, i, x, cy, IDIRECTORYID_L_FOLDER, OFFSET_OFFSET, cp );
      i++;
    }

  if ( m_d_cnt )
    {
      mi_lines[ i ].index  = BREAK_ARROW;
    }
  else
    {
      mi_lines[ i ].index  = BREAK_LINE;
    }

  x = SetMiiSubRect( hDC, i, x, cy, IDIRECTORYID_NULL );
  i++;

  for ( ; i < TB_MAX_LINE - 1; i++ )
    {
      mi_lines[ i ].handle = NULL;
    }

  if ( m_enable )
    {
      for ( i = top + 1, len = 0; i < TB_MAX_LINE - 1; i += 2 )
        {
          if ( mi_lines[ i + 2 ].handle == NULL || cx > ( x - len ) )
            {
              break;
            }

          len = mi_lines[ i ].rc.right - mi_lines[ top ].rc.left;
        }

      if ( len > 0 )
        {
          for ( i--, j = 0; i < TB_MAX_LINE; i++, j++ )
            {
              mi_lines[ j + top ] = mi_lines[ i ];

              if ( mi_lines[ j + top ].handle != NULL )
                {
                  mi_lines[ j + top ].rc.left  -= len;
                  mi_lines[ j + top ].rc.right -= len;
                }
            }

          mi_lines[ top ].index  = BACK_ARROW;
        }
    }
  else
    {
      for ( i = top + 1, len = 0; i < TB_MAX_LINE - 1; i += 2 )
        {
          if ( mi_lines[ i + 2 ].handle == NULL || cx > ( x - len - mi_lines[ top ].rc.right ) )
            {
              break;
            }

          len = mi_lines[ i ].rc.right - mi_lines[ top ].rc.left;
        }

      for ( i--, j = 0; i < TB_MAX_LINE; i++, j++ )
        {
          mi_lines[ j + top ] = mi_lines[ i ];

          if ( mi_lines[ j + top ].handle != NULL )
            {
              mi_lines[ j + top ].rc.left  -= len;
              mi_lines[ j + top ].rc.right -= len;
            }
        }
    }

  free(tmp);

  return 1;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iDirectoryView::GetToolActionID( int x, int y )
{
  int   i;
  int   target = -1;
  POINT pos;

  pos.x = x - m_x;
  pos.y = y - m_y;

  for ( i = 0; i < TB_MAX_LINE; i++ )
    {
      if ( mi_lines[ i ].handle == NULL )
        {
          continue;
        }

      if ( PtInRect( &mi_lines[ i ].rc, pos ) )
        {
          target = i;
          break;
        }
    }

  return target;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iDirectoryView::OnMouseMove( int x, int y )
{
  int   id = GetToolActionID( x, y );

  if ( id != m_prev_id )
    {
      m_prev_id = id;
      Redraw();
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iDirectoryView::OnMouseLeave( void )
{
  m_prev_id = -1;
  Redraw();
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iDirectoryView::OnLButtonDown( int x, int y )
{
  m_action = GetToolActionID( x, y );
  Redraw();
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iDirectoryView::OnRButtonDown( int x, int y )
{
  int   action = GetToolActionID( x, y );

  if ( action > 2 )
    {
      m_action = action;
      Redraw();
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iDirectoryView::OnLButtonUp( int x, int y )
{
  if ( m_action == GetToolActionID( x, y ) && m_action >= 0 )
    {
      SendID( mi_lines[ m_action ].id );
    }

  m_action = -1;
  Redraw();
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iDirectoryView::OnRButtonUp( int x, int y )
{
  if ( m_action == GetToolActionID( x, y ) && m_action >= 0 )
    {
      SendID( mi_lines[ m_action ].id | IDIRECTORY_R_MASK );
    }

  m_action = -1;
  Redraw();
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iDirectoryView::Update( void )
{
  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
TCHAR *iDirectoryView::GetSelected( void )
{
  int   action = 0;

  if ( m_action >= 0 && m_action < TB_MAX_LINE )
    {
      action = m_action;
    }

  return mi_lines[ action ].folder;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iDirectoryView::SetDirectory1( const TCHAR *dir, int d_cnt, BOOL redraw )
{
  _tcscpy_s( m_directory, MAX_PATH, dir );

  m_d_cnt = d_cnt;

  if ( redraw )
    {
      Redraw();
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iDirectoryView::SetEnable( BOOL enable, BOOL redraw )
{
  m_enable = enable;
  if ( redraw )
    {
      Redraw();
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iDirectoryView::GetHeight( HWND hWnd )
{
  xDC   dc( hWnd );

  return GetHeight( dc );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iDirectoryView::GetHeight( HDC hDC )
{
  xTmpDC  dc( hDC );
  int     height;

  dc.SetSelectFont( m_fontname, m_fontsize );

  if ( m_enable )
    {
      if ( dc.tm.tmHeight > ICON_SIZE )
        {
          height = OFFSET_TOP + OFFSET_OFFSET + dc.tm.tmHeight + OFFSET_OFFSET + OFFSET_BTM;
        }
      else
        {
          height = OFFSET_TOP + OFFSET_OFFSET + ICON_SIZE + OFFSET_OFFSET + OFFSET_BTM;
        }
    }
  else
    {
      height = OFFSET_TOP + dc.tm.tmHeight + OFFSET_BTM;
    }

  return height;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
TCHAR *iDirectoryView::GetSelected2( void )
{
  return ( mi_lines[ m_action + 1 ].handle != NULL ) ? mi_lines[ m_action + 1 ].folder : NULL;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iDirectoryView::OnMouseHover( void )
{
  m_prev_id = -1;
  Redraw();
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iDirectoryView::SetTop( BOOL isTop )
{
  mIsTop = isTop;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iDirectoryView::SetStepBackAndUpFocus( BOOL step_enable,  BOOL back_enable,  BOOL up_enable )
{
  m_back_enable = back_enable;
  m_step_enable = step_enable;
  m_up_enable   = up_enable;
}
// --------------------------------------------------------------------------
