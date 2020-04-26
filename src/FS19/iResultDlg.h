/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iResultDlg.h
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
#ifndef _IRESULTDLG_H_
#define _IRESULTDLG_H_


#include "ipt.h"
#include "formatinc.h"
#include "resource.h"


// ==========================================================================
// -- iComList2 class --
// --------------------------------------------------------------------------
class iComList2 : public xListBox
{
public:
  TCHAR*  dir;

private:
  enum {
    BASE_HEIGHT = 16,
    OFFSET_TOP  = 3,
    OFFSET1     = 2,
    OFFSET2     = 2,
    OFFSET_DOWN = 3,
  };

public:
  // ======================================================================
  // -- 
  // ----------------------------------------------------------------------
  iComList2() {
    dir = NULL;
  }
  // ----------------------------------------------------------------------


  // ======================================================================
  // -- 
  // ----------------------------------------------------------------------
  ~iComList2() {
  }
  // ----------------------------------------------------------------------


private:
  // ======================================================================
  // -- 
  // ----------------------------------------------------------------------
  int DrawItem( const DRAWITEMSTRUCT *pDIS )
  {
    xTmpDC    dc( pDIS->hwndItem, pDIS->hDC );
    xBmpDC    bmp;
    TCHAR     buf[ MAX_PATH * 2 ];
    iPt       pt;
    RECT      rc;
    RECT      rc_bmp;
    dirent*   elm;
    int       height = BASE_HEIGHT;
    iico      ico(bmp, IMG_COPYDLG);

    if ( pDIS->itemID == ( UINT )(~0) )
      {
        return 1;
      }

    rc = pDIS->rcItem;

    rc_bmp.left   = 0;
    rc_bmp.top    = 0;
    rc_bmp.right  = rc.right - rc.left;
    rc_bmp.bottom = rc.bottom - rc.top;

    bmp.Create( dc, rc_bmp.right, rc_bmp.bottom );

    bmp.SetBkMode( TRANSPARENT );
    bmp.SetTextColor( 0 );
    bmp.SetSelectFont( pt.fontname2, pt.fontsize2 );

    if ( BASE_HEIGHT < bmp.tm.tmHeight )
      {
        height = bmp.tm.tmHeight;
      }

    if ( pDIS->itemState & ODS_SELECTED )
      {
        bmp.FillRect( &rc_bmp, RGB( 224, 224, 224 ) );
      }
    else
      {
        bmp.FillRect( &rc_bmp, pt.COL_BASE_BAR );
      }

    elm = ( dirent* )GetItemData( pDIS->itemID );

    GetText( pDIS->itemID, buf );

    if ( buf[ 0 ] != 0 )
      {
        bmp.TextOut( 4 + BASE_HEIGHT + 4, ( rc_bmp.bottom - bmp.tm.tmHeight ) / 2, buf + 1 );

        if ( buf[ 0 ] == '0' )
          {
            ico.draw(4, ( rc_bmp.bottom - BASE_HEIGHT ) / 2, 1, TRUE);
          }
        else
          {
            ico.draw(4, ( rc_bmp.bottom - BASE_HEIGHT ) / 2, 0, TRUE);
          }
      }
    else
      {
        height += ( OFFSET1 + OFFSET2 );
        ico.draw(4, OFFSET_TOP + ( height - BASE_HEIGHT ) / 2, 1, TRUE);
        bmp.TextOut( 4 + 16 + 4, OFFSET_TOP + ( height - bmp.tm.tmHeight ) / 2, GetLastErrString( elm->state ) );

        _tcscpy_s(buf, MAX_PATH, elm->d_name);

        if ( elm->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
          {
            _tcscat_s( buf, MAX_PATH, L" <folder>" );
          }

        bmp.SetTextColor( FormatColor( elm ) );
        bmp.TextOut( 4 + 16 + 4, OFFSET_TOP + height + ( height - bmp.tm.tmHeight ) / 2, buf );

        ico.draw(4 + 16 + 4, OFFSET_TOP + height + height + ( height - BASE_HEIGHT ) / 2, 3, TRUE);

        PathAppend( buf, ( ( dirent* )elm->param )->d_name + ( ( dirent* )elm->param )->d_cnt );

        bmp.TextOut( 4 + 16 + 4 + 16 + 4, OFFSET_TOP + height + height + ( height - bmp.tm.tmHeight ) / 2, buf );
      }

    dc.BitBlt( rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, bmp, 0, 0, SRCCOPY );

    return 1;
  }
  // ----------------------------------------------------------------------


  // ======================================================================
  // -- 
  // ----------------------------------------------------------------------
  int MeasureItem( MEASUREITEMSTRUCT *pMIS )
  {
    TCHAR path[ MAX_PATH ];
    xDC   dc( mhWnd );
    iPt   pt;
    int   height = BASE_HEIGHT;

    dc.SetSelectFont( pt.fontname1, pt.fontsize1 );

    GetText( pMIS->itemID, path );

    if ( BASE_HEIGHT < dc.tm.tmHeight )
      {
        height = dc.tm.tmHeight;
      }

    if ( path[ 0 ] == 0 )
      {
        pMIS->itemHeight = OFFSET_TOP + ( OFFSET1 + height + OFFSET1 ) * 3 + OFFSET_DOWN;
      }
    else
      {
        pMIS->itemHeight = ( OFFSET_TOP + height + OFFSET_DOWN );
      }

    return 1;
  }
  // ----------------------------------------------------------------------


private:
  // ======================================================================
  // -- 
  // ----------------------------------------------------------------------
  void PathAppend( TCHAR *buf, const TCHAR *path )
  {
    if ( dir )
      {
        StrCpy( buf, dir );
        ::PathAppend( buf, path );
      }
  }
  // ----------------------------------------------------------------------


  // ======================================================================
  // -- Last error content display
  // ----------------------------------------------------------------------
  TCHAR *GetLastErrString( DWORD err )
  {
    static TCHAR  buf[ 128 ];

    FormatMessage( 
      FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      err,
      MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),    // Default language
      ( LPTSTR )buf,
      128,
      NULL 
    );

    return buf;
  }
  // ----------------------------------------------------------------------

};
// --------------------------------------------------------------------------


// ==========================================================================
// -- iResultDlg
// --------------------------------------------------------------------------
class iResultDlg : public xDialog
{
public:
  // ======================================================================
  // -- iEjectDskDlg class construction
  // ----------------------------------------------------------------------
  iResultDlg( HWND hParent ) : xDialog( _T( "IDD_RESULTDSK" ), hParent )
  {
    mpLogList = NULL;
    mpTop = NULL;
    mIsCancel = FALSE;
  }
  // ----------------------------------------------------------------------


  // ======================================================================
  // -- 
  // ----------------------------------------------------------------------
  ~iResultDlg()
  {
    delete mpLogList;
  }
  // ----------------------------------------------------------------------


  // ======================================================================
  // -- 
  // ----------------------------------------------------------------------
  void SetExtParams( dirent *top, TCHAR *dir, BOOL isCancel )
  {
    mpTop = top;
    StrCpy( mDirs, dir );
    mIsCancel = isCancel;
  }
  // ----------------------------------------------------------------------


private:
  // ======================================================================
  // -- 
  // ----------------------------------------------------------------------
  BOOL OnInitDialog( HWND hwndFocus, LPARAM lParam )
  {
    dirent* elm;
    TCHAR   buf[ 1024 ];
    TCHAR   tmp[  512 ];
    int     f_total            = 0;
    int     d_total            = 0;
    int     fail_cnt           = 0;
    int     comp_cnt           = 0;
    int     over_write_cnt     = 0;
    int     date_not_write_cnt = 0;
    int     name_write_cnt     = 0;
    int     null_not_write_cnt = 0;

    mpLogList = new iComList2;

    Attach( *mpLogList, IDC_LIST1 );

    mpLogList->dir = mDirs;

    for ( elm = mpTop; elm; elm = elm->next )
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
                  comp_cnt++;
                  break;
                case TCOPYBMARK_NOT_COPY_FOR_DATE:
                  date_not_write_cnt++;
                  break;
                case TCOPYBMARK_NOT_COPY_FOR_FILE:
                  null_not_write_cnt++;
                  break;
                default:
                  MessageBox( L"", L"", 0 );
                  break;
              }
          }
        else if ( elm->state > 0 )
          {
            if ( fail_cnt < 100 )
              {
                mpLogList->InsertString( 0, L"" );
                mpLogList->SetItemData( 0, ( WPARAM )elm );
              }
            fail_cnt++;
          }
      }

    if ( mIsCancel )
      {
        SetWindowText( L"Cancel" );
        ssnprintf( buf, 512, _T( "0 Cancel: %,d / %,d  (ファイル: %,d  フォルダー: %,d   日付: %,d  変更: %,d  上書: %,d  対象外: %,d)  失敗: %,d" ),
          comp_cnt, f_total + d_total, f_total, d_total,  date_not_write_cnt, name_write_cnt, over_write_cnt, null_not_write_cnt, fail_cnt );
      }
    else
      {
        if ( fail_cnt == 0 )
          {
            SetWindowText( L"Complete" );
            ssnprintf( buf, 512, L" -----Complete  %,d/ %,d", comp_cnt, f_total + d_total );
          }
        else
          {
            SetWindowText( L"An error has occurred!" );
            ssnprintf( buf, 512, L" Complete  %,d/ %,d   Failure  %,d/ %,d", comp_cnt, f_total + d_total, fail_cnt, f_total + d_total );
          }

        if ( null_not_write_cnt > 0 )
          {
            ssnprintf( tmp, 512, L"   Some files are not copied  %,d/ %,d (File with the same name)", null_not_write_cnt, f_total + d_total );
          }
        else if ( date_not_write_cnt > 0 )
          {
            ssnprintf( tmp, 512, L"   Some files are not copied  %,d/ %,d (The destination file is new)", date_not_write_cnt, f_total + d_total );
          }
        else
          {
            tmp[ 0 ] = '\0';
          }

        StrCat( buf, tmp );
      }

    mpLogList->InsertString( 0, buf );

    CenterWindow();   // Move in the middle

    mpLogList->SetCurSel( 0 );
    mpLogList->SetFocus();

    return 0;
  }
  // ----------------------------------------------------------------------


  // ======================================================================
  // -- 
  // ----------------------------------------------------------------------
  void OnCommand( int id, HWND hwndCtl, UINT codeNotify )
  {
    switch ( id )
      {
        case IDC_LIST1:
          if ( codeNotify != LBN_DBLCLK )
            {
              break;
            }

        case IDOK:
          break;

        case IDCANCEL:
          EndDialog( IDOK );
          break;

        default:
          break;
      }
  }
  // ----------------------------------------------------------------------


private:
  iComList2*  mpLogList;
  dirent*     mpTop;
  TCHAR       mDirs[ MAX_PATH ];
  BOOL        mIsCancel;

};
// --------------------------------------------------------------------------


#endif // _IRESULTDLG_H_
