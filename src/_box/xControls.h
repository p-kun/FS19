/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: xControls.h
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
#ifndef _XCONTROLS_H_
#define _XCONTROLS_H_


/*===========================================================================

  -- xControls クラス --

 ===========================================================================*/
class xControls : public _boxMsgCore
{
public:
  HWND  Create( _boxCore *pParent, int x = 0, int y = 0, int cx = 64, int cy = 24 );
  HWND  Create( HWND hWnd, int x = 0, int y = 0, int cx = 64, int cy = 24 );

public:
  void SetFont( HFONT hFont );
  void SetText( const TCHAR *pBuf );
  void GetText( TCHAR *pBuf, int len );
  void Enable( BOOL bEnable );

public:
  virtual int DrawItem( const DRAWITEMSTRUCT *pDIS ) { return 0; }
  virtual int MeasureItem( MEASUREITEMSTRUCT *pMIS ) { return 0; }
  virtual LRESULT DefaultMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
  virtual int OnMessage( UINT msg, WPARAM wParam, LPARAM lParam );

public:
  virtual ~xControls();

};
/*==========================================================================*/


/*===========================================================================

  -- xButton クラス --

 ===========================================================================*/
class xButton : public xControls
{
public:
  xButton()
    {
      mpClassName = _T( "BUTTON" );
    }

public:
  int   GetCheck();
  void  SetCheck( int check );
  int   GetState();
  void  SetState( int state );
};
/*==========================================================================*/


/*===========================================================================

  -- xEdit クラス --

 ===========================================================================*/
class xEdit : public xControls
{
public:
  xEdit();

public:
  void  LimitText( int max );
  void  SetSel( int start, int end );
  void  ReplaceSel( TCHAR *pReplace );
  int   GetLineCount();
  int   GetLine( int line, TCHAR *pBuf, int cchMax );
  DWORD GetSel();
};
/*==========================================================================*/


/*===========================================================================

  -- xListBox クラス --

 ===========================================================================*/
class xListBox : public xControls
{
public:
  xListBox();

public:
  int InsertString( int i, const TCHAR *pName );
  int FindString( int indexStart, const TCHAR *pName );
  int SelectString( int indexStart, const TCHAR *pName );
  int GetSel( int index );
  int DeleteString( int index );
  int SetSel( int select, int index );
  int GetCurSel();
  int SetCurSel( int index );
  int GetText( int index, TCHAR *pBuffer );
  int GetCount();
  int GetTextLen( int index );

  // ※ すでにアイテムが挿入されている項目について有効！！

  int   SetItemData( int index, WPARAM data );
  WPARAM GetItemData( int index );
  int   AddString( const TCHAR *pBuf );
};
/*==========================================================================*/


/*===========================================================================

  -- xStatic クラス --

 ===========================================================================*/
class xStatic : public xControls
{
public:
  xStatic()
    {
      mpClassName = _T( "STATIC" );
      mStyle      = WS_VISIBLE | WS_CHILD;
    }
};
/*==========================================================================*/


/*===========================================================================

  -- xComboBox クラス --

 ===========================================================================*/
class xComboBox : public xControls
{
public:
  xComboBox();

public:
  int  InsertString( int index, const TCHAR *cp );
  void GetText( TCHAR *pBuf, int len );
  int  GetLBText( int index, TCHAR *pBuf );
  int  GetLBTextLen( int index );
  int  GetCount();
  int  SetCurSel( int index );
  int  GetCurSel();
  int  ShowDropdown( BOOL fShow );
};
/*==========================================================================*/


/*===========================================================================

  -- xProgress クラス --

 ===========================================================================*/
class xProgress : public xControls
{
public:
  xProgress();

public:
  void SetRange( int min, int max );
  void SetPos( int pos );
  void SetStep( int step );
  void StepIt();
};
/*==========================================================================*/


/*===========================================================================

  -- xStatusBar クラス --

 ===========================================================================*/
class xStatusBar : public xControls
{
public:
  xStatusBar();

public:
  void SetText( const TCHAR *pBuf );
  void SetSimple( BOOL isSimple = TRUE );
};
/*==========================================================================*/


/*===========================================================================

  -- xListView クラス --

 ===========================================================================*/
class xListView : public xControls
{
public:
  xListView();

public:
  int   InsertItem( const LV_ITEM *pItem );
  BOOL  DeleteItem( int i );
  int   InsertColumn( int i, const LV_COLUMN *pCol );
  BOOL  SetItem( const LV_ITEM *pItem );
  DWORD SetExtendedListViewStyle( DWORD style );
  DWORD GetExtendedListViewStyle();
};
/*==========================================================================*/

#endif  /* _XCONTROLS_H_ */
