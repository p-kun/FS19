/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iMkDirDlg.cpp
 *  Created  : 08/06/04(水) 08:28:31
 *
 *  Function
 *    ディレクトリの作成
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/07/27 10:32:49 $
 *  $Author: poko $
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 */
#include "_box.h"
#include "resource.h"
#include "iComEdit.h"
#include "dirent.h"
#include "SubFuncs.h"
#include "iMkDirDlg.h"


// ==========================================================================
// -- iMkDirDlgクラスの構築 --
// --------------------------------------------------------------------------
iMkDirDlg::iMkDirDlg( HWND hParent ) : xDialog( _T( "IDD_MKDIRDLG" ), hParent )
{
  m_mode = 0;
  mpFont = new xFont;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- iMkDirDlgクラスの破棄 --
// --------------------------------------------------------------------------
iMkDirDlg::~iMkDirDlg()
{
  delete mpEdit;
  delete mpFont;
}
// --------------------------------------------------------------------------


// ===========================================================================
// -- Dialog initialization
// ---------------------------------------------------------------------------
BOOL iMkDirDlg::OnInitDialog( HWND hwndFocus, LPARAM lParam )
{
  CenterWindow();   // Move in the middle

  mpEdit = new iComEdit;
  mpEdit->Attach( GetDlgItem( IDC_EDIT1 ) );
  mpEdit->SetFont( *mpFont );
  mpEdit->SetFocus();
  mpEdit->SendKeyDown( VK_UP, TRUE, 0, 0 );

  mStatic.SetFont( *mpFont );
  mStatic.Attach( GetDlgItem( IDC_STATIC2 ) );
  mStatic.SetText( _T( "□ Create a directory" ) );

  return 0;
}
// ---------------------------------------------------------------------------


// ===========================================================================
// -- Command processing
// ---------------------------------------------------------------------------
void iMkDirDlg::OnCommand( int id, HWND hwndCtl, UINT codeNotify )
{
  TCHAR         buf[ MAX_PATH ];
  SYSTEMTIME    systime;
  const TCHAR*  ext[] = { _T( "" ), _T( ".txt" ) };
  HANDLE        hFile;
  int           mkdir;

  switch( id )
    {
      case IDC_EDIT1:
        if ( ( codeNotify & 0xFF00 ) != ICOMEDIT_KEY )
          {
            break;
          }

        ::GetLocalTime( &systime );

        wsprintf( buf, _T( "%02d%02d%02d_%02d%02d%02d_%03d%s" ),
          systime.wYear % 100,
          systime.wMonth,
          systime.wDay,
          systime.wHour,
          systime.wMinute,
          systime.wSecond,
          systime.wMilliseconds % 1000, ext[ m_mode ] );

        mpEdit->SetText( buf );
        mpEdit->SetSel( 0, 17 );

        if ( m_mode == 0 )
          {
            mStatic.SetText( _T( "□ Create a directory" ) );
            m_mode = 1;
          }
        else
          {
            mStatic.SetText( _T( "■ Create a file" ) );
            m_mode = 0;
          }
        break;
        
      case IDOK:
        mpEdit->GetText( buf, MAX_PATH - 1 );

        wsprintf( m_buf, _T( "%s%s" ), m_dir, buf );

        if ( m_mode )
          {
            PathAddBackslash( m_buf );
            mkdir = ::SHCreateDirectoryEx( mhWnd, m_buf, 0 );

            if ( mkdir == ERROR_SUCCESS )
              {
                ;
              }
            else if ( mkdir == ERROR_BAD_PATHNAME )
              {
                break;
              }
            else if ( mkdir == ERROR_FILENAME_EXCED_RANGE )
              {
                break;
              }
            else if ( mkdir == ERROR_PATH_NOT_FOUND )
              {
                break;
              }
            else if ( mkdir == ERROR_FILE_EXISTS )
              {
                break;
              }
            else if ( mkdir == ERROR_ALREADY_EXISTS )
              {
                break;
              }
            else if ( mkdir == ERROR_CANCELLED )
              {
                break;
              }
            else
              {
                break;
              }
          }
        else
          {
            hFile = ::CreateFile(
                  m_buf,
                  GENERIC_READ,
                  0, //FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
                  NULL,
                  CREATE_NEW,
                  FILE_ATTRIBUTE_NORMAL,
                  NULL );

            if ( hFile == INVALID_HANDLE_VALUE )
              {
                break;
              }

            ::CloseHandle( hFile );
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
// --
// ---------------------------------------------------------------------------
void iMkDirDlg::SetFont( const TCHAR *name, int size )
{
  mpFont->Create( name, size );
}
// ---------------------------------------------------------------------------


// ===========================================================================
// --
// ---------------------------------------------------------------------------
void iMkDirDlg::SetDirectory( const TCHAR *name )
{
  _tcsncpy_s( m_dir, MAX_PATH, name, MAX_PATH );
  PathAddBackslash( m_dir );
}
// ---------------------------------------------------------------------------


// ===========================================================================
// --
// ---------------------------------------------------------------------------
TCHAR *iMkDirDlg::GetSel( void )
{
  StrTrimRight( m_buf, _T( "\\" ) );
  return m_buf;
}
// ---------------------------------------------------------------------------
