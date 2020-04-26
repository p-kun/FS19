/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iRenameDlg.cpp
 *  Created  : 08/06/04(水) 08:28:31
 *
 *  Function
 *    リネームダイアログ
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/07/27 10:32:49 $
 *  $Author: poko $
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 */
#include "_box.h"
#include "dirent.h"
#include "ssnprintf.h"
#include "resource.h"
#include "SubFuncs.h"
#include "iPt.h"
#include "iRenameDlg.h"


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iRenameDlg::SetFileInfo( dirent* entry )
{
  _tcscpy_s( m_dir,  MAX_PATH, entry->d_name );
  _tcscpy_s( m_name, MAX_PATH, entry->d_name );

  PathRemoveFileSpec( m_dir );

  m_entry = *entry;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- WM_PAINTの処理
// --------------------------------------------------------------------------
void iRenameDlg::OnPaint()
{
  xPaintDC  dc( mhWnd );
  RECT      rc;
  iPt       pt;
  dirent*   entry;
  size_t    len;

  GetClientRect( &rc );

  dc.FillRect(&rc, pt.COL_BASE_BAR);

  rc.left  += DPIX(16);
  rc.right -= DPIX(16);
  rc.top   += DPIY(16);

  len = _tcslen(m_name) + 1;

  entry = (dirent *)alloca(sizeof(dirent) + len * sizeof(TCHAR));

  memcpy(entry, &m_entry, sizeof(dirent));

  _tcscpy_s(entry->d_name, len, m_name);

  DispFileStatus( dc, &rc, entry, pt.fontname2, pt.fontsize2 );
}
// --------------------------------------------------------------------------


// ===========================================================================
// -- iRenameDlgクラスの構築 --
// ---------------------------------------------------------------------------
iRenameDlg::iRenameDlg( HWND hParent ) : xDialog( _T( "IDD_RENAMEDLG" ), hParent )
{
  mpFont = new xFont;
}
// ---------------------------------------------------------------------------



// ===========================================================================
// -- iRenameDlgクラスの破棄 --
// ---------------------------------------------------------------------------
iRenameDlg::~iRenameDlg()
{
  if ( mpFont )
    {
      delete mpFont;
    }
}
// ---------------------------------------------------------------------------



// ===========================================================================
// -- ダイアログの初期化
// ---------------------------------------------------------------------------
BOOL iRenameDlg::OnInitDialog( HWND hwndFocus, LPARAM lParam )
{
  iPt   pt;

  /* 真ん中に移動 */

  CenterWindow();

  mpFont->Create( pt.fontname2, pt.fontsize2 );

  mEdit.Attach( GetDlgItem( IDC_EDIT1 ) );

  mEdit.SetFocus();
  mEdit.SetText( PathFindFileName( m_name ) );
  mEdit.SetSel( 0, ( int )( PathFindExtension( m_name ) - PathFindFileName( m_name ) ) );
  mEdit.SetFont( *mpFont );

  return 0;
}
// ---------------------------------------------------------------------------


// ===========================================================================
// -- コマンドの処理
// ---------------------------------------------------------------------------
void iRenameDlg::OnCommand( int id, HWND hwndCtl, UINT codeNotify )
{
  TCHAR buf[ MAX_PATH ];

  switch ( id )
    {
      case IDOK:
        mEdit.GetText( buf, MAX_PATH );
        _tcscpy_s( rename, MAX_PATH, m_dir );
        PathAppend( rename, buf );
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
