/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iSettingDialog.cpp
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
#include "iSettingDialog.h"


// ==========================================================================
// -- cIncDirDlgクラスの構築と破棄
// --------------------------------------------------------------------------
iSettingDialog::iSettingDialog( HWND hParent ) : xDialog( _T( "IDD_SETTING_DIALOG" ), hParent )
{
  mpCmbo1 = new xComboBox;
  mpCmbo2 = new xComboBox;
  mpCmbo3 = new xComboBox;
  mpEdit1 = new xEdit;
  mpEdit2 = new xEdit;
  mpEdit3 = new xEdit;
  mpEdit4 = new xEdit;

  ZeroMemory( &mLogFont1, sizeof( LOGFONT ) );

  mLogFont1.lfHeight = 9;
  _tcscpy_s( mLogFont1.lfFaceName, _T( "ＭＳ ゴシック" ) );

  mLogFont2 = mLogFont1;

  mColumn = 0;
  mNumber = 0;
  mType   = 0;

}
// --------------------------------------------------------------------------
iSettingDialog::~iSettingDialog()
{
  delete mpCmbo1;
  delete mpCmbo2;
  delete mpCmbo3;
  delete mpEdit1;
  delete mpEdit2;
  delete mpEdit3;
  delete mpEdit4;
}
// --------------------------------------------------------------------------


// ===========================================================================
// -- ダイアログの初期化
// ---------------------------------------------------------------------------
BOOL iSettingDialog::OnInitDialog( HWND hwndFocus, LPARAM lParam )
{
  TCHAR buf[ MAX_PATH ];

  CenterWindow();   // 真ん中に移動

  Attach( mpCmbo1, IDC_COMBO1 );
  Attach( mpCmbo2, IDC_COMBO2 );
  Attach( mpCmbo3, IDC_COMBO3 );
  Attach( mpEdit1, IDC_EDIT1 );
  Attach( mpEdit2, IDC_EDIT2 );
  Attach( mpEdit3, IDC_EDIT3 );
  Attach( mpEdit4, IDC_EDIT4 );

  mpCmbo1->InsertString( 0, _T( "１列" ) );
  mpCmbo1->InsertString( 1, _T( "２列" ) );
  mpCmbo1->InsertString( 2, _T( "３列" ) );
  mpCmbo1->InsertString( 3, _T( "４列" ) );
  mpCmbo1->InsertString( 4, _T( "５列" ) );
  mpCmbo1->InsertString( 5, _T( "６列" ) );
  mpCmbo1->SetCurSel( mColumn );
  mpCmbo2->InsertString( 0, _T( "昇順" ) );
  mpCmbo2->InsertString( 1, _T( "降順" ) );
  mpCmbo2->SetCurSel( mNumber );
  mpCmbo3->InsertString( 0, _T( "名前" ) );
  mpCmbo3->InsertString( 1, _T( "拡張子" ) );
  mpCmbo3->InsertString( 2, _T( "サイズ" ) );
  mpCmbo3->InsertString( 3, _T( "日時" ) );
  mpCmbo3->SetCurSel( mType );

  wsprintf( buf, _T( "%s (%dpt)" ), mLogFont1.lfFaceName, mLogFont1.lfHeight );
  mpEdit1->SetText( buf );

  wsprintf( buf, _T( "%s (%dpt)" ), mLogFont2.lfFaceName, mLogFont2.lfHeight );
  mpEdit2->SetText( buf );

  wsprintf( buf, _T( "%s (%dpt)" ), mLogFont3.lfFaceName, mLogFont3.lfHeight );
  mpEdit3->SetText( buf );

  wsprintf( buf, _T( "%s (%dpt)" ), mLogFont4.lfFaceName, mLogFont4.lfHeight );
  mpEdit4->SetText( buf );

  return TRUE;
}
// ---------------------------------------------------------------------------


// ===========================================================================
// -- コマンドの処理
// ---------------------------------------------------------------------------
void iSettingDialog::OnCommand( int id, HWND hwndCtl, UINT codeNotify )
{
  TCHAR buf[ MAX_PATH ];

  switch( id )
    {
      case IDC_COMBO1:
        break;
      case IDC_COMBO2:
        break;
      case IDC_COMBO3:
        break;
      case IDC_EDIT1:
        break;
      case IDC_EDIT2:
        break;
      case IDC_BUTTON1:
        SetFontDialog( &mLogFont1 );
        wsprintf( buf, _T( "%s (%dpt)" ), mLogFont1.lfFaceName, mLogFont1.lfHeight );
        mpEdit1->SetText( buf );
        break;
      case IDC_BUTTON2:
        SetFontDialog( &mLogFont2, 1 );
        wsprintf( buf, _T( "%s (%dpt)" ), mLogFont2.lfFaceName, mLogFont2.lfHeight );
        mpEdit2->SetText( buf );
        break;
      case IDC_BUTTON3:
        SetFontDialog( &mLogFont3, 0 );
        wsprintf( buf, _T( "%s (%dpt)" ), mLogFont3.lfFaceName, mLogFont3.lfHeight );
        mpEdit3->SetText( buf );
        break;
      case IDC_BUTTON4:
        SetFontDialog( &mLogFont4, 1 );
        wsprintf( buf, _T( "%s (%dpt)" ), mLogFont4.lfFaceName, mLogFont4.lfHeight );
        mpEdit4->SetText( buf );
        break;
      case IDOK:
        mColumn = mpCmbo1->GetCurSel();
        mNumber = mpCmbo2->GetCurSel();
        mType   = mpCmbo3->GetCurSel();
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
BOOL iSettingDialog::SetFontDialog( LOGFONT *font, int fixed )
{
  CHOOSEFONT  cf;

  cf.lStructSize = sizeof( CHOOSEFONT );
  cf.hwndOwner   = mhWnd;
  cf.hDC         = NULL;
  cf.lpLogFont   = font;
  cf.iPointSize  = 12;

  if ( fixed )
    {
      cf.Flags   = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_NOVERTFONTS | CF_FIXEDPITCHONLY;
    }
  else
    {
      cf.Flags   = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_NOVERTFONTS;
    }

  return ChooseFont( &cf );
}
// ---------------------------------------------------------------------------


// ===========================================================================
// --
// ---------------------------------------------------------------------------
void iSettingDialog::SetParam( int column, int number, int type )
{
  if ( column > 0 )
    {
      column--;
    }

  mColumn = column;
  mNumber = number;
  mType   = type;
}
// ---------------------------------------------------------------------------


// ===========================================================================
// --
// ---------------------------------------------------------------------------
void iSettingDialog::GetParam( int *column, int *number, int *type )
{
  *column = mColumn + 1;
  *number = mNumber;
  *type   = mType;
}
// ---------------------------------------------------------------------------


// ===========================================================================
// --
// ---------------------------------------------------------------------------
void iSettingDialog::SetLogFont( int no, const TCHAR *fnam, int height )
{
  if ( no == 0 )
    {
      _tcscpy_s( mLogFont1.lfFaceName, LF_FACESIZE, fnam );
      mLogFont1.lfHeight = height;
    }
  else if ( no == 1 )
    {
      _tcscpy_s( mLogFont2.lfFaceName, LF_FACESIZE, fnam );
      mLogFont2.lfHeight = height;
    }
  else if ( no == 2 )
    {
      _tcscpy_s( mLogFont3.lfFaceName, LF_FACESIZE, fnam );
      mLogFont3.lfHeight = height;
    }
  else
    {
      _tcscpy_s( mLogFont4.lfFaceName, LF_FACESIZE, fnam );
      mLogFont4.lfHeight = height;
    }
}
// ---------------------------------------------------------------------------


// ===========================================================================
// --
// ---------------------------------------------------------------------------
LOGFONT *iSettingDialog::GetLogFont( int no )
{
  LOGFONT*  font;

  if ( no == 0 )
    {
      font = &mLogFont1;
    }
  else if ( no == 1 )
    {
      font = &mLogFont2;
    }
  else if ( no == 2 )
    {
      font = &mLogFont3;
    }
  else
    {
      font = &mLogFont4;
    }

  return font;
}
// ---------------------------------------------------------------------------
