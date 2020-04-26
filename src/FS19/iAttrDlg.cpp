/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iAttrDlg.h
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
#include "iPt.h"
#include "xFont.h"
#include "SubFuncs.h"
#include "logf.h"
#include "iAttrDlg.h"

// --
#define FIND_DATA	WIN32_FIND_DATA


// ==========================================================================
// -- iAttrDlgクラスの構築 --
// --------------------------------------------------------------------------
iAttrDlg::iAttrDlg( HWND hParent ) : xDialog( _T( "IDD_ATTRDLG" ), hParent )
{
	mAttribute  = 0;
	mbEnableDir = FALSE;

	mpFont = new xFont;

	mpCheckBoxs[ 0 ] = new xButton;
	mpCheckBoxs[ 1 ] = new xButton;
	mpCheckBoxs[ 2 ] = new xButton;
	mpCheckBoxs[ 3 ] = new xButton;
	mpCheckBoxs[ 4 ] = new xButton;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- iAttrDlgクラスの破棄 --
// --------------------------------------------------------------------------
iAttrDlg::~iAttrDlg()
{
	delete mpCheckBoxs[ 0 ];
	delete mpCheckBoxs[ 1 ];
	delete mpCheckBoxs[ 2 ];
	delete mpCheckBoxs[ 3 ];
	delete mpCheckBoxs[ 4 ];
	delete mpFont;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- iAttrDlgクラスの破棄 --
// --------------------------------------------------------------------------
void iAttrDlg::SetAttribute( dirent* entry )
{
	mp_entry = entry;
}
// --------------------------------------------------------------------------


// ===========================================================================
// -- コマンドの処理
// ---------------------------------------------------------------------------
void iAttrDlg::OnCommand( int id, HWND hwndCtl, UINT codeNotify )
{
	switch ( id )
    {
    	case IDOK:
    		if ( mpCheckBoxs[ 0 ]->GetCheck() )
      	  {
      			mAttribute |= FILE_ATTRIBUTE_READONLY;
      		}
    		else
      	  {
      			mAttribute &= ~FILE_ATTRIBUTE_READONLY;
      		}

    		if ( mpCheckBoxs[ 1 ]->GetCheck() )
      	  {
      			mAttribute |= FILE_ATTRIBUTE_HIDDEN;
      		}
    		else
      	  {
      			mAttribute &= ~FILE_ATTRIBUTE_HIDDEN;
      		}

    		if ( mpCheckBoxs[ 2 ]->GetCheck() )
      	  {
      			mAttribute |= FILE_ATTRIBUTE_SYSTEM;
      		}
    		else
      	  {
      			mAttribute &= ~FILE_ATTRIBUTE_SYSTEM;
      		}

    		if ( mpCheckBoxs[ 3 ]->GetCheck() )
      	  {
      			mAttribute |= FILE_ATTRIBUTE_ARCHIVE;
      		}
    		else
      	  {
      			mAttribute &= ~FILE_ATTRIBUTE_ARCHIVE;
      		}

    		mbEnableDir = mpCheckBoxs[ 4 ]->GetCheck();

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
// -- ダイアログの初期化
// ---------------------------------------------------------------------------
BOOL iAttrDlg::OnInitDialog( HWND hwndFocus, LPARAM lParam )
{
	iPt		pt;

	CenterWindow();		// 真ん中に移動

	mpFont->Create( pt.fontname1, pt.fontsize1 );

	mpCheckBoxs[ 0 ]->Attach( GetDlgItem( IDC_CHECK1 ) );
	mpCheckBoxs[ 1 ]->Attach( GetDlgItem( IDC_CHECK2 ) );
	mpCheckBoxs[ 2 ]->Attach( GetDlgItem( IDC_CHECK3 ) );
	mpCheckBoxs[ 3 ]->Attach( GetDlgItem( IDC_CHECK4 ) );
	mpCheckBoxs[ 4 ]->Attach( GetDlgItem( IDC_CHECK5 ) );

	mAttribute = mp_entry->dwFileAttributes;

	mpCheckBoxs[ 0 ]->SetFont( *mpFont );
	mpCheckBoxs[ 1 ]->SetFont( *mpFont );
	mpCheckBoxs[ 2 ]->SetFont( *mpFont );
	mpCheckBoxs[ 3 ]->SetFont( *mpFont );
	mpCheckBoxs[ 4 ]->SetFont( *mpFont );
	mpCheckBoxs[ 0 ]->SetCheck( ( mAttribute & FILE_ATTRIBUTE_READONLY ) != 0 );
	mpCheckBoxs[ 1 ]->SetCheck( ( mAttribute & FILE_ATTRIBUTE_HIDDEN )   != 0 );
	mpCheckBoxs[ 2 ]->SetCheck( ( mAttribute & FILE_ATTRIBUTE_SYSTEM )   != 0 );
	mpCheckBoxs[ 3 ]->SetCheck( ( mAttribute & FILE_ATTRIBUTE_ARCHIVE )  != 0 );
	mpCheckBoxs[ 4 ]->SetCheck( FALSE );
	mpCheckBoxs[ 4 ]->Enable( TRUE );

	mpCheckBoxs[ 0 ]->SetFocus();

	return TRUE;
}
// ---------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static void open_sub( const TCHAR *dir, DWORD attribute )
{
	HANDLE		hFile;
	FIND_DATA	fd;
	TCHAR 		buf[ MAX_PATH ];

	_tcscpy_s( buf, MAX_PATH, dir );
	PathAddBackslash( buf );
	_tcscat_s( buf, MAX_PATH, _T( "*.*" ) );

	hFile = ::FindFirstFile( buf, &fd );

	if ( INVALID_HANDLE_VALUE == hFile )
    {
      return;
    }

	do
    {
			if ( _tcscmp( fd.cFileName, _T( "." ) ) == 0 || _tcscmp( fd.cFileName, _T( ".." ) ) == 0 )
  		  {
  				continue;
  			}

		  _tcscpy_s( buf, MAX_PATH, dir );
			PathAddBackslash( buf );
			_tcscat_s( buf, MAX_PATH, fd.cFileName );

			if ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
  				open_sub( buf, attribute );
  			}

			::SetFileAttributes( buf, attribute );
		}
	while( ::FindNextFile( hFile, &fd ) );

	::FindClose( hFile );

}
// --------------------------------------------------------------------------


// ===========================================================================
// -- ダイアログの初期化
// ---------------------------------------------------------------------------
void iAttrDlg::SetAttributes( void )
{
	dirent*	entry;

	for ( entry = mp_entry; entry; entry = entry->next )
    {
  		::SetFileAttributes( entry->d_name, mAttribute );

  		if ( mbEnableDir && entry->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
    			open_sub( entry->d_name, mAttribute );
    		}
  	}
}
// ---------------------------------------------------------------------------


// ==========================================================================
// -- WM_PAINTの処理
// --------------------------------------------------------------------------
void iAttrDlg::OnPaint()
{
	xPaintDC	dc( mhWnd );
	RECT		rc;
	RECT		rc2;
	HWND		hWnd;
	iPt			pt;

	GetClientRect( &rc );

	rc.left  += 16;
	rc.right -= 32;
	rc.top   += 16;

	hWnd = GetDlgItem( IDC_STATIC1 );
	::GetClientRect( hWnd, &rc2 );

	rc.right -= ( rc2.right - rc2.left ) ;

	DispFileStatus( dc, &rc, mp_entry, pt.fontname2, pt.fontsize2 );

}
// --------------------------------------------------------------------------
