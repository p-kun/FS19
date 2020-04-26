#ifndef _ISTATUS_H_
#define _ISTATUS_H_

#include <stdio.h>
#include <windows.h>
#include <locale.h>
#include <tchar.h>
#include <shlwapi.h>
#include <malloc.h>
#include "ssnprintf.h"
#include "logf.h"
#include "oneCopy.h"
#include "iIcon.h"
#include "ssnprintf.h"
#include "ipt.h"
#include "ContextMenu.h"


/* Private definitions */

#define MAX_STRING      256
#define WIN10_LINES     1


// ==========================================================================
// -- アプリケーションクラスの作成
// --------------------------------------------------------------------------
class iStATUsViEW : public oneCopy
{
public:
  // ======================================================================
  // -- クラスの構築
  // ----------------------------------------------------------------------
  iStATUsViEW()
  {
    m_string[ 0 ]          = L'\0';
    m_sub_string[ 0 ]      = L'\0';
    m_copy_string[ 0 ]     = L'\0';
    m_copy_sub_string[ 0 ] = L'\0';
    m_tick_count           = 0;
    m_cnt                  = 0;
    m_rate                 = -1;
    m_timer                = 0;
    m_icon_cnt             = 0;
    m_timer2               = 0;
  }
  // ----------------------------------------------------------------------


  // ======================================================================
  // -- クラスの破棄
  // ----------------------------------------------------------------------
  ~iStATUsViEW()
  {
  }
  // ----------------------------------------------------------------------


  // ======================================================================
  // -- 
  // ----------------------------------------------------------------------
  void Draw( HWND hWnd, HDC hDC, int x, int y, int cx, int cy )
  {
    xTmpDC  dc( hDC );
    xBmpDC  bmp;
    int     yyy;
    int     xxx;
    int     height;
    SIZE    size;
    iPt     pt;
    iico    cpy;
    iico    cal;

    mhWnd = hWnd;

    m_currc.top    = y;
    m_currc.left   = x;
    m_currc.right  = x + cx;
    m_currc.bottom = y + cy;

    bmp.Create( dc, cx, cy );
    bmp.SetSelectFont( pt.fontname4, pt.fontsize4 );
    bmp.FillRect( 0, 0, cx, cy, pt.COL_BASE_BAR );
#if not WIN10_LINES
    bmp.Line( 1, 0, cx - 1, 0, pt.COL_SPLIT_LINE );
    bmp.Line( cx - 2, 1, cx - 2, cy - 1, pt.COL_SPLIT_LINE );
#endif
    bmp.SetBkMode( TRANSPARENT );

    if ( m_enable )
      {
        bmp.SetTextColor( 0 );
      }
    else
      {
        bmp.SetTextColor( pt.COL_SPLIT_LINE );
      }

    height = GetHeight( bmp );

    yyy = ( height - bmp.tm.tmHeight ) / 2;
    xxx = 8;

    cpy.attach(bmp, IMG_COPYING);
    cal.attach(bmp, IMG_STATUS);

    if ( m_rate >= 0 )
      {
        bmp.FillRect( 0, 0, m_rate * cx / 10000, 3, pt.COL_PROGRESS_BAR );

        cpy.draw(xxx, ( height - 16 ) / 2, m_cnt % 16, m_enable );

        xxx += 16 + 4;
        bmp.ExtTextOut( xxx, yyy, m_copy_string, bmp.tm.tmAveCharWidth );
        GetTextExtentPoint32( bmp, m_copy_string, (int)_tcslen( m_copy_string ), &size );
        xxx += size.cx;

        cal.draw(xxx, ( height - 16 ) / 2, 4, m_enable );

        xxx += 16;
        bmp.ExtTextOut( xxx, yyy, m_string, bmp.tm.tmAveCharWidth );
        GetTextExtentPoint32( bmp, m_string, (int)_tcslen( m_string ), &size );
        xxx += size.cx;

        cal.draw(xxx, ( height - 16 ) / 2, 4, m_enable);

        xxx += 16;
        bmp.ExtTextOut( xxx, yyy, m_sub_string, bmp.tm.tmAveCharWidth );
      }
    else if ( m_rate == -1 )
      {
        cal.draw(xxx, ( height - 16 ) / 2, m_icon_cnt, m_enable);

        xxx += 16 + 4;
        bmp.ExtTextOut( xxx, yyy, m_string, bmp.tm.tmAveCharWidth );
        GetTextExtentPoint32( bmp, m_string, (int)_tcslen( m_string ), &size );
        xxx += size.cx;

        cal.draw(xxx, ( height - 16 ) / 2, 4, m_enable);

        xxx += 16;
        bmp.ExtTextOut( xxx, yyy, m_sub_string, bmp.tm.tmAveCharWidth );
      }
    else if ( m_rate == -2 )
      {
        bmp.FillRect( 0, 0, cx, 3, pt.COL_PROGRESS_BAR );

        cal.draw(xxx, ( height - 16 ) / 2, 5, m_enable);

        xxx += 16 + 4;
        bmp.ExtTextOut( xxx, yyy, m_copy_string, bmp.tm.tmAveCharWidth );

        if ( m_copy_sub_string[ 0 ] )
          {
            GetTextExtentPoint32( bmp, m_copy_string, (int)_tcslen( m_copy_string ), &size );
            xxx += size.cx;
            xxx += 16;

            cal.draw(xxx, ( height - 16 ) / 2, 6, m_enable);

            xxx += 16;
            bmp.ExtTextOut( xxx, yyy, m_copy_sub_string, bmp.tm.tmAveCharWidth );
          }
      }
    else if ( m_rate == -3 )
      {
        cal.draw(xxx, ( height - 16 ) / 2, 6, m_enable);

        xxx += 16 + 4;
        bmp.ExtTextOut( xxx, yyy, m_copy_string, bmp.tm.tmAveCharWidth );
      }
    else if ( m_rate == -4 )
      {
        cal.draw(xxx, ( height - 16 ) / 2, 7, m_enable);

        xxx += 16 + 4;
        bmp.ExtTextOut( xxx, yyy, m_copy_string, bmp.tm.tmAveCharWidth );

        if ( m_copy_sub_string[ 0 ] )
          {
            GetTextExtentPoint32( bmp, m_copy_string, (int)_tcslen( m_copy_string ), &size );
            xxx += size.cx;
            xxx += 16;

            cal.draw(xxx, ( height - 16 ) / 2, 7, m_enable);

            xxx += 16;
            bmp.ExtTextOut( xxx, yyy, m_copy_sub_string, bmp.tm.tmAveCharWidth );
          }
      }

    dc.BitBlt( x, y, cx, cy, bmp, 0, 0, SRCCOPY );

  }
  // ----------------------------------------------------------------------


  // ======================================================================
  // -- 
  // ----------------------------------------------------------------------
  BOOL KeyDown( int vk, int ctrl, int shift, int alt )
  {
    return TRUE;
  }
  // ----------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  BOOL SetFocus( BOOL enable )
  {
    m_enable = enable;

    return m_enable;
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void OnLButtonDown( int x, int y )
  {
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void OnLButtonDblClk( int x, int y, UINT flags )
  {
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void OnRButtonDown( int x, int y )
  {
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void OnRButtonUp( int x, int y, UINT flags = 1 )
  {
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  int Update( void )
  {
    return 0;
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void Refresh( void )
  {
    Redraw();
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void OnMouseMove( int x, int y )
  {
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void OnMouseLeave( void )
  {
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  int GetHeight( HDC hDC )
  {
    xTmpDC  dc( hDC );
    iPt     pt;

    dc.SetSelectFont( pt.fontname4, pt.fontsize4 );

    dc.GetTextMetrics();

    return DPIX( dc.tm.tmHeight + 8 );
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void Redraw( HWND hWnd = NULL, RECT *rc = NULL )
  {
    xDC*  dc;

    if ( hWnd == NULL )
      {
        hWnd = mhWnd;
      }

    if ( rc == NULL )
      {
        rc = &m_currc;
      }

    dc = new xDC( hWnd );

    Draw( hWnd, *dc, rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top );

    delete dc;
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  virtual BOOL doCopy( HWND hWnd, dirent *top, TCHAR *dir )
  {
    DWORD times;
    int   rate;
    BOOL  res;
    DWORD tempTrans = 0;
    DWORD firstTm;
    DWORD nowTime;
    DWORD result_time = 3600;

    _top = top;
    _dir = dir;

    _act.hWnd           = hWnd;
    _act.cancel         = FALSE;
    _act.counter        = 0;
    _act.total_count    = 0;
    _act.total.QuadPart = 0;
    _act.trans.QuadPart = 0;
    _act.in1st.QuadPart = 0;

    Create();

    times   = ::GetTickCount();
    firstTm = times;

    m_cnt  = 0;
    m_rate = 0;

    while ( Wait() )
      {
        if ( _act.total.QuadPart )
          {
            rate = ( int )( _act.trans.QuadPart * 10000 / _act.total.QuadPart ) + 1;

            if ( rate > 10000 )
              {
                rate = 10000;
              }

            nowTime = ::GetTickCount();

            if( nowTime - times > 100 )
              {
                ::SendMessage( hWnd, WM_COMMAND, MAKEWPARAM( ( UINT )( ISTATUS_BASE_ID ), ( UINT )( m_cnt ) ), ( LPARAM )( _act.des_name ) );

                if ( nowTime > firstTm && ( m_cnt % 6 ) == 0 && _act.trans.QuadPart )
                  {
                    tempTrans  = ( DWORD )( ( 8 * 1000 * _act.trans.QuadPart / ( nowTime - firstTm ) ) / 1024 );
                  }

                if ( m_cnt >= 200 )
                  {
                    if ( ( m_cnt % 10 ) == 0 ) {
                      result_time  = ( DWORD )( ( _act.total.QuadPart * ( nowTime - firstTm ) ) / _act.trans.QuadPart );
                      result_time -= ( nowTime - firstTm );
                      result_time /= 1000;  // 秒
                    }

                    if ( result_time >= 3600 )
                      {
                        wsprintf( m_copy_string, L"コピー中...  残り ---- [%3d.%02d%% %3d.%03dMbps ]", rate / 100, rate % 100, tempTrans / 1024, tempTrans % 1000 );
                      }
                    else if( result_time >= 60 )
                      {
                        wsprintf( m_copy_string, L"コピー中...  残り %2d分 [%3d.%02d%% %3d.%03dMbps ]", result_time / 60 + 1, rate / 100, rate % 100, tempTrans / 1024, tempTrans % 1000 );
                      }
                    else
                      {
                        wsprintf( m_copy_string, L"コピー中...  残り %2d秒 [%3d.%02d%% %3d.%03dMbps ]", result_time, rate / 100, rate % 100, tempTrans / 1024, tempTrans % 1000 );
                      }
                  }
                else
                  {
                    wsprintf( m_copy_string, L"コピー中... [%3d.%02d%% %3d.%03dMbps ]", rate / 100, rate % 100, tempTrans / 1024, tempTrans % 1000 );
                  }

                m_cnt++;
                times += 100;

                Redraw();
              }

            m_rate = rate;
          }

        if ( PumpMessage() )
          {
            _act.cancel = TRUE;
            break;
          }
      }

    End();

    ::SendMessage( hWnd, WM_COMMAND, MAKEWPARAM( ( UINT )( ISTATUS_BASE_ID ), ( UINT )( m_cnt ) ), 0 );

    res = CheckCopiedFiles( top );

    if ( !_act.cancel )
      {
        if ( res )
          {
            m_rate = -2;
          }
        else
          {
            m_rate = -4;
          }
      }
    else
      {
        m_rate = -3;
      }

    Redraw();

    m_timer2 = 6;
    m_cnt    = 0;

    return res;
  }
  // --------------------------------------------------------------------------
  
  
  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  BOOL CheckCopiedFiles( dirent *top )
  {
    dirent* elm;
    int     f_total            = 0;
    int     d_total            = 0;
    int     fail_cnt           = 0;
    int     comp_cnt           = 0;
    int     date_not_write_cnt = 0;
    int     null_not_write_cnt = 0;

    for ( elm = top; elm; elm = elm->next )
      {
        if ( elm->bookMark == TCOPYBMARK_NULL || elm->bookMark == TCOPYBMARK_EXIST_DIRECTORY )
          {
            continue;
          }

        if ( elm->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
          {
            d_total++;
          }
        else
          {
            f_total++;
          }

        if ( elm->state == 0 )
          {
            switch ( elm->bookMark )
              {
                case TCOPYBMARK_OVERWRITE:
                case TCOPYBMARK_FAIL_IF_EXISTS:
                case TCOPYBMARK_RENAME:
                  comp_cnt++;
                  break;
                case TCOPYBMARK_NOT_COPY_FOR_DATE:
                  date_not_write_cnt++;
                  break;
                case TCOPYBMARK_NOT_COPY_FOR_FILE:
                  null_not_write_cnt++;
                  break;
                default:
                  MessageBox( mhWnd, L"", L"", 0 );
                  break;
              }
          }
        else if ( elm->state > 0 )
          {
            fail_cnt++;
          }
      }

    m_copy_sub_string[ 0 ] = '\0';

    if ( _act.cancel )
      {
        ssnprintf( m_copy_string, MAX_STRING, L" キャンセル  %,d/ %,d%\n", comp_cnt, f_total + d_total );
      }
    else
      {
        if ( fail_cnt == 0 )
          {
            ssnprintf( m_copy_string, MAX_STRING, L" 成功  %,d/ %,d", comp_cnt, f_total + d_total );
          }
        else
          {
            ssnprintf( m_copy_string, MAX_STRING, L" 成功  %,d/ %,d   失敗  %,d/ %,d", comp_cnt, f_total + d_total, fail_cnt, f_total + d_total );
          }

        if ( null_not_write_cnt > 0 )
          {
            ssnprintf( m_copy_sub_string, MAX_STRING,
              L" コピーされないファイルがあります  %,d/ %,d (同名のファイルが存在)", null_not_write_cnt, f_total + d_total );
          }
        else if ( date_not_write_cnt > 0 )
          {
            ssnprintf( m_copy_sub_string, MAX_STRING,
              L" コピーされないファイルがあります  %,d/ %,d (コピー先のファイルが新しい)", date_not_write_cnt, f_total + d_total );
          }
      }

    return ( fail_cnt == 0 );
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void SetStatus( int f_cnt, int d_cnt, ULARGE_INTEGER *f_size, int marked_f_cnt, int marked_d_cnt, ULARGE_INTEGER *marked_f_size )
  {
    int           total              = f_cnt + d_cnt;
    int           marked             = marked_f_cnt + marked_d_cnt;
    static int    last_f_cnt         = -1;
    static int    last_d_cnt         = -1;
    static int    last_marked_f_cnt  = -1;
    static int    last_marked_d_cnt  = -1;
    static DWORD  last_f_size        = 0xFFFFFFFF;
    static DWORD  last_marked_f_size = 0xFFFFFFFF;
    TCHAR         buf[ 64 ];

    if ( last_f_cnt != f_cnt
      || last_d_cnt != d_cnt
      || last_marked_f_cnt != marked_f_cnt
      || last_marked_d_cnt != marked_d_cnt
      || last_f_size != f_size->LowPart
      || last_marked_f_size != marked_f_size->LowPart )
      {
        m_tick_count = ::GetTickCount();

        if ( marked > 0 )
          {
            ssnprintf( m_string, MAX_STRING, L" %,d 個の項目  %,d 個の選択 ", total, marked );
            ssnprintf( m_sub_string, MAX_STRING, L" %,d 個のファイル  %,d 個のフォルダー  %s バイトの合計ファイルサイズ",
              marked_f_cnt, marked_d_cnt, SizeToStr( marked_f_size->QuadPart, buf ) );
            
          }
        else
          {
            ssnprintf( m_string, MAX_STRING, L" %,d 個の項目 ", total );
            ssnprintf( m_sub_string, MAX_STRING, L" %,d 個のファイル  %,d 個のフォルダー  %s バイトの合計ファイルサイズ",
              f_cnt, d_cnt, SizeToStr( f_size->QuadPart, buf ) );
          }
      }

    last_f_cnt         = f_cnt;
    last_d_cnt         = d_cnt;
    last_marked_f_cnt  = marked_f_cnt;
    last_marked_d_cnt  = marked_d_cnt;
    last_f_size        = f_size->LowPart;
    last_marked_f_size = marked_f_size->LowPart;
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- ドライブ容量を文字列へ
  // --------------------------------------------------------------------------
  TCHAR *SizeToStr( ULONGLONG size, TCHAR *pBuf )
  {
    static TCHAR  hs[] = _T( " KMGT" );
    int           i    = 0;
    ULONGLONG     tmp  = size * 100;
    DWORD         poi;
    DWORD         mod;

    /* ファイル総表示のフォーマット */

    while ( tmp >= 100000 )
      {
        tmp /= 1024;
        i++;
      }

    poi = ( DWORD )( tmp / 100 );
    mod = ( DWORD )( tmp % 100 );

    if ( i == 0 )
      {
        ssnprintf( pBuf, 64, _T( "%,U" ), size );
      }
    else
      {
        ssnprintf( pBuf, 64, _T( "%,U (%d.%02d%c)" ), size, poi, mod, hs[ i ] );
      }

    return pBuf;
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void OnTimer( UINT id )
  {
    static int    last_icon_cnt = 0;

    if ( m_timer2 > 0 )
      {
        m_timer2--;

        if ( m_timer2 == 0 )
          {
            m_copy_string[ 0 ] = 0;
            m_rate = -1;
            Refresh();
          }
      }
    else
      {
        m_timer++;
        m_timer %= 20;
      }

    switch ( m_timer )
      {
        case 0:
        case 7:
          m_icon_cnt = 0;
          break;
        case 1:
          m_icon_cnt = 1;
          break;
        case 2:
          m_icon_cnt = 2;
          break;
        case 3:
        case 4:
        case 5:
        case 6:
          m_icon_cnt = 3;
          break;
        default:
          break;
      }

    if ( last_icon_cnt != m_icon_cnt )
      {
        last_icon_cnt = m_icon_cnt;
        Redraw();
      }
  }
  // --------------------------------------------------------------------------


private:
  HWND  mhWnd;
  BOOL  m_enable;
  TCHAR m_string[ MAX_STRING + 1 ];
  TCHAR m_sub_string[ MAX_STRING + 1 ];
  TCHAR m_copy_string[ MAX_STRING + 1 ];
  TCHAR m_copy_sub_string[ MAX_STRING + 1 ];
  RECT  m_currc;
  DWORD m_tick_count;
  int   m_cnt;
  int   m_rate;
  int   m_timer;
  int   m_timer2;
  int   m_icon_cnt;

};
// --------------------------------------------------------------------------

#endif // _ISTATUS_H_
