/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: xControls.cpp
 *  Created  : 08/06/04(水) 08:28:31
 *
 *  Function
 *    コントロールの基底クラス
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/07/27 10:32:49 $
 *  $Author: poko $
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 */
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>
#include "_boxCore.h"
#include "_boxMsgCore.h"
#include "xControls.h"


// ==========================================================================
// -- クラスが破棄されたときの処理
// --------------------------------------------------------------------------
xControls::~xControls()
{
  if ( IsWindow( mhWnd ) )
    {
      /* ウインドウがまだ存在していたら破棄！ */

      ::SetWindowLongPtr( mhWnd, GWLP_WNDPROC, ( LONG_PTR )mpControlFunc );
      ::DestroyWindow( mhWnd );
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- ウインドウの作成
// --------------------------------------------------------------------------
HWND xControls::Create( _boxCore *pParent, int x, int y, int cx, int cy )
{
  return Create( pParent->mhWnd, x, y, cx, cy );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- ウインドウの作成
// --------------------------------------------------------------------------
HWND xControls::Create( HWND hParentWnd, int x, int y, int cx, int cy )
{
  HWND  hWnd = _boxCore::Create( hParentWnd, x, y, cx, cy );

  Attach( hWnd );

  return mhWnd;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- デフォルトメッセージ処理
// --------------------------------------------------------------------------
LRESULT xControls::DefaultMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  return ( int )::CallWindowProc( mpControlFunc, mhWnd, msg, wParam, lParam );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- その他のメッセージ毎の処理
// --------------------------------------------------------------------------
int xControls::OnMessage( UINT msg, WPARAM wParam, LPARAM lParam )
{
  return ( int )::CallWindowProc( mpControlFunc, mhWnd, msg, wParam, lParam );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- xControlsのメソッド
// --------------------------------------------------------------------------
void xControls::Enable( BOOL bEnable ) { EnableWindow( mhWnd, bEnable ); }
void xControls::SetFont( HFONT hFont ) { SetWindowFont( mhWnd, hFont, TRUE ); }
void xControls::SetText( const TCHAR *pBuf ) { SetWindowText( pBuf ); }
void xControls::GetText( TCHAR *pBuf, int len ) { GetWindowText( pBuf, len ); }
// --------------------------------------------------------------------------


// ==========================================================================
// -- xButtonのメソッド
// --------------------------------------------------------------------------
int  xButton::GetCheck() { return Button_GetCheck( mhWnd ); }
void xButton::SetCheck( int check ) { Button_SetCheck( mhWnd, check ); }
int  xButton::GetState() { return Button_GetState( mhWnd ); }
void xButton::SetState( int state ) { Button_SetState( mhWnd, state ); }
// --------------------------------------------------------------------------


// ==========================================================================
// -- xEditのメソッド
// --------------------------------------------------------------------------
xEdit::xEdit() { mpClassName = _T( "EDIT" ); mExStyle = WS_EX_CLIENTEDGE; }
void  xEdit::LimitText( int max ) { Edit_LimitText( mhWnd, max ); }
void  xEdit::SetSel( int start, int end ) { Edit_SetSel( mhWnd, start, end ); }
void  xEdit::ReplaceSel( TCHAR *pReplace ) { Edit_ReplaceSel( mhWnd, pReplace ); }
DWORD xEdit::GetSel() { return Edit_GetSel( mhWnd ); }
int   xEdit::GetLineCount() { return Edit_GetLineCount( mhWnd ); }
int   xEdit::GetLine( int line, TCHAR *pBuf, int cchMax ) { return Edit_GetLine( mhWnd, line, pBuf, cchMax ); }
// --------------------------------------------------------------------------


// ==========================================================================
// -- xListBoxのメソッド
// --------------------------------------------------------------------------
xListBox::xListBox() { mpClassName = _T( "LISTBOX" ); mExStyle = WS_EX_CLIENTEDGE; }
int    xListBox::InsertString( int i, const TCHAR *pName ) { return ListBox_InsertString( mhWnd, i, pName ); }
int    xListBox::FindString( int indexStart, const TCHAR *pName ) { return ListBox_FindString( mhWnd, indexStart, pName ); }
int    xListBox::SelectString( int indexStart, const TCHAR *pName ) { return ListBox_SelectString( mhWnd, indexStart, pName ); }
int    xListBox::GetSel( int index ) { return ListBox_GetSel( mhWnd, index ); }
int    xListBox::DeleteString( int index ) { return ListBox_DeleteString( mhWnd, index ); }
int    xListBox::SetSel( int select, int index ) { return ListBox_SetSel( mhWnd, select, index ); }
int    xListBox::GetCurSel() { return ListBox_GetCurSel( mhWnd ); }
int    xListBox::SetCurSel( int index ) { return ListBox_SetCurSel( mhWnd, index ); }
int    xListBox::GetText( int index, TCHAR *pBuffer ) { return ListBox_GetText( mhWnd, index, pBuffer ); }
int    xListBox::GetCount() { return ListBox_GetCount( mhWnd ); }
int    xListBox::GetTextLen( int index ) { return ListBox_GetTextLen( mhWnd, index ); }
// ※ すでにアイテムが挿入されている項目について有効！！
int    xListBox::SetItemData( int index, WPARAM data ) { return ListBox_SetItemData( mhWnd, index, data ); }
WPARAM xListBox::GetItemData( int index ) { return ( WPARAM )ListBox_GetItemData( mhWnd, index ); }
int    xListBox::AddString( const TCHAR *pBuf ) { return ListBox_AddString( mhWnd, pBuf ); }
// --------------------------------------------------------------------------


// ==========================================================================
// -- xComboBoxのメソッド
// --------------------------------------------------------------------------
xComboBox::xComboBox() { mpClassName = _T( "COMBOBOX" ); mStyle = WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST; mExStyle = WS_EX_CLIENTEDGE; }
int  xComboBox::InsertString( int index, const TCHAR *cp ) { return ComboBox_InsertString( mhWnd, index, cp ); }
void xComboBox::GetText( TCHAR *pBuf, int len ) { GetWindowText( pBuf, len ); }
int  xComboBox::GetLBText( int index, TCHAR *pBuf ) { return ComboBox_GetLBText( mhWnd, index, pBuf ); }
int  xComboBox::GetLBTextLen( int index ) { return ComboBox_GetLBTextLen( mhWnd, index ); }
int  xComboBox::GetCount() { return ComboBox_GetCount( mhWnd ); }
int  xComboBox::SetCurSel( int index ) { return ComboBox_SetCurSel( mhWnd, index ); }
int  xComboBox::GetCurSel() { return ComboBox_GetCurSel( mhWnd ); }
int  xComboBox::ShowDropdown( BOOL fShow ) { return ComboBox_ShowDropdown( mhWnd, fShow ); }
// --------------------------------------------------------------------------


// ==========================================================================
// -- xProgressのメソッド
// --------------------------------------------------------------------------
xProgress::xProgress() { mpClassName = PROGRESS_CLASS; }
void xProgress::SetRange( int min, int max ) { SendMessage( mhWnd, PBM_SETRANGE, ( WPARAM )0, MAKELPARAM( min, max ) ); }
void xProgress::SetPos( int pos ) { SendMessage( mhWnd, PBM_SETPOS, pos, 0 ); }
void xProgress::SetStep( int step ) { SendMessage( mhWnd, PBM_SETSTEP, step, 0 ); }
void xProgress::StepIt() { SendMessage( mhWnd, PBM_STEPIT, 0, 0 ); }
// --------------------------------------------------------------------------


// ==========================================================================
// -- xStatusのメソッド
// --------------------------------------------------------------------------
xStatusBar::xStatusBar() { mpClassName = STATUSCLASSNAME; mStyle = WS_CHILD | SBARS_SIZEGRIP | CCS_BOTTOM | WS_VISIBLE; }
void xStatusBar::SetSimple( BOOL isSimple ) { SendMessage( mhWnd, SB_SIMPLE, isSimple, 0L ); }
void xStatusBar::SetText( const TCHAR *pBuf ) { SendMessage( mhWnd, SB_SETTEXT, 255 | 0, ( LPARAM )pBuf ); }
// --------------------------------------------------------------------------


// ==========================================================================
// -- xListViewのメソッド
// --------------------------------------------------------------------------
xListView::xListView() { mpClassName = WC_LISTVIEW; mStyle = WS_CHILD | WS_VISIBLE | LVS_REPORT; mExStyle = LVS_EX_FULLROWSELECT | LVS_EX_FLATSB; }
int   xListView::InsertItem( const LV_ITEM *pItem ) { return ListView_InsertItem( mhWnd, pItem ); }
BOOL  xListView::DeleteItem( int i ) { return ListView_DeleteItem( mhWnd, i ); }
int   xListView::InsertColumn( int i, const LV_COLUMN *pCol ) { return ListView_InsertColumn( mhWnd, i, pCol ); }
BOOL  xListView::SetItem( const LV_ITEM *pItem ) { return ListView_SetItem( mhWnd, pItem ); }
DWORD xListView::SetExtendedListViewStyle( DWORD style ) { return ListView_SetExtendedListViewStyle( mhWnd, style ); }
DWORD xListView::GetExtendedListViewStyle() { return ListView_GetExtendedListViewStyle( mhWnd ); }
// --------------------------------------------------------------------------
