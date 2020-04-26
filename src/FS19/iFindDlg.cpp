/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iFindDlg.cpp
 *  Created  : 08/06/04(水) 08:28:31
 *
 *  Function
 *    ファインダイアログ
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
#include "iPt.h"
#include "resource.h"
#include "SubFuncs.h"
#include "logf.h"
#include "iFindDlg.h"


// ===========================================================================
// -- iFindDlgクラスの構築 --
// ---------------------------------------------------------------------------
iFindDlg::iFindDlg( HWND hParent ) : xDialog( _T( "IDD_FIND" ), hParent )
{
  mpFont = new xFont;
  mpCmbo = new xComboBox;
}
// ---------------------------------------------------------------------------


// ===========================================================================
// -- iFindDlgクラスの破棄 --
// ---------------------------------------------------------------------------
iFindDlg::~iFindDlg()
{
  if ( mpFont )
    {
      delete mpFont;
    }

  if ( mpCmbo )
    {
      delete mpCmbo;
    }
}
// ---------------------------------------------------------------------------


// ===========================================================================
// -- ダイアログの初期化
// ---------------------------------------------------------------------------
BOOL iFindDlg::OnInitDialog( HWND hwndFocus, LPARAM lParam )
{
  iPt   pt;

  CenterWindow();   // 真ん中に移動

  mpFont->Create( pt.fontname1, pt.fontsize1 );

  Attach( mpCmbo, IDC_COMBO1 );

  mpCmbo->SetFont( *mpFont );

  SetList();

  mpCmbo->SetCurSel( 0 );
  mpCmbo->SetFocus();

  return 0;
}
// ---------------------------------------------------------------------------


// ===========================================================================
// -- コマンドの処理
// ---------------------------------------------------------------------------
void iFindDlg::OnCommand( int id, HWND hwndCtl, UINT codeNotify )
{
  switch( id )
    {
      case IDOK:
        SaveList();
        mpCmbo->GetText( m_target, MAX_PATH );

        if ( m_target[ 0 ] == '\0' )
          {
            EndDialog( IDCANCEL );
          }
        else
          {
            EndDialog( IDOK );
          }

        break;

      case IDCANCEL:
        EndDialog( IDCANCEL );
        break;

      case IDC_COMBO1:
        if ( codeNotify == CBN_SELCHANGE )
          {
            mpCmbo->ShowDropdown( TRUE );
          }
        else if ( codeNotify == CBN_DBLCLK )
          {
          }
        else if ( codeNotify == CBN_SETFOCUS )
          {
          }
        else if ( codeNotify == CBN_KILLFOCUS )
          {
          }
        else if ( codeNotify == CBN_EDITCHANGE )
          {
            mpCmbo->ShowDropdown( FALSE );
          }
        else if ( codeNotify == CBN_EDITUPDATE )
          {
          }
        else if ( codeNotify == CBN_DROPDOWN )
          {
          }
        else if ( codeNotify == CBN_CLOSEUP )
          {
          }
        else if ( codeNotify == CBN_SELENDOK )
          {
          }
        else if ( codeNotify == CBN_SELENDCANCEL )
          {
          }

        break;

      default:
        break;
    }
}
// ---------------------------------------------------------------------------


// ===========================================================================
// -- 
// ---------------------------------------------------------------------------
void iFindDlg::SetList( void )
{
  iPt     pt;
  TCHAR   wclist[MAX_PATH ];
  TCHAR*  cp;
  TCHAR*  contex;

  _tcscpy_s( wclist, MAX_PATH, pt.wclist );

  for( cp = _tcstok_s( wclist, _T( " " ), &contex ); cp; cp = _tcstok_s( NULL, _T( " " ), &contex ) )
    {
      mpCmbo->InsertString( -1, cp );
    }
}
// ---------------------------------------------------------------------------


// ===========================================================================
// -- 
// ---------------------------------------------------------------------------
void iFindDlg::SaveList( void )
{
  iPt     pt;
  TCHAR   text[ MAX_PATH ];
  TCHAR   wc[ MAX_PATH ];
  int     i;
  size_t  size;

  mpCmbo->GetText( text, MAX_PATH );

  size = _tcslen( text ) + 2;

  if ( size < MAX_PATH && text[ 0 ] != '\0' )
    {
      _tcscpy_s( pt.wclist, MAX_PATH, text );

      for ( i = 0; i < mpCmbo->GetCount(); i++ )
        {
          mpCmbo->GetLBText( i, wc );

          size += ( _tcslen( wc ) + 2 );

          if ( size > MAX_PATH )
            {
              break;
            }

          if ( _tcsicmp( wc, text ) == 0 )
            {
              continue;
            }

          _tcscat_s( pt.wclist, MAX_PATH, _T( " " ) );
          _tcscat_s( pt.wclist, MAX_PATH, wc );
        }
    }
}
// ---------------------------------------------------------------------------
