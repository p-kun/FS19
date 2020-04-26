/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iLogDskDlg.h
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
#include "resource.h"
#include "iComList.h"
#include "iComEdit.h"
#include "SubFuncs.h"
#include "iPt.h"
#include "iHistory.h"
#include "iLogDskDlg.h"


// ==========================================================================
// -- Construction of iLogDskDlg class
// --------------------------------------------------------------------------
iLogDskDlg::iLogDskDlg( HWND hParent ) : xDialog( _T( "IDD_LOGDSK" ), hParent )
{
  mpLogList = new iComList;
  mpEdit    = new iComEdit;
  mpFont    = new xFont;

  ZeroMemory( logdrv, sizeof( logdrv ) );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- Destroy iLogDskDlg class
// --------------------------------------------------------------------------
iLogDskDlg::~iLogDskDlg()
{
  delete mpFont;
  delete mpLogList;
  delete mpEdit;
}
// --------------------------------------------------------------------------


// ===========================================================================
// -- Command processing
// ---------------------------------------------------------------------------
void iLogDskDlg::OnCommand( int id, HWND hwndCtl, UINT codeNotify )
{
  TCHAR     buf[ MAX_PATH2 ];
  iHistory  his;

  switch( id )
    {
      case IDC_EDIT1:
        if ( ( codeNotify & 0xFF00 ) != ICOMEDIT_KEY )
          {
            break;
          }

        mpLogList->SendMessage( WM_KEYDOWN, codeNotify & 0xFF, 0 );
        mpEdit->SetSel( 0, -1 );

        if ( ( codeNotify & 0xFF ) == VK_INSERT )
          {
            id = mpLogList->GetCurSel();

            if ( mpLogList->GetItemData( id ) != 1 )
              {
                break;
              }

            mpLogList->GetText( id, buf );

            if ( buf[ 0 ] == _T( '*' ) )
              {
                buf[ 0 ] = _T( ' ' );
              }
            else
              {
                buf[ 0 ] = _T( '*' );
              }

            mpLogList->DeleteString( id );

            mpLogList->InsertString( id, buf );
            mpLogList->SetItemData( id, 1 );
            mpLogList->SetCurSel( id );

            his.SetMark( buf + HISTORY_PATH_OFFSET );
          }

        break;

      case IDC_LIST1:
        id = mpLogList->GetCurSel();

        mpLogList->GetText( id, buf );

        if ( mpLogList->GetItemData( id ) == 1 )
          {
            mpEdit->SetWindowText( buf + HISTORY_PATH_OFFSET );
          }
        else
          {
            mpEdit->SetWindowText( buf );
          }

        if ( codeNotify != LBN_DBLCLK )
          {
            break;
          }

      case IDOK:
        mpEdit->GetText( logdrv, MAX_PATH2 );

        if ( _tcslen( logdrv ) < 3 )
          {
            /* Drive name correction */

            logdrv[ 1 ] = ':';
            logdrv[ 2 ] = '\\';
            logdrv[ 3 ] = '\0';
          }

        if ( !CheckPathRules( logdrv ) )
          {
            wsprintf( buf, L"\"%s\" Invalid path !!", logdrv );
            ::MessageBox( mhWnd, buf, L"Failure(-o-;", MB_ICONSTOP );
            break;
          }

        if ( CheckPath( logdrv, 2, mhWnd, 1 ) < 2 )
          {
            break;
          }

        if ( !PathIsRoot( logdrv ) && !PathIsUNC( logdrv ) && !PathFileExists( logdrv ) )
          {
            if ( ::MessageBox( mhWnd, _T( "There is no folder. Do you want to make it? " ), _T( "What will you do?" ), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 ) == IDNO )
              {
                break;
              }

            PathAddBackslash( logdrv );

            if ( SHCreateDirectoryEx( mhWnd, logdrv, 0 ) != ERROR_SUCCESS  )
              {
                ::MessageBox( mhWnd, L"Failed to create folder !!", L"Failure(-o-;", MB_ICONSTOP );
                break;
              }
          }

        EndDialog( IDOK );
        break;

      case IDCANCEL:
        EndDialog( IDCANCEL );
        break;

      default:
        break;
    }
}
// ---------------------------------------------------------------------------


// ===========================================================================
// -- Dialog initialization
// ---------------------------------------------------------------------------
BOOL iLogDskDlg::OnInitDialog( HWND hwndFocus, LPARAM lParam )
{
  TCHAR buf[ MAX_PATH2 ];
  iPt   pt;
  RECT  rc;

  GetClientRect(&rc);

  Attach( *mpLogList, IDC_LIST1 );
  Attach( *mpEdit,    IDC_EDIT1 );

  SetList();

  /* Move to center */

  CenterWindow();

  mpFont->Create( pt.fontname3, pt.fontsize3 );

  mpEdit->SetFocus();
  mpEdit->SetFont( *mpFont );

  if ( logdrv[ 0 ] == '\0' )
    {
      mpLogList->SetCurSel( 0 );
      mpLogList->GetText( 0, buf );
      mpEdit->SetText( buf );
    }
  else
    {
      mpEdit->SetText( logdrv );
    }

  mpEdit->SetSel( 0, -1 );

  return 0;
}
// ---------------------------------------------------------------------------


// ===========================================================================
// --
// ---------------------------------------------------------------------------
void iLogDskDlg::SetCurrent( const TCHAR *name )
{
  _tcscpy_s( logdrv, MAX_PATH2, name );
}
// ---------------------------------------------------------------------------


// ===========================================================================
// -- WM_PAINT
// ---------------------------------------------------------------------------
void iLogDskDlg::OnPaint( void )
{
  xPaintDC  dc( mhWnd );
  RECT      rc;
  RECT      rc2;
  iPt       pt;
  int       x;
  int       y;

  GetClientRect( &rc );

  mpLogList->GetWindowRect( &rc2 );

  dc.SetSelectFont( pt.fontname1, pt.fontsize1 );
  dc.SetBkMode( TRANSPARENT );

  rc.bottom = rc2.bottom;
  dc.FillRect( &rc, pt.COL_BASE_BAR );

  x = rc.left + 8;
  y = rc.top  + 8;

  dc.SetTextColor( pt.COL_TITLE );
  dc.TextOut( x, y, _T( "Enter the drive name or folder name to be changed." ) );
}
// ---------------------------------------------------------------------------


// ===========================================================================
// -- 
// ---------------------------------------------------------------------------
void iLogDskDlg::SetList( void )
{
  iHistory  his;
  int       i;
  int       cur = 0;
  int       cnt = mpLogList->GetCount();
  TCHAR     buf[ MAX_PATH2 ];
  TCHAR     drv[ MAX_PATH2 ];
  TCHAR*    cp;
  iPt       pt;

  for ( i = 0; i < cnt; i++ )
    {
      mpLogList->DeleteString( 0 );
    }

  /* Get list of drives */

  GetLogicalDriveStrings( MAX_PATH2, buf );

  _tcscpy_s( drv, MAX_PATH2, logdrv );
  PathStripToRoot( drv );

  for ( i = 0, cp = buf; *cp != '\0'; cp += ( lstrlen( cp ) + 1 ) )
    {
      if ( GetDriveType( cp ) == DRIVE_REMOTE )
        {
          ;
        }
      else
        {
          if ( !PathFileExists( cp ) )
            {
              continue;
            }
        }

      mpLogList->SetLogData( i, cp, 0 );

      if ( _tcsicmp( drv, cp ) == 0 )
        {
          mpLogList->SetCurSel( i );
        }
      i++;
    }

  for ( i = 0; i < his.GetCount(); i++ )
    {
      mpLogList->SetLogData( 0, his.GetAt( i ), 1 );
      cur = i;
    }

  /* Set the cursor to the end and then set the cursor to the current! */

  mpLogList->SetCurSel( mpLogList->GetCount() - 1 );
  mpLogList->SetCurSel( cur );
}
// ---------------------------------------------------------------------------
