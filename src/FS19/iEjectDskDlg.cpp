/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iEjectDskDlg.h
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
#include "SubFuncs.h"
#include "iEjectDskDlg.h"


// ==========================================================================
// -- Construction of iEjectDskDlg class
// --------------------------------------------------------------------------
iEjectDskDlg::iEjectDskDlg( HWND hParent ) : xDialog( _T( "IDD_EJECTDSK" ), hParent )
{
  mpLogList = NULL;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- iEjectDskDlg class destruction
// --------------------------------------------------------------------------
iEjectDskDlg::~iEjectDskDlg()
{
  delete mpLogList;
}
// --------------------------------------------------------------------------


// ===========================================================================
// -- Command processing
// ---------------------------------------------------------------------------
void iEjectDskDlg::OnCommand( int id, HWND hwndCtl, UINT codeNotify )
{
  int   cmd  = 0;
  TCHAR buf[ MAX_PATH ];

  switch ( id )
    {
      case IDC_LIST1:
        if ( cmd != LBN_DBLCLK )
          {
            break;
          }
      case IDOK:
        id = mpLogList->GetCurSel();

        mpLogList->GetText( id, buf );

        if ( DiscEject( buf ) == FALSE )
          {
            break;
          }

        EndDialog( IDOK );
        break;

      case IDCANCEL:
        //
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
BOOL iEjectDskDlg::OnInitDialog( HWND hwndFocus, LPARAM lParam )
{
  TCHAR   buf[ MAX_PATH ];
  int     type;
  TCHAR*  cp;

  mpLogList = new iComList;

  Attach( *mpLogList, IDC_LIST1 );

  /* Get list of drives */

  GetLogicalDriveStrings( MAX_PATH, buf );

  for ( cp = buf; *cp != '\0'; cp += ( lstrlen( cp ) + 1 ) )
    {
      type = GetDriveType( cp );

      if ( !( type == DRIVE_CDROM || type == DRIVE_REMOVABLE ) )
        {
          continue;
        }

      if ( !PathFileExists( cp ) )
        {
          continue;
        }

      mpLogList->SetLogData( -1, cp, 0 );
    }

  /* Move in the middle */

  CenterWindow();

  mpLogList->SetCurSel( 0 );
  mpLogList->SetFocus();

  return 0;
}
// ---------------------------------------------------------------------------


// ==========================================================================
// -- Eject disc
// --------------------------------------------------------------------------
static BOOL DiscEjectSub( TCHAR path[] )
{
  return EjectVolume( path[ 0 ] );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- Eject disc
// --------------------------------------------------------------------------
BOOL iEjectDskDlg::DiscEject( TCHAR *drv )
{
  BOOL  res = FALSE;

  res = DiscEjectSub( drv );

  if ( res )
    {
      MessageBox( _T( "Ejected. Unplug it." ), _T( "Success!" ), MB_ICONINFORMATION );
    }
  else
    {
      MessageBox( _T( "Could not eject (; _;)\r\nClose other applications." ), _T( "Error!" ), MB_ICONSTOP );
    }

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- Loading a disc
// --------------------------------------------------------------------------
BOOL iEjectDskDlg::DiscLoad( int c )
{
  return FALSE;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL iEjectDskDlg::CheckEjectDrive( void )
{
  TCHAR   buf[ MAX_PATH ];
  BOOL    res = FALSE;
  int     type;
  TCHAR*  cp;

  /* Get list of drives */

  GetLogicalDriveStrings( MAX_PATH, buf );

  for ( cp = buf; *cp != '\0'; cp += ( lstrlen( cp ) + 1 ) )
    {
      type = GetDriveType( cp );

      if ( !( type == DRIVE_CDROM || type == DRIVE_REMOVABLE ) )
        {
          continue;
        }

      if ( !PathFileExists( cp ) )
        {
          continue;
        }

      res = TRUE;
      break;
    }

  return res;
}
// --------------------------------------------------------------------------
