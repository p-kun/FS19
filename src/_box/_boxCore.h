/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: _boxCore.h
 *  Created  : 08/06/04(水) 08:28:31
 *
 *  Function
 *    ウインドウズのAPI一覧クラス
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/07/27 10:32:48 $
 *  $Author: poko $
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 */
#ifndef __BOXCORE_H_
#define __BOXCORE_H_

#define DPIX(x)       (INT)(_boxCore::dpiX * (x) / 96.0f)
#define DPIY(y)       (INT)(_boxCore::dpiY * (y) / 96.0f)
#define DISPLAY_SCALE (INT)((_boxCore::dpiY / 96.0f) * 100)

/*===========================================================================

  -- _boxCore クラス --

 ===========================================================================*/
class _boxCore
{
public:
  int       GetWindowText( TCHAR* pString, int len );
  int       SetWindowText( const TCHAR* pString );
  int       ShowWindow( int show = SW_SHOW );
  int       UpdateWindow();
  LRESULT   SendMessage( UINT msg, WPARAM wParam, LPARAM lParam );
  LRESULT   SendMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
  LRESULT   PostMessage( UINT msg, WPARAM wParam, LPARAM lParam );
  BOOL      GetClientRect( RECT *rc );
  BOOL      GetWindowRect( RECT *rc );
  BOOL      Invalidate( BOOL isErase = TRUE );
  BOOL      MoveWindow( int x, int y, int w, int h, BOOL isRepaint = TRUE );
  BOOL      MoveWindow( RECT *rc, BOOL isRepaint = TRUE );
  _boxCore* GetParent();
  _boxCore* GetClassPtr();
  BOOL      GetWindowPlacement( WINDOWPLACEMENT *pPlace );
  UINT      PumpMessage( void );
  HDC       BeginPaint( PAINTSTRUCT *pPaint );
  void      EndPaint( PAINTSTRUCT *pPaint );
  int       MessageBox( const TCHAR *pText, const TCHAR *pCaption = _T( "" ), UINT type = MB_OK | MB_ICONASTERISK );
  UINT_PTR  SetTimer( UINT_PTR iD, UINT timout, TIMERPROC pTimrFunc = NULL );
  HWND      SetFocus();
  HWND      SetActiveWindow();
  BOOL      SetWindowPos( HWND hInsWnd = HWND_TOPMOST, int x = 0, int y = 0, int cx = 0, int cy = 0, UINT uFlags = SWP_NOMOVE | SWP_NOSIZE );
  void      SetWindowLong( int nIndex, LONG dwNewLong );
  HWND      Create( _boxCore *pParent, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT );
  HWND      Create( HWND hWnd, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT );
  BOOL      Destroy();
  // --
  int       Attach( HWND hWnd );
  // --
  TCHAR*    GetLastErrString( DWORD *pErr = NULL );
  // --
  DWORD     GetStyle() { return mStyle; }
  void      SetStyle( DWORD style ) { mStyle = style; }
  DWORD     GetStyleEx() { return mExStyle; }
  void      SetStyleEx( DWORD style ) { mExStyle = style; }
  void      UpdateScale( void );

public:
  operator HWND () const { return mhWnd; }
  operator HWND ()       { return mhWnd; }

public:
  virtual LRESULT   Message( UINT msg, WPARAM wParam, LPARAM lParam ) = 0;

public:
  static HINSTANCE  GetInstance() { return mhInstance; }
  static FLOAT      dpiX;
  static FLOAT      dpiY;

public:
  _boxCore();
  virtual ~_boxCore();

protected:
  static LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
  static LRESULT CALLBACK InitWindowFunc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

public:
  HWND              mhWnd;

protected:
  DWORD             mStyle;
  DWORD             mExStyle;
  const TCHAR*      mpClassName;      // アプリケーション名
  const TCHAR*      mpCaptionName;    // ウインドウタイトル名
  WNDPROC           mpControlFunc;

protected:
  static HINSTANCE  mhInstance;       // アプリケーションインスタンス
  static _boxCore*  theTopLevelWindow;
  static TCHAR      mErrMsg[ 128 ];

};
/*==========================================================================*/

#endif  /* __BOXCORE_H_ */
